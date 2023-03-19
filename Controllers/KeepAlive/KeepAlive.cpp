#include "Controllers/KeepAlive/KeepAlive.hpp" 

KeepAlive::KeepAlive(std::string name) : 
                        name_of_service_stop_(std::move(name)), 
                        proxy_(nullptr), 
                        keep_(true), 
                        all_active_(false) {
    check_priodic_time_.RegisterTimerHandler(HandleKeepAlive, this);
}

KeepAlive::KeepAlive() : 
                        proxy_(nullptr), 
                        keep_(true), 
                        all_active_(false) {
    check_priodic_time_.RegisterTimerHandler(HandleKeepAlive, this);
}


KeepAlive::~KeepAlive() {

}

int 
KeepAlive::Start() { 
    auto config = JsonConfiguration::GetInstance()->Read();
    LOG_CRIT("==========Check Keep Alive===============");
     if(!config.isMember("services") && !config["services"].isObject()) {
        LOG_ERRO("Missing configuration");
        return -1;
    }
    for (auto &name : config["services"].getMemberNames()) {
        if (!config["services"][name].isMember("execute") ||
            !config["services"][name].isMember("kill") ||
            !config["services"][name].isMember("pathlog") ||
            !config["services"][name].isMember("priority")) {
            LOG_ERRO("Missing members of service");
            return -1;
        }
        if(name_of_service_stop_ == name) 
            LOG_CRIT("Component service: %s(No monitor)", name.c_str());
        else 
            LOG_INFO("Component service: %s", name.c_str()); 
    }

    check_priodic_time_.Start(100, CHECK_PERIODIC_TIME);
    proxy_ = GDBusProxyConnect(COM_AUDIO_PROCESS_BUS_NAME, COM_AUDIO_PROCESS_OBJECT_PATH, COME_AUDIO_PROCESS_CONTROLLER_INTERFACE);
    if (proxy_ == nullptr) {
        LOG_ERRO("Failed to create proxy");
        return -1; 
    }
    g_signal_connect(proxy_,
                        "g-properties-changed",
                        G_CALLBACK(HandleStopOnlyService),
                        this);
    return 1;   
}

void 
KeepAlive::Stop() {
    check_priodic_time_.Stop(); 
}

int 
KeepAlive::HandleKeepAlive(void *user_data) {
    auto data = (KeepAlive *) user_data; 
    std::string command;
    auto config = JsonConfiguration::GetInstance()->Read(); 
        for( auto &name: config["services"].getMemberNames()) {
            if(data->name_of_service_stop_ != name) { 
                command = "pidof " + std::string(name); 
                if (Execute(command)) 
                    LOG_INFO("Service %s is active", name.c_str());
                else {
                    LOG_DBUG("Service %s is not active. Trying Start......", name.c_str()); 
                    StartService(name);
                    usleep(1000);
                }
            }
            else 
                continue;
        }
    return 0; 
}

GVariant *
KeepAlive::HandleStopOnlyService (LBus::Message * message, void * user_data) {
    LOG_INFO("something is here"); 
}

GVariant * 
KeepAlive::StatusService( std::string &name, void *user_data) {
    
}

void
KeepAlive::StartService(std::string name) {
    std::string command; 
    auto config = JsonConfiguration::GetInstance()->Read(); 
    for ( auto &find_name: config["services"].getMemberNames()) {
            if(find_name == name) {
                command = config["services"][name]["execute"].asString(); 
                LOG_DBUG("%s", ExecuteCommand(command.c_str()).c_str()); 
            }
    }
}

void 
KeepAlive::StopService(std::string name) {
    std::string command; 
    auto config = JsonConfiguration::GetInstance()->Read(); 
   for ( auto &find_name: config["services"].getMemberNames()) {
            if(find_name == name) {
                command = config["services"][name]["kill"].asString(); 
                LOG_DBUG("%s", ExecuteCommand(command.c_str()).c_str()); 
            }
    }
}

void
KeepAlive::RestartService(std::string name) {
    StopService(name); 
    StartService(name); 
}





    