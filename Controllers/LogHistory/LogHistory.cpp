#include "LogHistory.hpp"
#include "Generic.hpp"

#define MAXIMUM_COUNT_CPU_TO_REBOOT 10 
#define MAXIMUM_COUNT_RAM_TO_REBOOT 3 

pthread_mutex_t LogHistory::log_transfer_mutex_ = PTHREAD_MUTEX_INITIALIZER; 


LogHistory::LogHistory() : 
                        count(0),
                        full_log_path_(), 
                        log_size_(0), 
                        url_server_(), 
                        dir_upload_(), 
                        port_(), 
                        is_loaded_(false),
                        modifying_mutex_(PTHREAD_MUTEX_INITIALIZER)
                        

{
    time_upload_file_log_.RegisterTimerHandler(CheckLogSize, this); 
}

LogHistory::~LogHistory() {

}

int 
LogHistory::Start() {
    auto config = JsonConfiguration::GetInstance()->Read();
    if( !config.isMember("address-server") || !config.isMember("port") || !config.isMember("dirupload") || !config.isMember("services") ) {
        LOG_ERRO("Missing configuration "); 
        return -1; 
    } 
    port_ = config["port"].asString(); 
    dir_upload_ = config["dirupload"].asString(); 
    url_server_ = config["address-server"].asString(); 
    Service ser; 
    for(int i = 0; i < config["services"].size(); ++i) {
        ser.logpath = config["services"][i]["pathlog"].asString(); 
        ser.name = config["services"][i]["name"].asString(); 
        ser.priority = config["services"][i]["priority"].asInt(); 

        service_.push_back(ser); 
    }
    dir_upload_.append(dir_upload_.back() == '/' ? "":"/"); 
 
    LOG_CRIT("==========Component Log module==========="); 
    LOG_INFO("ADDRESS SERVER: %s", url_server_.c_str()); 
    LOG_INFO("PORT: %s", port_.c_str()); 
    LOG_INFO("DIR UPLOAD: %s", dir_upload_.c_str()); 
    LOG_INFO("LOG PATH: "); 
    for(int i = 0; i < service_.size(); ++i) {
        LOG_INFO("%s", (service_[i].logpath).c_str()); 
    }
    if ( time_upload_file_log_.Start(100, PERIODIC_UPLOAD) < 0) {
        LOG_ERRO("Could not start periodic timer to start to check log");
        return -1; 
    }
    LOG_INFO("OK"); 
    return 1; 
}

void 
LogHistory::Stop() {
    time_upload_file_log_.Stop(); 
}

int 
LogHistory::CheckLogSize(void *user_data) {
    auto data = (LogHistory *) user_data; 
    std::string command; 
    std::string log_path; 

    for(int i = 0; i < (data->service_).size(); ++i) {
        log_path += (data->service_[i].logpath); 
        log_path += " "; 
    }
    command = "du -c " + log_path + " |grep total | awk '{print $1}'; "; 
    LOG_INFO("Execute: %s", command.c_str()); 
    int log_size_ = std::stoi(ExecuteCommand(command.c_str())); 
    LOG_INFO("log size: %d", log_size_); 
    if (log_size_ >= LOG_SIZE) {
        LOG_WARN("Size of log is over limited. Trying push log into the server");
        data->LogTransfer(data);
    } 
}

int 
LogHistory::LogTransfer(void *user_data) {
    auto data = (LogHistory *)user_data; 
    // load file config 
    if(!data->is_loaded_) {
    auto config = JsonConfiguration::GetInstance()->Read();
    data->url_server_ = config["address-server"].asString();  
    data->port_ = config["port"].asString(); 
    data->dir_upload_ = config["dirupload"].asString(); 
    (data->dir_upload_).append((data->dir_upload_).back() == '/' ? "":"/"); 
    Service ser; 
    for(int i = 0; i < config["services"].size(); ++i) {
        ser.logpath = config["services"][i]["pathlog"].asString(); 
        ser.name = config["services"][i]["name"].asString(); 
        ser.priority = config["services"][i]["priority"].asInt(); 
        
        (data->service_).push_back(ser); 
    }
    data->is_loaded_ = true; 
    }
    pthread_mutex_lock(&log_transfer_mutex_); 
    CURL *curl; 
    CURLcode res; 
    std::string command; 
    std::string response; 
    long resCode = 0; 
    struct curl_httppost *formpost = NULL, *lastptr = NULL;
    struct curl_slist *headerlist = NULL;
    const char buf[] = "Expect:";
    char *dir_path; 
    std::string date = GetLocalTime(); 
    std::string macaddr = getMacAddress(); 
    std::string uploadFolder = macaddr + "_" + date; 
    dir_path = (char *)malloc((data->dir_upload_).length() + uploadFolder.length() + 1); 
    std::strcpy(dir_path, data->dir_upload_.c_str()); 
    std::strcat(dir_path, uploadFolder.c_str()); 
    std::string log_path ; 
    for(int i = 0; i < (data->service_).size(); ++i) {
        log_path += (data->service_[i].logpath); 
        log_path += " "; 
    }
    
    LOG_INFO("here"); 
    command = "tar -cvf " + std::string(dir_path) + ".tar" + " "+ log_path; 
    LOG_INFO("Execute: %s", command.c_str()); 
    if (!Execute(command)) 
        LOG_ERRO("Failed to tar file log"); 

    std::string url = data->url_server_ + ":" + data->port_ + "/logs?mac=" + macaddr; 
    std::string dir_upload = std::string(dir_path) + ".tar"; 
    LOG_INFO("Prepare upload log file to %s", url.c_str());  

    curl_global_init(CURL_GLOBAL_ALL);

    curl = curl_easy_init();

    curl_formadd(&formpost, &lastptr,
                CURLFORM_COPYNAME, "file",
                CURLFORM_FILE, dir_upload.c_str(), 
                CURLFORM_END); 

    if (curl) {
        headerlist = curl_slist_append(headerlist, buf); 
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, LogHistory::WriteCallback); 
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1L);

        res = curl_easy_perform(curl); //executable 
        if (res != CURLE_OK)
        {
            LOG_INFO("Failed to upload file %d - %s", res, curl_easy_strerror(res));
            goto clean;
        }
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &resCode);
        LOG_INFO("Sucess push log into server");
    }
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &resCode); // get response
    LOG_INFO("Response: %d - %s", resCode, response.c_str());

clean:
    curl_formfree(formpost);
    curl_easy_cleanup(curl);
    curl_slist_free_all(headerlist);
    curl_global_cleanup();

    command.clear();
    Execute("rm -rf " + std::string(dir_path));
    remove(dir_upload.c_str());
    for( int i = 0; i < (data->service_).size() ; ++i) {
    fclose(fopen(((data->service_[i]).logpath).c_str(), "w"));
    LOG_INFO("Clean %s", (data->service_[i]).logpath.c_str()); 
    usleep(10); 
    }

    // DIR *dir;
    // struct dirent *ent; 
    // FILE *fname; 
    // if ((dir = opendir(data->log_path_.c_str())) != nullptr) {
    //     while ((ent = readdir(dir)) != nullptr) {
    //         std::string s_name = ent->d_name;
    //         if (s_name.find(".log") != std::string::npos) {
    //             LOG_INFO("Clean %s", s_name.c_str());
    //             fclose(fopen((data->log_path_ + s_name).c_str(), "w"));
    //         }
    //     }
    // }

    free(dir_path);
    pthread_mutex_unlock(&log_transfer_mutex_); 
}

size_t 
LogHistory::WriteCallback(const char *buffer, size_t size, size_t nmemb, void *user_data) {
    auto response = (String*)user_data;
    response->append(buffer);
    return size * nmemb;
}





