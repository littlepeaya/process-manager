#include "Controllers/KeepAlive/KeepAlive.hpp" 

KeepAlive::KeepAlive(): 
                        service_(), 
                        proxy_(nullptr),
                        is_changed(false), 
                        LBusNode::Server(LMainBus::GetInstance()), 
                        LBusNode::Client(LMainBus::GetInstance()) {
    Subscribe("get-list-of-services", LBus::GET, HandleGetListOfService, this); 
    Subscribe("stop-service", LBus::SET, HandleStopService, this);
    Subscribe("start-service", LBus::SET, HandleStartService, this);
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
        ser.monitor = true; 
        
        service_.insert(std::pair<std::string, Service> (name,ser));
        LOG_INFO("Component service: %s", name.c_str()); 
    }
    //sorting services bases on priorites 
    for(auto itr1 = service_.begin(); itr1 != service_.end(); ++itr1){
        for(auto itr2 = service_.begin(); itr2 != service_.begin() && itr2 != service_.end(); ++itr2) {
            if(itr1->second.priority < itr2->second.priority) {
                std::swap(itr1, itr2); 
            }
        }
    }
    if (LBusNode::Server::Start(KEEPALIVE_MODULE) < 0) 
        LOG_ERRO("Error to start LBus Server"); 

    if(check_priodic_time_.Start(50, CHECK_PERIODIC_TIME) < 0) {
        LOG_ERRO("Error start timer"); 
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
    for (auto &itr : data->service_) {
        if (itr.second.monitor) {
            command = "pidof " + itr.first;
            if (!Execute(command)) {
                LOG_DBUG("Service %s is not active. Trying start......", itr.first.c_str());
                data->StartService(itr.first);
                usleep(1000);
            }
            else 
                LOG_INFO("Service %s is active", itr.first.c_str()); 
        }
    }
    return 0;
}

void  
KeepAlive::HandleStopService(const LBus::Message *message, void *user_data) {
    auto request = (GVariant **)LBus::GetTransaction(&message->request);
    auto data = (KeepAlive *)user_data; 
    std::string name;
    g_variant_get(*request, "(s)", &name);
    const char *ret; 
    for (auto &service : data->service_) {
        if (strcmp(service.first.c_str(), name.c_str()) == 0) {
            if(service.second.monitor) {
                LOG_DBUG("Monitoring service %s is set true -> false", name.c_str()); 
                service.second.monitor = false;
            } 
            ret = "OK";
            goto out;
        }
    }
    LOG_WARN("Can not find service in process-manager service"); 
    ret = "FAIL";
out: 
    data->LBusNode::Server::Response(message, &ret, sizeof(&ret), [] (void *) {}); 
}

void  
KeepAlive::HandleStartService(const LBus::Message *message, void *user_data) {
    auto request = (GVariant **)LBus::GetTransaction(&message->request);
    auto data = (KeepAlive *)user_data; 
    std::string name;
    g_variant_get(*request, "(s)", &name);
    const char *ret; 
    for (auto &service : data->service_) {
        if (strcmp(service.first.c_str(), name.c_str()) == 0) {
            if(!service.second.monitor) {
                LOG_DBUG("Monitoring service %s is set false -> true", name.c_str()); 
                service.second.monitor = true; 
            }
            ret = "OK";
            goto out;
        }
    }
    LOG_DBUG("Can not find service in process-manager service"); 
    ret = "FAIL";
out: 
    data->LBusNode::Server::Response(message, &ret, sizeof(&ret), [] (void *) {}); 
}

void 
KeepAlive::HandleGetListOfService(const LBus::Message *message, void *user_data) {
    auto request = (GVariant *)LBus::GetTransaction(&message->request); 
    GVariantIter *iter; 
    std::string sname; 
    const gchar *name; 
    int ret; 
    auto data = (KeepAlive *)user_data; 
    GVariantBuilder *builders = g_variant_builder_new(G_VARIANT_TYPE("aa{sv}"));
    g_variant_builder_init(builders, G_VARIANT_TYPE("aa{sv}"));
    for (auto itr = data->service_.begin(); itr != data->service_.end(); ++itr) {
        GVariantBuilder *buil = g_variant_builder_new(G_VARIANT_TYPE("a{sv}")); 
        g_variant_builder_init(buil, G_VARIANT_TYPE("a{sv}"));
        g_variant_builder_add(buil, "{sv}", "name", g_variant_new_string(itr->first.c_str())); 
        g_variant_builder_add(buil, "{sv}", "execute", g_variant_new_string(itr->second.execute.c_str())); 
        g_variant_builder_add(buil, "{sv}", "kill", g_variant_new_string(itr->second.kill.c_str())); 
        g_variant_builder_add(buil, "{sv}", "priority", g_variant_new_int32(itr->second.priority)); 
        g_variant_builder_add(buil, "{sv}", "logpath", g_variant_new_string(itr->second.logpath.c_str()));
        g_variant_builder_add(buil, "{sv}", "monitor", g_variant_new_boolean(itr->second.monitor));  
        g_variant_builder_add(builders, "a{sv}", buil); 
    }
    data->LBusNode::Server::Response(message, &builders, sizeof(&builders), [] (void * buff) {
                                                                                auto builders = (GVariantBuilder **)buff; 
                                                                                g_variant_builder_unref(*builders); 
                                                                                g_free(*builders);}); 
}

void
KeepAlive::StartService(std::string name) {
    std::string command, res; 
    if(service_.find(name) != service_.end()) {
        command = service_[name].execute; 
        res = ExecuteCommand(command.c_str()); 
        LOG_DBUG("%s", res.c_str()); 
    } else  
        LOG_DBUG("Cannot find service in list"); 
}

void 
KeepAlive::StopService(std::string name) {
    std::string command, res; 
    if(service_.find(name) != service_.end()) {
        command = service_[name].kill; 
        res = ExecuteCommand(command.c_str()); 
        LOG_DBUG("%s", res.c_str()); 
    } else  
        LOG_DBUG("Cannot find service in list"); 
}

void
KeepAlive::RestartService(std::string name) {
    StopService(name); 
    StartService(name); 
}





    
