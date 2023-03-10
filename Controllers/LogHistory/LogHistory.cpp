#include "LogHistory.hpp"
#include "Generic.hpp"

unsigned int LogHistory::log_size_ = 0; 
char *LogHistory::log_path_ = nullptr;
const char *LogHistory::full_log_path_ = nullptr; 
std::string LogHistory::url_server_ = ""; 


LogHistory::LogHistory() : count(0)
{
    time_upload_file_log_.RegisterTimerHandler(CheckLogSize, this); 
}

LogHistory::~LogHistory() {

}

int 
LogHistory::Start() {
    LOG_CRIT("==========Component Log module==========="); 
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
    auto config = JsonConfiguration::GetInstance()->Read();
    url_server_ = config["logtransfer"]["server"].asCString();
    log_path_ = (char *)config["logtransfer"]["path"].asCString();   
    char *log_file = "*.log"; 
    full_log_path_ = (const char *)std::strcat(log_path_, log_file); 
    std::string command = "du -c " + std::string(full_log_path_) + " |grep total | awk '{print $1}' "; 
    int log_size_ = std::stoi(ExecuteCommand(command.c_str())); 
    if (log_size_ >= LOG_SIZE) {
        LOG_INFO("Size of log is over limited. Trying push log into the server");
        LogTransfer(data);
      
        
    } 
}

void 
LogHistory::LogTransfer(void *user_data) {
    CURL *curl; 
    CURLcode res; 
    std::string response;  
    std::string command; 
    struct curl_httppost *form = NULL, *lastptr = NULL;
    char *dir_path; 
    std::string date = GetLocalTime(); 
    std::string macaddr = getMacAddress(); 
    std::string uploadFoler = macaddr + "_" + date; 

    dir_path = (char *)malloc(std::strlen(log_path_) + uploadFoler.length() + 1); 
    auto config = JsonConfiguration::GetInstance()->Read();
    url_server_ = config["address-server"].asString(); 
    char *log_path = (char *)config["logtransfer"]["path"].asCString(); 
    std::string port = config["port"].asString(); 

    std::strcpy(dir_path, log_path); 
    std::string full_log_path = full_log_path_; 
    command = "mkdir -p " + std::string(dir_path) + uploadFoler ; 
    bool result = Execute(command);
    LOG_INFO("Execute: %d", result); 
    command = "cp -R " + std::string(full_log_path) + " " + std::string(dir_path) + uploadFoler;  
    result = Execute(command);
    LOG_INFO("Execute: %d", result); 
    command = "tar -cvf " + std::string(dir_path) + std::string(uploadFoler) + ".tar " + std::string(dir_path) + uploadFoler; 
    LOG_INFO("%s", command.c_str()); 
    result = Execute(command);
    LOG_INFO("Execute: %d", result); 
    std::string url = url_server_ + ":" + port + "/logs?mac=" + macaddr; 
    LOG_INFO("Prepare upload log file to %s", url.c_str());  
    curl = curl_easy_init();
        if (curl) {
            curl_formadd(&form, &lastptr,
                CURLFORM_COPYNAME, "file",
                CURLFORM_FILE, dir_path, 
                CURLFORM_END); 
            curl_easy_setopt(curl, CURLOPT_URL, (dir_path));
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response); 
            curl_easy_setopt(curl, CURLOPT_HTTPPOST, form);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
            LOG_INFO("push log into server"); 
            res = curl_easy_perform(curl);
            LOG_INFO("push log into server"); 
            if (res != CURLE_OK){
                LOG_INFO("Failed to upload file %d - %s", res, curl_easy_strerror(res)); 
                goto clean;
            }
            LOG_INFO("push log into server");  
        }
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &res);
    LOG_INFO("Response: %d - %s", res, response.c_str());

    command = "cat /dev/null > " + std::string(full_log_path_); 
    system(command.c_str()); 

    clean:
        curl_formfree(form); 
        curl_easy_cleanup(curl); 

        free(dir_path); 
        // remove(dir_path); 
        remove((dir_path + uploadFoler).c_str()); 
        remove((dir_path + uploadFoler + ".tar").c_str()); 
        command = "echo \"\"  >> " + std::string(full_log_path_); 
        Execute(command); 
}





