#include "Controllers/KeepAlive/KeepAlive.hpp" 
#include "Generic.hpp"


KeepAlive::KeepAlive() :
                        keep_(true), 
                        all_active_(false), 
                        name_services() {
        
    periodic_check_.RegisterTimerHandler(HandleKeepAlive, this);

}


KeepAlive::~KeepAlive(){

}

int 
KeepAlive::Start() { 
    auto configuration = JsonConfiguration::GetInstance()->Read();
    // for(int i = 0; i < configuration["services"].size(); ++i) {
    //     name_services[i] = configuration["services"][i].asString();
    //     LOG_INFO("Service %s is started", name_services[i].c_str()); 
    // }
    // LOG_INFO("%d", (int)all_active_); 
    
    if(!all_active_) {
        all_active_ = false;
    } 
    else   
        LOG_INFO("Check Keep Alive"); 
    
    return 1;   
}

void 
KeepAlive::Run() {
    auto configuration = JsonConfiguration::GetInstance()->Read();
    periodic_check_.Start(1*1000,15*1000);
}

void 
KeepAlive::Stop() {
    keep_ = false;
}


int 
KeepAlive::HandleKeepAlive(void *user_data) {
    auto data = (KeepAlive *) user_data; 
    auto config = JsonConfiguration::GetInstance()->Read(); 
        for (int i = 0; i < config["services"].size(); ++i) {
            std::string service; 
            service = config["services"][i].asString(); 
            std::string command = "pidof " + std::string(service);
            // int ret = std::stoi(ExecuteCommand(command.c_str())); 
            if (Execute(command)) 
                LOG_INFO("Service %s is active", service.c_str());
            else {
                LOG_DBUG("Service %s is not active. Trying Start......", service.c_str()); 
                StartService(service);
                usleep(1000);
            }
        }
    return 0; 
}

void
KeepAlive::StartService(std::string name) {
    std::string command; 
    command = "/etc/init.d/" + std::string(name) + " " + "start";
    std::string result = ExecuteCommand(command.c_str()); 
    LOG_DBUG("%s", result.c_str()); 
}


void 
KeepAlive::StopService(std::string name) {
    std::string command; 
    command = "/etc/init.d/" + std::string(name) + " " + "stop";
    std::string result = ExecuteCommand(command.c_str()); 
    LOG_DBUG("%s", result.c_str()); 
}

void
KeepAlive::RestartService(std::string name) {
    std::string command; 
    command = "/etc/init.d/" + std::string(name) + " " + "restart";
    std::string result = ExecuteCommand(command.c_str()); 
    LOG_DBUG("%s", result.c_str()); 
}



/*
if(!active_) {
    auto configuration = JsonConfiguration::GetInstance()->Read();
    std::string service; 
    for(int i = 0; i < configuration["services"].size(); ++i) {
        service = configuration["services"][i].asString();
        StartService(service);
        LOG_INFO("Service %s: Start", service.c_str());   
    }
    return 0; 
    }
*/



    