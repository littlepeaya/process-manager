#ifndef __MAV3_PROCESS_MANAGER_CONTROLLERS_HPP__
#define __MAV3_PROCESS_MANAGER_CONTROLLERS_HPP__

#include "Controllers/KeepAlive/KeepAlive.hpp"
#include "Controllers/LogHistory/LogHistory.hpp"
#include "Controllers/Resources/Resources.hpp"
#include "Libraries/Log/LogPlus.hpp"

class Controllers {
public:
    Controllers();
    ~Controllers();

    int Start();
    void Stop(); 

private: 
    KeepAlive keepalive_;

};


#endif //__MAV3_PROCESS_MANAGER_CONTROLLERS_HPP__