#include "Controllers/KeepAlive/KeepAlive.hpp" 

std::map<std::string, Service> KeepAlive::service_; 
KeepAlive::KeepAlive() : 
                        proxy_(nullptr) {
    check_priodic_time_.RegisterTimerHandler(HandleKeepAlive, this);
}


KeepAlive::~KeepAlive() {

}

int 
KeepAlive::Start() { 
    auto config = JsonConfiguration::GetInstance()->Read();
    LOG_CRIT("==========Check Keep Alive===============");
    //load services 
     if(!config.isMember("services") && !config["services"].isObject()) {
        LOG_ERRO("Missing configuration");
        return -1;
    }
    Service ser; 
    for (auto &name : config["services"].getMemberNames()) {
        if (!config["services"][name].isMember("execute") ||
            !config["services"][name].isMember("kill") || 
            !config["services"][name].isMember("pathlog") ||
            !config["services"][name].isMember("priority")) {
            LOG_ERRO("Missing members of service");
                return -1;
        } 
        ser.logpath = config["services"][name]["pathlog"].asString(); 
        ser.priority = config["services"][name]["priority"].asInt(); 
        ser.execute = config["services"][name]["execute"].asString(); 
        ser.kill = config["services"][name]["kill"].asString(); 

        service_.insert(std::pair<std::string, Service> (name,ser)); 
        LOG_INFO("Component service: %s", name.c_str()); 
    }

    proxy_ = GDBusProxyConnect(COM_AUDIO_PROCESS_BUS_NAME, COM_AUDIO_PROCESS_OBJECT_PATH, COME_AUDIO_PROCESS_CONTROLLER_INTERFACE);
    if (proxy_ == nullptr) {
        LOG_ERRO("Failed to create proxy");
        return -1; 
    }
    g_signal_connect(proxy_,
                        "g-properties-changed",
                        G_CALLBACK(HandleStopOnlyService),
                        this);
    if(check_priodic_time_.Start(50, CHECK_PERIODIC_TIME) < 0) {
        LOG_INFO("Error start timer"); 
        return -1; 
    }
    return 1;  
}

void 
KeepAlive::Stop() {
    check_priodic_time_.Stop();
    check_priodic_time_.CancelTimerHandler(HandleKeepAlive);  
}

int KeepAlive::HandleKeepAlive(void *user_data) {
    auto data = (KeepAlive *)user_data;
    std::string command;
    for (auto itr = data->service_.begin(); itr != data->service_.end(); ++itr) {
        command = "pidof " + itr->first;
        if (Execute(command))
            LOG_INFO("Service %s is active", itr->first.c_str());
        else {
            LOG_DBUG("Service %s is not active. Trying Start......", itr->first.c_str());
            StartService(itr->first);
            usleep(1000);
        }
    }
    return 0;
}

void 
KeepAlive::BlockingService(std::string name) {

}


GVariant *
KeepAlive::HandleStopOnlyService (LBus::Message * message, void * user_data) {
}

GVariant * 
KeepAlive::StatusService( std::string &name, void *user_data) {
    
}

void
KeepAlive::StartService(std::string name) {
    std::string command; 
    if(service_.find(name) != service_.end()) {
        command = service_[name].execute; 
        LOG_DBUG("%s", ExecuteCommand(command.c_str()).c_str()); 
    } else  
        LOG_DBUG("Cannot find service in list"); 
}

void 
KeepAlive::StopService(std::string name) {
    std::string command; 
    if(service_.find(name) != service_.end()) {
        command = service_[name].kill; 
        LOG_DBUG("%s", ExecuteCommand(command.c_str()).c_str()); 
    } else  
        LOG_DBUG("Cannot find service in list"); 
}

void
KeepAlive::RestartService(std::string name) {
    StopService(name); 
    StartService(name); 
}





    