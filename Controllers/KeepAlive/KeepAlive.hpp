#ifndef __MAV3_PROCESS_MANAGER_KEEP_ALIVE_HPP__
#define __MAV3_PROCESS_MANAGER_KEEP_ALIVE_HPP__

#include<string.h>
#include<iostream>
#include<unistd.h>

#include "Libraries/JsonConfiguration/JsonConfiguration.hpp"
#include "Libraries/Log/LogPlus.hpp"
#include "Libraries/Timer/Timer.hpp"

#define CHECK_PERIODIC_TIME  15*1000 //15s 

typedef struct {
    std::string name; 
    int periority; 
    bool is_monitor; 
} Service; 

class KeepAlive {
public: 
    KeepAlive(); 
    ~KeepAlive();

    int Start();
    void Stop();

private: 
    bool keep_;
    bool all_active_; 

    static void StartService(std::string name);
    void StopService(std::string name);
    void RestartService(std::string name); 
    static int HandleKeepAlive(void *user_data); 

    Timer check_priodic_time_;
    Service service_; 
};


#endif //__MAV3_PROCESS_MANAGER_KEEP_ALIVE_HPP__ 