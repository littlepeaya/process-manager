#include "Controllers.hpp"

Controllers::Controllers() : 
                keepalive_(),
                resources_(),
                log_history_()
{}

Controllers::~Controllers() 
{}

int 
Controllers::Start() {
    // if(keepalive_.Start() < 0) {
    //     LOG_ERRO("Error to start Keep Alive module!");
    //     return -1; 
    // }
    if(resources_.Start() < 0) {
        LOG_ERRO("Error to start Resources module!");
        return -1; 
    }
    // if(log_history_.Start() < 0) {
    //     LOG_ERRO("Error to start Log History module!");
    //     return -1; 
    // }
    return 0; 
};



void 
Controllers::Stop() {
    keepalive_.Stop(); 
    resources_.Stop();
    log_history_.Stop(); 
};




