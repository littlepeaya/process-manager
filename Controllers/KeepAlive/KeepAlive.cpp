#include "Controllers/KeepAlive/KeepAlive.hpp" 

std::map<std::string, Service> KeepAlive::service_; 

KeepAlive::KeepAlive(): 
                        proxy_(nullptr),
                        LBusNode::Server(LMainBus::GetInstance()), 
                        LBusNode::Client(LMainBus::GetInstance()) {
    Subscribe("get-list-of-services", LBus::GET, HandleGetListOfService, this); 
    Subscribe("get-list-services", LBus::SET, HandleStopListOfService, this);
    Subscribe("stop-service", LBus::SET, HandleStopService, this);
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
    if (LBusNode::Server::Start(KEEPALIVE_MODULE) < 0) 
        LOG_ERRO("Error to start LBus Server"); 
    if(check_priodic_time_.Start(50, CHECK_PERIODIC_TIME) < 0) {
        LOG_INFO("Error start timer"); 
        return -1; 
    }
    proxy_ = GDBusProxyConnect(COM_AUDIO_PROCESS_BUS_NAME, COM_AUDIO_PROCESS_OBJECT_PATH, COME_AUDIO_PROCESS_CONTROLLER_INTERFACE);
    if (proxy_ == nullptr) {
        LOG_ERRO("Failed to create proxy");
        return -1; 
    }
    g_signal_connect(proxy_, 
                    "g-properties-changed", 
                    G_CALLBACK(HandleKeepAlivePropertiesChanged), 
                    this); 
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
KeepAlive::HandleStopService(const LBus::Message *message, void *user_data) {
    auto request = (GVariant *)LBus::GetTransaction(&message->request);
    auto data = (KeepAlive *)user_data; 
    const gchar *name; 
    g_variant_get(request, "s", name);
    for (auto &ser : data->service_) {
        if(ser.first.c_str() == name) 
            data->service_.erase(name); 
    }
    GVariantBuilder *builders = g_variant_builder_new(G_VARIANT_TYPE("aa{sv}"));
    g_variant_builder_init(builders, G_VARIANT_TYPE("aa{sv}"));
    for(auto &ser: data->service_) {  
        GVariantBuilder *buil = g_variant_builder_new(G_VARIANT_TYPE("a{sv}")); 
        g_variant_builder_init(buil, G_VARIANT_TYPE("a{sv}"));
        g_variant_builder_add(buil, "{sv}", "name", g_variant_new_string(ser.first.c_str())); 
        g_variant_builder_add(buil, "{sv}", "execute", g_variant_new_string(ser.second.execute.c_str())); 
        g_variant_builder_add(buil, "{sv}", "kill", g_variant_new_string(ser.second.kill.c_str())); 
        g_variant_builder_add(buil, "{sv}", "priority", g_variant_new_int32(ser.second.priority)); 
        g_variant_builder_add(buil, "{sv}", "logpath", g_variant_new_string(ser.second.logpath.c_str())); 
        g_variant_builder_add(builders, "a{sv}", buil); 
    }
    LOG_INFO("got response");  
    data->LBusNode::Server::Response(message, &builders, sizeof(&builders), [] (void * buff) {
        auto builders = (GVariantBuilder **)buff; 
        g_variant_builder_unref(*builders); 
        g_free(*builders);
    }); 
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
    for(auto &ser: data->service_) {  
        GVariantBuilder *buil = g_variant_builder_new(G_VARIANT_TYPE("a{sv}")); 
        g_variant_builder_init(buil, G_VARIANT_TYPE("a{sv}"));
        g_variant_builder_add(buil, "{sv}", "name", g_variant_new_string(ser.first.c_str())); 
        g_variant_builder_add(buil, "{sv}", "execute", g_variant_new_string(ser.second.execute.c_str())); 
        g_variant_builder_add(buil, "{sv}", "kill", g_variant_new_string(ser.second.kill.c_str())); 
        g_variant_builder_add(buil, "{sv}", "priority", g_variant_new_int32(ser.second.priority)); 
        g_variant_builder_add(buil, "{sv}", "logpath", g_variant_new_string(ser.second.logpath.c_str())); 
        g_variant_builder_add(builders, "a{sv}", buil); 
    }
    LOG_INFO("got response");  
    data->LBusNode::Server::Response(message, &builders, sizeof(&builders), [] (void * buff) {
        auto builders = (GVariantBuilder **)buff; 
        g_variant_builder_unref(*builders); 
        g_free(*builders);
    }); 
}
void 
KeepAlive::HandleStopListOfService(const LBus::Message *message, void *user_data) {
    auto request = (GVariant *)LBus::GetTransaction(&message->request); 
    GVariantIter *iter; 
    std::string sname; 
    const gchar *name; 
    int ret; 
    auto data = (KeepAlive *)user_data; 
    GVariantBuilder *builders = g_variant_builder_new(G_VARIANT_TYPE("aa{sv}"));
    g_variant_builder_init(builders, G_VARIANT_TYPE("aa{sv}"));
    for(auto &ser: data->service_) {  
        GVariantBuilder *buil = g_variant_builder_new(G_VARIANT_TYPE("a{sv}")); 
        g_variant_builder_init(buil, G_VARIANT_TYPE("a{sv}"));
        g_variant_builder_add(buil, "{sv}", "name", g_variant_new_string(ser.first.c_str())); 
        g_variant_builder_add(buil, "{sv}", "execute", g_variant_new_string(ser.second.execute.c_str())); 
        g_variant_builder_add(buil, "{sv}", "kill", g_variant_new_string(ser.second.kill.c_str())); 
        g_variant_builder_add(buil, "{sv}", "priority", g_variant_new_int32(ser.second.priority)); 
        g_variant_builder_add(buil, "{sv}", "logpath", g_variant_new_string(ser.second.logpath.c_str())); 
        g_variant_builder_add(builders, "a{sv}", buil); 
    }
    LOG_INFO("got response");  
    data->LBusNode::Server::Response(message, &builders, sizeof(&builders), [] (void * buff) {
        auto builders = (GVariantBuilder **)buff; 
        g_variant_builder_unref(*builders); 
        g_free(*builders);
    });
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

void
KeepAlive::HandleKeepAlivePropertiesChanged(GDBusProxy *proxy,
                                            GVariant *changed_properties,
                                            const gchar* const *invalidated_properties,
                                            gpointer user_data) {
    auto self = (KeepAlive *)user_data; 
     bool boolean;

    LOG_DBUG("%s, %s.", g_dbus_proxy_get_interface_name(proxy), g_variant_print(changed_properties, true));
    
    if (g_variant_n_children(changed_properties) > 0) {
        if (g_variant_lookup(changed_properties, "Paired", "b", &boolean)) {
            if (boolean == true) {
                GVariant *reply = GDBusProxyGetProperty(self->proxy_, "MainPaired");
                if(reply != nullptr) {
                    LOG_ERRO("Error get changed properties"); 
                    g_variant_unref(reply); 
                }
            }
        }
    }
}





// void KeepAlive::HandleControllerMethods(GDBusConnection *connection,
//                                         const gchar *sender,
//                                         const gchar *object_path,
//                                         const gchar *interface_name,
//                                         const gchar *method_name,
//                                         GVariant *paramenter,
//                                         GDBusMethodInvocation *invocation,
//                                         gpointer user_data) {
//     auto data = (KeepAlive *)user_data;

//     LOG_INFO("Handler call method: sender (%s), obj_path (%s), interface_name (%s), method_name (%s), type_string (%s)", sender, 
//                                                                                                                         object_path, 
//                                                                                                                         interface_name, 
//                                                                                                                         method_name, 
//                                                                                                        g_variant_get_type_string(paramenter));
//     if(g_strcmp0(method_name, "StopServices") == 0) {
//         GVariantIter *iter;
//         GVariant *dict;
//         const gchar *name; 

//         g_variant_get(paramenter, "(aa{sv})", &iter);
//         while( g_variant_iter_loop(iter, "&s", &name)) {
//             data->service_.erase(name); 
//         } 
//         // while ((dict = g_variant_iter_next_value(iter))) {
//         //     GVariant *service_erase = g_variant_lookup_value(dict, "&s", G_VARIANT_TYPE_STRING);
//         //     if (service_erase) {
//         //         name = g_variant_get_string(service_erase, NULL);
//         //         g_variant_unref(service_erase);
//         //         data->service_.erase(name); 
//         //     }
//         //     g_variant_unref(dict);
//         // }
//         g_dbus_method_invocation_return_value(invocation, NULL);
//         g_variant_iter_free(iter);
//     }                                                     
// }



    
