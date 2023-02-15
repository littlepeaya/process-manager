#include "Controllers.hpp"

Controllers::Controllers() : 
                keepalive_()
{}

Controllers::~Controllers() 
{}

int 
Controllers::Start() {
    if(keepalive_.Start() < 0) {
        LOG_ERRO("Error to start Keep Alive module!");
        return -1; 
    }
    return 0; 
};

void 
Controllers::Stop() {
    keepalive_.Stop(); 
};


