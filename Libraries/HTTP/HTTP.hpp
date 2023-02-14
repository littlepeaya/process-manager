#ifndef LIBRARIES_HTTP_CLIENT_HPP
#define LIBRARIES_HTTP_CLIENT_HPP


#include <curl/curl.h>

#include <string>
#include <map>

namespace HTTP {

typedef enum {
    GET,
    PUT,
    POST,
    DELETE
} Method;

typedef struct {
    Method method;
    std::string endpoint;
    std::map<std::string, std::string> header, query;
    std::string body;
} Request;

typedef struct  {
    int code;
    std::map<std::string, std::string> header;
    std::string body;
} Response;

class Builder {
public:
    virtual ~Builder() {};

    virtual void Clear() = 0;

    virtual void WriteHeader(const std::string& key, const std::string& value) {};
    virtual std::string ReadHeader(const std::string& key) {};

    virtual void WriteQuery(const std::string& key, const std::string& value) {};
    virtual std::string ReadQuery(const std::string& key) {};

    virtual void WriteMethod(Method method) {};
    virtual Method ReadMethod() {};

    virtual void WriteBody() {};
    virtual std::string ReadBody() {};

    virtual void WriteStatusCode(int code) {};
    virtual int ReadStatusCode() {};
    
    virtual void WriteEndpoint(const std::string& endpoint) {};
    virtual std::string ReadEndpoint() {};
};

class RequestBuilder : public Builder {
public:
    RequestBuilder();
    ~RequestBuilder() = default;

    Request Build();
    void Clear();

    void WriteMethod(Method method);
    Method ReadMethod();

    void WriteEndpoint(const std::string& endpoint);
    std::string ReadEndpoint();

    void WriteHeader(const std::string& key, const std::string& value);
    std::string ReadHeader(const std::string& key);

    void WriteQuery(const std::string& key, const std::string& value);
    std::string ReadQuery(const std::string& value);

    void WriteBody(const std::string& body);
    std::string ReadBody();

private:
    Request request_;

};

class ResponseBuilder : public Builder {
public:
    ResponseBuilder() = default;
    ~ResponseBuilder() = default;

    Response Build();
    void Clear(); 

    void WriteStatusCode(int code);
    int ReadStatusCode();

    void WriteHeader(const std::string& header);
    void WriteHeader(const std::string& key, const std::string& value);
    std::string ReadHeader(const std::string& key);

    void WriteBody(const std::string& body);
    std::string ReadBody();

private:
    Response response_;

};

class Client {
public:
    Client();
    ~Client();

    int Open(const std::string& url);
    void Close();

    int Perform(const Request& request, Response *response);

private:
    static int number_of_instance_;
    struct curl_slist *slist_header_;
    std::string url_;
    CURL *curl_;

    void WriteBody(HTTP::Request request);
    void WriteQuery(const HTTP::Request& request);
    void WriteHeader(const HTTP::Request& request);

    static size_t  HandleWritingResponseBody(char *data, size_t size, size_t nmemb, void *user_data);
    static size_t  HandleWritingResponseHeader(char *data, size_t size, size_t nmemb, void *user_data);
};

} //HTTP

#endif //LIBRARIES_HTTP_CLIENT_HPP
