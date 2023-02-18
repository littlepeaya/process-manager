#ifndef __MAV3_PROCESS_MANAGER_KEEP_ALIVE_HPP__
#define __MAV3_PROCESS_MANAGER_KEEP_ALIVE_HPP__

#include<string.h>
#include<iostream>
#include<unistd.h>

#include "Libraries/JsonConfiguration/JsonConfiguration.hpp"
#include "Libraries/Log/LogPlus.hpp"


class KeepAlive {
public: 
    KeepAlive(); 
    ~KeepAlive();

    int Start();
    void Stop();

private: 
    int active_;

    static void StartService(const std::string &name);
    static void StopService(const std::string &name);
    static void RestartService(); 
    static int CheckALiveService();
    static void HandleKeepAlive(std::string name); 
    static std::string ExecuteCommand(const char *cmd);
};


#endif //__MAV3_PROCESS_MANAGER_KEEP_ALIVE_HPP__ 