#ifndef __MAV3_PROCESS_MANAGER_LOG_HISTORY__
#define __MAV3_PROCESS_MANAGER_LOG_HISTORY__

#include "Libraries/Log/LogPlus.hpp"
#include "Libraries/JsonConfiguration/JsonConfiguration.hpp"
#include "Libraries/Timer/Timer.hpp"
#include <vector>
#include <string>
#define LOG_SIZE 40 //mb 
#define PERIODIC_UPLOAD 60*1000  // 60s 



class LogHistory {
public: 
    LogHistory();
    ~LogHistory();

    int Start();
    void Stop();
    int LogTransfer(void *user_data); 

    typedef struct {
    std::string name; 
    int priority; 
    std::string logpath; 
    } Service; 

private: 
    
    std::string url_server_; 
    int count; 
    std::string full_log_path_; 
    std::vector<Service> service_; 
    std::string dir_upload_; 
    unsigned int log_size_;
    std::string port_; 
    bool is_loaded_; 
    
    static pthread_mutex_t log_transfer_mutex_;
    pthread_mutex_t modifying_mutex_;
   
    static int CheckLogSize(void *user_data); 
    static size_t WriteCallback(const char *buffer, size_t size, size_t nmemb, void *userdata);

    Timer time_upload_file_log_; 

}; 

#endif //__MAV3_PROCESS_MANAGER_LOG_HISTORY__ 