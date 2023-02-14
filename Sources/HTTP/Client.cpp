#include "Libraries/HTTP/HTTP.hpp"

#include "Libraries/Log/LogPlus.hpp"

// TODO: Request
HTTP::RequestBuilder::RequestBuilder() {
    Clear();
}

HTTP::Request HTTP::RequestBuilder::Build() {
    return request_;
}

void HTTP::RequestBuilder::Clear() {
    request_.header.clear();
    request_.header.insert(std::pair<std::string, std::string>("Accept", "*/*"));
    request_.header.insert(std::pair<std::string, std::string>("Accept-Encoding", "gzip, deflate, br"));
    request_.header.insert(std::pair<std::string, std::string>("Content-Type", "text/plain"));

    request_.method = GET;
    request_.endpoint = "/";
    request_.query.clear();
    request_.body.clear();
}

void HTTP::RequestBuilder::WriteMethod(HTTP::Method method) {
    request_.method = method;
}

HTTP::Method HTTP::RequestBuilder::ReadMethod() {
    return request_.method;
}

void HTTP::RequestBuilder::WriteQuery(const std::string& key, const std::string& value) {
    request_.query[key] = value;
}

std::string 
HTTP::RequestBuilder::ReadQuery(const std::string& key) {
    return request_.query[key];
}

void HTTP::RequestBuilder::WriteHeader(const std::string& key, const std::string& value) {
    request_.header[key] = value;
}

std::string HTTP::RequestBuilder::ReadHeader(const std::string& key) {
    return request_.header[key];
}

void HTTP::RequestBuilder::WriteEndpoint(const std::string& endpoint) {
    request_.endpoint = endpoint;
}

std::string HTTP::RequestBuilder::ReadEndpoint() {
    return request_.endpoint;
}

void HTTP::RequestBuilder::WriteBody(const std::string& body) {
    request_.body = body;
}

std::string HTTP::RequestBuilder::ReadBody() {
    return request_.body;
}

// TODO: Response
HTTP::Response HTTP::ResponseBuilder::Build() {
    return response_;
}

void HTTP::ResponseBuilder::Clear() {
    response_.code = 0;
    response_.body.clear();
    response_.header.clear();
}

void HTTP::ResponseBuilder::WriteStatusCode(int code) {
    response_.code = code;
}

int 
HTTP::ResponseBuilder::ReadStatusCode() {
    return response_.code ;
}

void 
HTTP::ResponseBuilder::WriteHeader(const std::string& header) {
    // LOG_INFO("%s, %s", __FUNCTION__, header.c_str());
}

void 
HTTP::ResponseBuilder::WriteHeader(const std::string& key, const std::string& value) {
    response_.header[key] = value;
}

std::string HTTP::ResponseBuilder::ReadHeader(const std::string& key) {
    return response_.header[key];
}

void 
HTTP::ResponseBuilder::WriteBody(const std::string& body) {
    response_.body = body;
}

std::string 
HTTP::ResponseBuilder::ReadBody() {
    return response_.body;
}

// TODO: Client
int HTTP::Client::number_of_instance_ = 0;

HTTP::Client::Client() : 
                        curl_(nullptr),
                        slist_header_(nullptr) {
    if (number_of_instance_++ == 0)
        curl_global_init(CURL_GLOBAL_ALL);
}

HTTP::Client::~Client() {
    Close();

    if (slist_header_ != nullptr)
        curl_slist_free_all(slist_header_);

    if (--number_of_instance_ == 0)
        curl_global_cleanup();
}

int 
HTTP::Client::Open(const std::string& url) {
    curl_ = curl_easy_init();
    if (curl_ == nullptr) {
        LOG_ERRO("Error allocating a CURL easy.");
        return -1;
    }
    
    url_ = url;
    if (url.find("http") == std::string::npos)
        url_ = "http://" + url;

    return 0;
}

void 
HTTP::Client::Close() {
    if (curl_ != nullptr) {
        curl_easy_cleanup(curl_);
        curl_ = nullptr;
    }
}

int 
HTTP::Client::Perform(const Request& request, Response *response) {
    CURLcode retval = CURLE_OK;
    std::string *body, *header;

    curl_easy_reset(curl_);
    curl_easy_setopt(curl_, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl_, CURLOPT_ACCEPT_ENCODING, "");

    if (request.method == HTTP::Method::GET) {
        curl_easy_setopt(curl_, CURLOPT_CUSTOMREQUEST, "GET");
    } else if (request.method == HTTP::Method::PUT) {
        curl_easy_setopt(curl_, CURLOPT_CUSTOMREQUEST, "PUT");
        WriteBody(request);
    } else if (request.method == HTTP::Method::POST) {
        curl_easy_setopt(curl_, CURLOPT_CUSTOMREQUEST, "POST");
        WriteBody(request);
    } else if (request.method == HTTP::Method::DELETE) {
        curl_easy_setopt(curl_, CURLOPT_CUSTOMREQUEST, "DELETE");
        WriteBody(request);
    }

    if (response != nullptr) {
        body = new (std::string);
        curl_easy_setopt(curl_, CURLOPT_WRITEFUNCTION, HandleWritingResponseBody);
        curl_easy_setopt(curl_, CURLOPT_WRITEDATA, body);

        header = new (std::string);
        curl_easy_setopt(curl_, CURLOPT_HEADERFUNCTION, HandleWritingResponseHeader);
        curl_easy_setopt(curl_, CURLOPT_HEADERDATA, header);
    }

    LOG_INFO("%s ---> %s", (url_ + request.endpoint).c_str(), request.body.c_str());

    WriteQuery(request);
    WriteHeader(request);
    retval = curl_easy_perform(curl_);
    if (retval != CURLE_OK) {
        LOG_ERRO("curl_easy_perform failed, err: %d", retval);
        goto out;
    }

    if (response != nullptr) {
        HTTP::ResponseBuilder response_builder;

        response_builder.WriteHeader(*header);
        delete header;

        response_builder.WriteBody(*body);
        delete body;

        *response = response_builder.Build();
        LOG_INFO("%s <--- %s", (url_ + request.endpoint).c_str(), response->body.c_str());
    }

    return 0;

out:
    if (response != nullptr) {
        delete header;
        delete body;
    }

    return -1;
}

void 
HTTP::Client::WriteBody(HTTP::Request request) {
    if (request.body.empty() == false) 
        curl_easy_setopt(curl_, CURLOPT_COPYPOSTFIELDS, request.body.c_str());
}

void
HTTP::Client::WriteHeader(const HTTP::Request& request) {
    if (request.header.empty() == false) {
        if (slist_header_ != nullptr) {
            curl_slist_free_all(slist_header_);
            slist_header_ = nullptr;
        }

        for (auto it : request.header)
            slist_header_ = curl_slist_append(slist_header_, std::string(it.first + ": " + it.second).c_str());

        curl_easy_setopt(curl_, CURLOPT_HTTPHEADER, slist_header_);
    }
}

void
HTTP::Client::WriteQuery(const HTTP::Request& request) {
    std::string uri = url_ + request.endpoint;

    if (request.query.empty() == false) {
        uri += "?";
        for (auto it : request.query)
            uri += it.first + "=" + it.second + "&";
        
        // remove the last & character
        uri.pop_back();
    }

    curl_easy_setopt(curl_, CURLOPT_URL, uri.c_str());
}

size_t  
HTTP::Client::HandleWritingResponseBody(char *data, size_t size, size_t nmemb, void *user_data) {
    auto body = (std::string *)user_data;
    int total = size * nmemb;

    body->append(data, total);
    return total;
}

size_t  
HTTP::Client::HandleWritingResponseHeader(char *data, size_t size, size_t nmemb, void *user_data) {
    auto header = (std::string *)user_data;
    int total = size * nmemb;

    header->append(data, total);
    return total;
}
