#include "LogHistory.hpp"
#include "Generic.hpp"
unsigned int LogHistory::log_size_ = 0; 
std::string LogHistory::log_path_ = ""; 


LogHistory::LogHistory() :
                            url_server_()
{
    time_upload_file_log_.RegisterTimerHandler([] (void*user_data) -> int {
        LOG_INFO("log history is running ..."); 
    }, this);
    time_upload_file_log_.RegisterTimerHandler(CheckLogSize, this); 
}

LogHistory::~LogHistory() {

}

int 
LogHistory::Start() {
    auto config = JsonConfiguration::GetInstance()->Read();
    url_server_ = config["LogTranfer"]["server"].asString();
    log_path_ = config["LogTranfer"]["path"].asString(); 
    if ( time_upload_file_log_.Start(200, PERIODIC_UPLOAD) < 0) {
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
    // auto data = (LogHistory *) user_data; 
    // std::string command = "du -m " + std::string(log_path_); 
    // log_size_ = std::stoi(ExecuteCommand(command.c_str())); 
    // LOG_INFO("size of log %s", log_size_);  
    // if (log_size_ > LOG_SIZE) {
    //     LOG_INFO("Size of log is over limited. Trying push log into the server");
    //     LogTransfer();
    } 

// }

void 
LogHistory::LogTransfer() {
    // prepare: 
    
    // std::string command; 
    // command = " cd " + log_path_;
    // command += " tar -zcvf *.log";
    

}