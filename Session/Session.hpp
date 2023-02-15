#ifndef __MAV3_PROCESS_MANAGER_SESSION_HPP__
#define __MAV3_PROCESS_MANAGER_SESSION_HPP__

#include "Controllers/Controllers.hpp"

class Session {
public: 
    Session() {};
    ~Session() {};

    int Start();
    void Stop();

private:

}; 

#endif //__MAV3_PROCESS_MANAGER_SESSION_HPP__