#ifndef __MAV3_PROCESS_MANAGER_LOG_HISTORY__
#define __MAV3_PROCESS_MANAGER_LOG_HISTORY__

#include "Libraries/Log/LogPlus.hpp"
#include "Libraries/JsonConfiguration/JsonConfiguration.hpp"
#include "Libraries/Timer/Timer.hpp"
#include <stdio.h>
#include <stdlib.h>

#define LOG_SIZE 40 //mb 
// #define PERIODIC_UPLOAD 1*24*60*60*1000 // 1 day 
#define PERIODIC_UPLOAD 60*1000 


class LogHistory {
public: 
    LogHistory();
    ~LogHistory();

    int Start();
    void Stop();
    static void LogTransfer(void *user_data); 
private: 
    static std::string url_server_; 
    int count; 
    static const char *full_log_path_; 
    static unsigned int log_size_;
    static char *log_path_;

    static int CheckLogSize(void *user_data); 
   

    Timer time_upload_file_log_; 

}; 

#endif //__MAV3_PROCESS_MANAGER_LOG_HISTORY__ 