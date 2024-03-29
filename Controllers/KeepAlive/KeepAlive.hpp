#ifndef __MAV3_PROCESS_MANAGER_KEEP_ALIVE_HPP__
#define __MAV3_PROCESS_MANAGER_KEEP_ALIVE_HPP__

#include<string.h>
#include<iostream>
#include<unistd.h>
#include "Generic.hpp"

#include "Libraries/LBus/LBusNode.hpp"
#include "Session/LMainBus/LMainBus.hpp"
#include "Libraries/JsonConfiguration/JsonConfiguration.hpp"
#include "Libraries/Log/LogPlus.hpp"
#include "Libraries/Timer/Timer.hpp"
#include "Libraries/Utils/Vector.hpp"

#define CHECK_PERIODIC_TIME  45*1000 //15s 


class KeepAlive : public LBusNode::Server, public LBusNode::Client {
public: 
    KeepAlive(); 
    ~KeepAlive();

    int Start();
    void Stop();

    void StartService(std::string name);
    void StopService(std::string name);
    void RestartService(std::string name);

private: 
    int count; 
    bool is_changed; 
    GDBusProxy *proxy_;
    std::map<std::string, Service> service_; 
    static int HandleKeepAlive(void *user_data); 
    static void HandleGetListOfService( const LBus::Message *message, void *user_data);
    static void HandleStopService(const LBus::Message *message, void *user_data); 
    static void HandleStartService(const LBus::Message *message, void *user_data);
    
    Timer check_priodic_time_;
    
};

#endif //__MAV3_PROCESS_MANAGER_KEEP_ALIVE_HPP__  