#ifndef __MAV3_PROCESS_MANAGER_LOG_HISTORY__
#define __MAV3_PROCESS_MANAGER_LOG_HISTORY__

#include "Libraries/Log/LogPlus.hpp"
#include "Libraries/JsonConfiguration/JsonConfiguration.hpp"
#include "Libraries/Timer/Timer.hpp"


#define LOG_SIZE 50 //mb 
#define PERIODIC_UPLOAD 7*24*60*60 // 1 week 


class LogHistory {
public: 
    LogHistory();
    ~LogHistory();

    int Start();
    void Stop();
private: 
    std::string url_server_; 

    static unsigned int log_size_;
    static std::string log_path_;

    static int CheckLogSize(void *user_data); 
    static void LogTransfer(); 

    Timer time_upload_file_log_; 

}; 

#endif //__MAV3_PROCESS_MANAGER_LOG_HISTORY__ 