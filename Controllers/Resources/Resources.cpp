#include "Resources.hpp"

int Resources::check_RAM_ = 0;
int Resources::check_CPU_ = 0; 

Resources::Resources() : 
                        is_stable_(true) { 
    time_.RegisterTimerHandler(HandleStatusCPUusage, this); 
    time_.RegisterTimerHandler(HandleStatusRAMIsOver,this); 
}

Resources::~Resources()
{

}

int 
Resources::Start() {
    while(true) {
        if(!is_stable_) {
            LOG_DBUG("Resources is not released"); 
            return -1; 
        }
        else {
            // CheckResource(0, TIME_CHECK); 
            LOG_INFO("Resources is avaible acquired successfully"); 
            is_stable_ = true; 
            return 1; 
        }
        sleep(TIME_CHECK); 
    }
    return 1; 
}
void 
Resources::Run() {
    time_.Start(2*1000,7*1000);
}

void 
Resources::CheckResource(int timepoint, int timeval) {
    
}

void 
Resources::Stop() {
    // time_.CancelTimerHandler(CheckResource);

    is_stable_ = true; 

}

int 
Resources::HandleStatusCPUusage(void *user_data) {
    auto data = (Resources *) user_data;
    LOG_INFO(" percentCPU check"); 

}

int  
Resources::HandleStatusRAMIsOver(void *user_data) {
    auto data = (Resources *) user_data; 
    LOG_INFO("RAM check"); 

}



