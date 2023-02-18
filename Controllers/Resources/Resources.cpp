#include "Resources.hpp"

int Resources::check_RAM_ = 0;
int Resources::check_CPU_ = 0; 

Resources::Resources() : 
                        is_stable_(true)
{

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
            CheckResource(0, TIME_CHECK); 
            LOG_INFO("Resources is avaible acquired successfully"); 
            is_stable_ = true; 
            return 1; 
        }
    }
}

void 
Resources::CheckResource(int timepoint, int timeval) {
    time_.Start(0,50);
    HandleStatusCPUusage();
    HandleStatusRAMIsOver();
    sleep(5); 
}

void 
Resources::Stop() {
    // time_.CancelTimerHandler(CheckResource);

    is_stable_ = true; 

}

void 
Resources::HandleStatusCPUusage() {


}

void 
Resources::HandleStatusRAMIsOver() {

}


