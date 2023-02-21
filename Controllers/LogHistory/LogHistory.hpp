#ifndef __MAV3_PROCESS_MANAGER_LOG_HISTORY__
#define __MAV3_PROCESS_MANAGER_LOG_HISTORY__

#include "Libraries/Log/LogPlus.hpp"
#include "Libraries/JsonConfiguration/JsonConfiguration.hpp"
#include "Libraries/Timer/Timer.hpp"

#define LOG_SIZE 50 //mb 
#define SCHEDULE_UPLOAD 7*24*60*60 // 1 week 


class LogHistory {
public: 
    LogHistory();
    ~LogHistory();

    int Start();
    void Stop();
    void LogTrangfer();
    void Run(); 
private: 
    int log_periority_;
    std::string url_server_; 
    unsigned int log_size_;

    Timer time_upload_file_log_;


}; 

#endif //__MAV3_PROCESS_MANAGER_LOG_HISTORY__