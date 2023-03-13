#include "LogHistory.hpp"
#include "Generic.hpp"

#define MAXIMUM_COUNT_CPU_TO_REBOOT 10 
#define MAXIMUM_COUNT_RAM_TO_REBOOT 3 

pthread_mutex_t LogHistory::log_transfer_mutex_ = PTHREAD_MUTEX_INITIALIZER; 


LogHistory::LogHistory() : 
                        count(0), 
                        log_path_(), 
                        full_log_path_(), 
                        log_size_(0), 
                        url_server_(), 
                        dir_upload_(), 
                        port_(), 
                        cpu_limitted_(0), 
                        ram_limmited_(0), 
                        modifying_mutex_(PTHREAD_MUTEX_INITIALIZER)

{
    time_upload_file_log_.RegisterTimerHandler(CheckLogSize, this); 
}

LogHistory::~LogHistory() {

}

int 
LogHistory::Start() {
    Json::Value root; 
    auto config = JsonConfiguration::GetInstance()->Read();
    if( !config.isMember("address-server") || !config.isMember("port") || !config.isMember("dirupload") || !config.isMember("dirlog")) {
        LOG_ERRO("Missing configuration "); 
        return -1; 
    }
    url_server_ = config["address-server"].asString();  
    port_ = config["port"].asString(); 
    dir_upload_ = config["dirupload"].asString(); 
    log_path_ = config["dirlog"][0].asString(); 
    full_log_path_ = config["dirlog"][1].asString(); 
    cpu_limitted_ = config["resources"]["cpu"].asInt(); 
    ram_limmited_ = config["resources"]["ram"].asInt(); 

    dir_upload_.append(dir_upload_.back() == '/' ? "":"/"); 
    log_path_.append(log_path_.back() == '/' ? "":"/"); 

    LOG_CRIT("==========Component Log module==========="); 
    LOG_INFO("ADDRESS SERVER: %s", url_server_.c_str()); 
    LOG_INFO("PORT: %s", port_.c_str()); 
    LOG_INFO("DIR UPLOAD: %s", dir_upload_.c_str()); 
    LOG_INFO("FILE LOG: %s %s ", log_path_.c_str(), full_log_path_.c_str()); 
    LOG_INFO("RAM LIMITTED: %d MB", ram_limmited_); 
    LOG_INFO("CPU LIMITTED: %d percent", ram_limmited_); 
    if ( time_upload_file_log_.Start(100, PERIODIC_UPLOAD) < 0) {
        LOG_ERRO("Could not start periodic timer to start to check log");
        return -1; 
    }
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
    command = "du -c " + data->log_path_ + "*.log" + " |grep total | awk '{print $1}'; "; 
    command += "du -c " + data->full_log_path_ + " |grep total | awk '{print $1}'; ";
    int log_size_ = std::stoi(ExecuteCommand(command.c_str())); 
    if (log_size_ >= LOG_SIZE) {
        LOG_WARN("Size of log is over limited. Trying push log into the server");
        LogTransfer(data);
    } 
}

int 
LogHistory::LogTransfer(void *user_data) {
    auto data = (LogHistory *)user_data; 
    CURL *curl; 
    CURLcode res; 
    std::string command; 
    std::string response; 
    long resCode = 0; 
    struct curl_httppost *formpost = NULL, *lastptr = NULL;
    struct curl_slist *headerlist = NULL;
    static const char buf[] = "Expect:";
    char *dir_path; 

    pthread_mutex_lock(&log_transfer_mutex_); 
    std::string date = GetLocalTime(); 
    std::string macaddr = getMacAddress(); 
    std::string uploadFolder = macaddr + "_" + date; 
    dir_path = (char *)malloc(data->dir_upload_.length() + uploadFolder.length() + 1); 
    std::strcpy(dir_path, data->dir_upload_.c_str()); 
    std::strcat(dir_path, uploadFolder.c_str()); 
    command = "mkdir -p " + std::string(dir_path) + "; "; 
    command += "cp -L " + data->log_path_ + "*.log" + " " + data->full_log_path_ + " " + std::string(dir_path) + "; "; 
    command += "tar -cvf " + std::string(dir_path) + ".tar" + " "+ std::string(dir_path); 
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
    fclose(fopen((data->full_log_path_).c_str(), "w"));
    LOG_INFO("Clean %s", data->full_log_path_.c_str()); 

    DIR *dir;
    struct dirent *ent; 
    FILE *fname; 
    if ((dir = opendir(data->log_path_.c_str())) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {
            std::string s_name = ent->d_name;
            if (s_name.find(".log") != std::string::npos) {
                LOG_INFO("Clean %s", s_name.c_str());
                fclose(fopen((data->log_path_ + s_name).c_str(), "w"));
            }
        }
    }

    free(dir_path);

    pthread_mutex_unlock(&log_transfer_mutex_); 
}

size_t 
LogHistory::WriteCallback(const char *buffer, size_t size, size_t nmemb, void *user_data) {
    auto response = (String*)user_data;
    response->append(buffer);
    return size * nmemb;
}





