#include "LogHistory.hpp"


LogHistory::LogHistory() :
                            log_periority_(0),
                            url_server_()
{
     time_upload_file_log_.RegisterTimerHandler([] (void*user_data) -> int {
        LOG_INFO("log history is running ..."); 
    }, this);
}

LogHistory::~LogHistory() {

}

int 
LogHistory::Start() {
    auto config = JsonConfiguration::GetInstance()->Read();
    log_periority_ = config["LogTranfer"]["periority"].asInt(); 
    url_server_ = config["LogTranfer"]["server"].asString();
    LOG_INFO("Start Log History"); 

    time_upload_file_log_.Start(200, PERIODIC_UPLOAD); 
  
}


void 
LogHistory::Stop() {

}