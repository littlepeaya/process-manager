#include "Session.hpp"

#define NUMBER_OF_L_MAIN_BUS_THREADS    4

Session::Session()  {

}

Session::~Session() {
    Stop();
}

int
Session::Start() {
    auto config = JsonConfiguration::GetInstance()->Read(); 

    if (local_network.Start() < 0) {
        LOG_ERRO("Error: starting local network");
        return -1;
    }

    return 0;
}

void 
Session::Stop() {
   local_network.Stop(); 
}