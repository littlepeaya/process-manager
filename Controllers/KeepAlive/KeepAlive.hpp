#ifndef __MAV3_PROCESS_MANAGER_KEEP_ALIVE_HPP__
#define __MAV3_PROCESS_MANAGER_KEEP_ALIVE_HPP__

#include<string.h>
#include<iostream>
#include<unistd.h>
#include "Generic.hpp"

#include "Libraries/LBus/LBus.hpp"
#include "Libraries/JsonConfiguration/JsonConfiguration.hpp"
#include "Libraries/Log/LogPlus.hpp"
#include "Libraries/Timer/Timer.hpp"
#include "Libraries/Utils/Vector.hpp"

#define CHECK_PERIODIC_TIME  15*1000 //15s 


class KeepAlive {
public: 
    KeepAlive(std::string name); 
    KeepAlive(); 
    ~KeepAlive();

    int Start();
    void Stop();

    static void StartService(std::string name);
    static void StopService(std::string name);
    static void RestartService(std::string name);

private: 
    bool keep_;
    bool all_active_; 
    std::string name_of_service_stop_; 
    int count; 
    GDBusProxy *proxy_;
     
    static int HandleKeepAlive(void *user_data); 

    static GVariant *HandleStopOnlyService(LBus::Message* message, void *user_data); 
    static GVariant *StatusService(std::string &name, void *user_data); 

    Timer check_priodic_time_;
    
};


#endif //__MAV3_PROCESS_MANAGER_KEEP_ALIVE_HPP__ 