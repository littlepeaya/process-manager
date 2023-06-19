#include "LogHistory.hpp"

pthread_mutex_t LogHistory::log_transfer_mutex_ = PTHREAD_MUTEX_INITIALIZER; 

LogHistory::LogHistory() : 
                        dir_log_path_(), 
                        url_server_(), 
                        dir_upload_(), 
                        port_(), 
                        is_loaded_(false),
                        modifying_mutex_(PTHREAD_MUTEX_INITIALIZER) {
    time_upload_file_log_.RegisterTimerHandler(CheckLogSize, this); 
}

LogHistory::~LogHistory() { 

}

int 
LogHistory::Start() {
    auto config = JsonConfiguration::GetInstance()->Read();
    if( !config.isMember("address-server") || 
        !config.isMember("port") || 
        !config.isMember("dirupload") || 
        !config.isMember("services") ) {
        LOG_ERRO("Missing configuration"); 
        return -1; 
    } 
    if(!is_loaded_) {
    port_ = config["port"].asString(); 
    dir_upload_ = config["dirupload"].asString(); 
    url_server_ = config["address-server"].asString(); 
    dir_log_path_ = config["log"]["path"].asString(); 
    Service ser; 
    for(auto &name : config["services"].getMemberNames()) {
        ser.logpath = config["services"][name]["pathlog"].asString(); 
        ser.priority = config["services"][name]["priority"].asInt(); 
        ser.execute = config["services"][name]["execute"].asString(); 
        ser.kill = config["services"][name]["kill"].asString(); 

        service_.insert(std::pair<std::string, Service> (name,ser)); 
    }
    dir_upload_.append(dir_upload_.back() == '/' ? "":"/"); 

    is_loaded_ = true;
    }
    LOG_CRIT("==========Component Log module==========="); 
    LOG_INFO("ADDRESS SERVER: %s", url_server_.c_str()); 
    LOG_INFO("PORT: %s", port_.c_str()); 
    LOG_INFO("DIR UPLOAD: %s", dir_upload_.c_str()); 
    LOG_INFO("LOG PATH: "); 
    LOG_INFO("%s", dir_log_path_.c_str()); 
    for(auto it = service_.begin(); it != service_.end(); ++it) {
        LOG_INFO("%s", it->second.logpath.c_str()); 
    }
    
    if ( time_upload_file_log_.Start(100, PERIODIC_CHECK) < 0) {
        LOG_ERRO("Could not start periodic timer to start to check log");
        return -1; 
    }
    return 1; 
}

void 
LogHistory::Stop() {
    time_upload_file_log_.Stop(); 
    time_upload_file_log_.CancelTimerHandler(CheckLogSize); 
}

int 
LogHistory::CheckLogSize(void *user_data) {
    auto data = (LogHistory *) user_data; 
    int log_size = 0; 
    struct stat stat_buf; 
    stat(data->dir_log_path_.c_str(), &stat_buf); 
    log_size = stat_buf.st_size; 
    for(auto itr = data->service_.begin(); itr != data->service_.end(); ++itr) {
        stat(itr->second.logpath.c_str(), &stat_buf); 
        log_size += stat_buf.st_size; 
    }
    if ((log_size/(1024*1024)) >= LOG_SIZE ) { // convert to mb   
        LOG_WARN("Size of log is over limited. Trying push log into the server");
        data->LogTransfer(data);
    }
    // periodic upload file among 1:00 a.m to 2:00 a.m
    time_t second = GetLocalTimestamp() % 86400;
    if (second >= 3600 && second <= 7200) {
        LOG_INFO("push log to server periodic"); 
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
        data->dir_log_path_ = config["log"]["path"].asString(); 
        (data->dir_upload_).append((data->dir_upload_).back() == '/' ? "":"/"); 
        Service ser; 
        for(auto &name : config["services"].getMemberNames()) {
            ser.logpath = config["services"][name]["pathlog"].asString(); 
            ser.priority = config["services"][name]["priority"].asInt(); 
            ser.execute = config["services"][name]["execute"].asString();            
            ser.kill = config["services"][name]["kill"].asString(); 

            service_.insert(std::pair<std::string, Service> (name,ser)); 
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
    std::string log_path = data->dir_upload_+ "*.log";
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
    // rmdir(dir_path); 
    Execute("rm -rf " + std::string(dir_path));                                                                                                                                                                                                                                                                                                                  
    remove(dir_upload.c_str());
    for( auto itr = data->service_.begin(); itr != data->service_.end(); ++itr) {
        fclose(fopen((itr->second.logpath).c_str(), "w")); 
        LOG_INFO("Clean %s", itr->second.logpath.c_str());
        usleep(10);
    }
    fclose(fopen((data->dir_log_path_).c_str(), "w"));
        LOG_INFO("Clean %s", data->dir_log_path_.c_str());
                                                                                                                  
    free(dir_path); 
    pthread_mutex_unlock(&log_transfer_mutex_); 
}

size_t 
LogHistory::WriteCallback(const char *buffer, size_t size, size_t nmemb, void *user_data) {
    auto response = (String*)user_data;
    response->append(buffer);
    return size * nmemb;
}




