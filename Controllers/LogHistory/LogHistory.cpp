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
    full_log_path_ = (const char *)std::strcat(log_path_, "/*.log"); 
    std::string command = "du -c " + std::string(full_log_path_) + " |grep total | awk '{print $1}' "; 
    int log_size_ = std::stoi(ExecuteCommand(command.c_str())); 
    if (log_size_ >= LOG_SIZE) {
        LOG_WARN("Size of log is over limited. Trying push log into the server");
        LogTransfer(data);
    } 
}

void 
LogHistory::LogTransfer(void *user_data) {
    CURL *curl; 
    CURLcode res; 
    std::string command; 
    std::string response; 
    long resCode = 0; 
    struct curl_httppost *formpost = NULL, *lastptr = NULL;
    struct curl_slist *headerlist = NULL;
    static const char buf[] = "Expect:";
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
    std::string full_log_path = full_log_path_ ; 
    command += "mkdir -p " + std::string(dir_path) + "/" + uploadFoler + "; " ; 
    command += "cp -R " + std::string(full_log_path) + " " + std::string(dir_path) +  "/" + uploadFoler + "; ";  
    command += "tar -cvf " + std::string(dir_path) + std::string(uploadFoler) + ".tar " + std::string(dir_path) +  "/" + uploadFoler + "; "; 
    LOG_INFO("Execute: %s", command.c_str());  
    if (!Execute(command)) 
        LOG_ERRO("Failed to tar file log"); 
    std::string url = url_server_ + ":" + port + "/logs?mac=" + macaddr; 
    LOG_INFO("Prepare upload log file to %s", url.c_str());  

    curl_global_init(CURL_GLOBAL_ALL);

    curl_formadd(&formpost, &lastptr,
                CURLFORM_COPYNAME, "file",
                CURLFORM_FILE, (std::string(dir_path) + std::string(uploadFoler) + ".tar ").c_str(), // + /var/log/7c53_wed.tar 
                CURLFORM_END); // create a http request 

    curl = curl_easy_init();

    if (curl) {
        headerlist = curl_slist_append(headerlist, buf); 
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
        curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);
        LOG_INFO("push log into server");

        res = curl_easy_perform(curl); //executable 
        LOG_INFO("push log into server");
        if (res != CURLE_OK)
        {
            LOG_INFO("Failed to upload file %d - %s", res, curl_easy_strerror(res));
            goto clean;
        }
        LOG_INFO("Sucess push log into server");
        // goto clean;
    }
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &resCode); //get response 
    LOG_INFO("Response: %d - %s", resCode, *headerlist);


clean:
    curl_formfree(formpost);
    curl_easy_cleanup(curl);
    curl_slist_free_all(headerlist);
    curl_global_cleanup();

    command.clear(); 
    std::strcat(dir_path, "/"); 
    Execute("rm -rf "+ std::string(dir_path) +  uploadFoler );    
    remove((dir_path +  uploadFoler + ".tar").c_str());
    
    DIR* dir;
    struct dirent* ent;
    if ((dir = opendir(log_path)) != nullptr) {
        while ((ent = readdir(dir)) != nullptr) {
            std::string fname = ent->d_name; 
            if(fname.find(".log") != std::string::npos) {
                LOG_INFO("%s", fname.c_str()); 
                command += "cat /dev/null > " + std::string(log_path) + "/" + String(ent->d_name) + "; ";
            }
        } 
    }
    
    LOG_INFO("%s", command.c_str()); 
     if (!Execute(command)) 
        LOG_ERRO("Failed to tar file log"); 
    free(dir_path);
}





