#include "LogHistory.hpp"
#include "Generic.hpp"

unsigned int LogHistory::log_size_ = 0; 
char *LogHistory::log_path_ = nullptr;
const char *LogHistory::full_log_path = nullptr; 
std::string LogHistory::url_server_ = ""; 


LogHistory::LogHistory() 
{
    time_upload_file_log_.RegisterTimerHandler(CheckLogSize, this); 
}

LogHistory::~LogHistory() {

}

int 
LogHistory::Start() {
     
    if ( time_upload_file_log_.Start(200, PERIODIC_UPLOAD) < 0) {
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
    url_server_ = config["LogTranfer"]["server"].asCString();
    
    log_path_ =(char *) config["LogTranfer"]["path"].asCString();   
    char *log_file = "*.log"; 
    full_log_path = (const char *)std::strcat(log_path_, log_file); 
    std::string command = "du -c " + std::string(full_log_path) + " |grep total | awk '{print $1}' "; 
    int log_size_ = std::stoi(ExecuteCommand(command.c_str())); 
    if (log_size_ >= LOG_SIZE) {
        LOG_INFO("Size of log is over limited. Trying push log into the server");
        LogTransfer();
    } 
}

void 
LogHistory::LogTransfer() {
    CURL *curl; 
    CURLcode res; 
    time_t time_; 
    struct timeval tv;
    struct tm gm;  
    struct tm *gmp; 
    std::string command; 
    struct curl_httppost *form = NULL;
    struct curl_httppost *lastptr = NULL; 

    // prepare: 
    //creat full log path 
    char *full_path = (char *)malloc(std::strlen(full_log_path) + 5);  
    char *audio_manager = "/var/log/audio-manager.log";
    Copy(audio_manager, full_log_path);  


    command = "tar -cvf " + std::string(full_log_path); 
    std::string url_to_server_ = ""; 
    ExecuteCommand(command.c_str());
    time_ = time(NULL);
   
    localtime(&time_);
    LOG_INFO("%s", ctime(&time_)); 
    char *url_to_server;
    char * url_server_ = "http://filelog.lumi.vn";  
    url_to_server = std::strcat(url_server_, ctime(&time_)); 
    curl = curl_easy_init();
        if (curl) {
            curl_formadd(&form, &lastptr,
                CURLFORM_COPYNAME, "file",
                CURLFORM_FILE, full_path, 
                CURLFORM_END); 
            curl_easy_setopt(curl, CURLOPT_URL, url_to_server_.c_str());
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);

            res = curl_easy_perform(curl);
            if (res != CURLE_OK)
                goto out;
        }

    out:
        curl_easy_cleanup(curl); 
        free(full_path); 
}



