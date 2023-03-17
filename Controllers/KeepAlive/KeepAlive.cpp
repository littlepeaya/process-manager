#include "Controllers/KeepAlive/KeepAlive.hpp" 

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
    for (int i = 0; i < config["services"].size(); ++i) {
            std::string service; 
            service = config["services"][i]["name"].asString(); 
            LOG_INFO("Component service: %s", service.c_str()); 
    }

    check_priodic_time_.Start(100, CHECK_PERIODIC_TIME);
    if(!all_active_) {
        all_active_ = false;
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

    
     
    return 1;   
}

void 
KeepAlive::Stop() {
    keep_ = false;
    check_priodic_time_.Stop(); 
}

int 
KeepAlive::HandleKeepAlive(void *user_data) {
    auto data = (KeepAlive *) user_data; 
    auto config = JsonConfiguration::GetInstance()->Read(); 
        for (int i = 0; i < config["services"].size(); ++i ) {
            std::string service; 
            service = config["services"][i]["name"].asString(); 
            std::string command = "pidof " + std::string(service); 
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

GVariant *
KeepAlive::HandleStopOnlyService (LBus::Message * message, void * user_data) {
    LOG_INFO("something is here"); 
    Execute("cat /var/log/process-manager.log"); 

}

GVariant * 
KeepAlive::StatusService( std::string &name, void *user_data) {
    
}

void
KeepAlive::StartService(std::string name) {
#ifdef USE_SYSTEMD 
    std::string command; 
    command = "sudo systemctl " + std::string(name) + ".service" + " start";
    std::string result = ExecuteCommand(command.c_str()); 
    LOG_DBUG("%s", result.c_str()); 
#elif USE_INITD 
    std::string command; 
    command = "/etc/init.d/" + std::string(name) + " " + "start";
    std::string result = ExecuteCommand(command.c_str()); 
    LOG_DBUG("%s", result.c_str()); 
#endif
}

void 
KeepAlive::StopService(std::string name) {
    auto config = JsonConfiguration::GetInstance()->Read(); 
#ifdef USE_SYSTEMD 
    std::string command; 
    command = "sudo systemctl " + std::string(name) + ".service" + " stop";
    std::string result = ExecuteCommand(command.c_str()); 
    LOG_DBUG("%s", result.c_str()); 
#elif USE_INITD 
    std::string command; 
    command = "/etc/init.d/" + std::string(name) + " " + "stop";
    std::string result = ExecuteCommand(command.c_str()); 
    LOG_DBUG("%s", result.c_str()); 
#endif
}

void
KeepAlive::RestartService(std::string name) {
#ifdef USE_SYSTEMD 
    std::string command; 
    command = "sudo systemctl " + std::string(name) + ".service" + " restart";
    std::string result = ExecuteCommand(command.c_str()); 
    LOG_DBUG("%s", result.c_str()); 
#elif USE_INITD 
    std::string command; 
    command = "/etc/init.d/" + std::string(name) + " " + "restart";
    std::string result = ExecuteCommand(command.c_str()); 
    LOG_DBUG("%s", result.c_str());
#endif 
}




    