#ifndef __MAV3_PROCESS_MANAGER_SESSION_HPP__
#define __MAV3_PROCESS_MANAGER_SESSION_HPP__

#include "Controllers/Controllers.hpp"

#include <iostream>

#include "Session/LMainBus/LMainBus.hpp"
#include "Libraries/JsonConfiguration/JsonConfiguration.hpp" 
#include "Session/LocalNetwork.hpp"

class Session {
public: 
    Session();
    ~Session();

    int Start();
    void Stop();

private:
    LocalNetwork local_network;
    LBus *main_bus; 


}; 

#endif //__MAV3_PROCESS_MANAGER_SESSION_HPP__