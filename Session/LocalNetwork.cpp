#include "LocalNetwork.hpp"
#include "Libraries/Log/LogPlus.hpp"
#include "Libraries/JsonConfiguration/JsonConfiguration.hpp"
#include "Generic.hpp"

#define COM_AUDIO_UPGRADE_BUS_NAME "com.audio.process"
#define CON_AUDIO_UPGRADE_CONTROLLER_INTERFACE "com.audio.process.Controller1"

GDBusNodeInfo *LocalNetwork::controller_introspection_data_ = nullptr;

const gchar *LocalNetwork::controller_introspection_xml_ =
    "<node>"
        "<interface name='com.audio.process.Controller1'>"
            "<method name='StopServices'>"
            "<arg type='as' direction='in' />"
        "</method>"
            "<method name='StartServices'>"
            "<arg type='as' direction='in' />"
        "</method>"
        "<method name='RestartServices'>"
            "<arg type='as' direction='in' />"
        "</method>"
            "<property name= 'GetListOfServices' type='aa{sv}' access='read'/>"
        "</interface>"
    "</node>";

LocalNetwork::LocalNetwork() : 
                                LBusNode::Server(LMainBus::GetInstance()), 
                                LBusNode::Client(LMainBus::GetInstance()) {
        
}

LocalNetwork::~LocalNetwork() {
        
}

int 
LocalNetwork::Start() {
    GError *error = nullptr;

    controller_introspection_data_ = g_dbus_node_info_new_for_xml(controller_introspection_xml_, &error);
    if (controller_introspection_data_ == nullptr) {
        LOG_ERRO("Error parse xml: %s\n", error->message);
        return -1;
    }

    bus_id_ = g_bus_own_name(G_BUS_TYPE_SYSTEM,
                                COM_AUDIO_UPGRADE_BUS_NAME,
                                G_BUS_NAME_OWNER_FLAGS_NONE,
                                GDBusAcquired,
                                GDBusNameAcquired,
                                GDBusNameLost,
                                this,
                                nullptr);
    if (bus_id_ == 0) {
        LOG_ERRO("Error acquiring name\n");
        return -1;
    }     
    
    return 0;
}

void 
LocalNetwork::Stop() {
    g_bus_unown_name(bus_id_);
}

void 
LocalNetwork::GDBusAcquired(GDBusConnection *connection,
                                    const gchar *name,
                                    gpointer user_data) {
    GError *error = nullptr;
    auto instance = (LocalNetwork *)user_data;

    guint controller_id;
    controller_id = g_dbus_connection_register_object(connection,
                                                        "/",
                                                        instance->controller_introspection_data_->interfaces[0],
                                                        &instance->controller_interface_vtable_,
                                                        instance,
                                                        nullptr,
                                                        &error); 
    if (controller_id == 0) {
        LOG_ERRO("Error registering interface: %s\n", error->message);
        return;
    }

    LOG_INFO("Acquire a instance of DBus daemon");
}

void 
LocalNetwork::GDBusNameAcquired(GDBusConnection *connection,
                                        const gchar *name,
                                        gpointer user_data) {
    auto instance = (LocalNetwork *)user_data;

    instance->connection_ = connection;
    LOG_INFO("Acquire %s well-know name.", name);
}

void 
LocalNetwork::GDBusNameLost(GDBusConnection *connection,
                                    const gchar *name,
                                    gpointer user_data) {
    auto instance = (LocalNetwork *)user_data;

    instance->connection_ = nullptr;
    LOG_INFO("Lost %s well-know name.", name);
}

GDBusInterfaceVTable LocalNetwork::controller_interface_vtable_ = {
    .method_call = HandleControllerMethods,
    .get_property = HandleControllerGetProperties,
    .set_property = HandleControllerSetProperties
};


void 
LocalNetwork::HandleControllerMethods(GDBusConnection *connection,
                                        const gchar *sender,
                                        const gchar *object_path,
                                        const gchar *interface_name,
                                        const gchar *method_name,
                                        GVariant *paramenters,
                                        GDBusMethodInvocation *invocation,
                                        gpointer user_data) {
    auto self = (LocalNetwork *)user_data;

    LOG_INFO("Handler call method: sender (%s), obj_path (%s), interface_name (%s), method_name (%s), type_string (%s)", sender, 
                                                                                                                        object_path, 
                                                                                                                        interface_name, 
                                                                                                                        method_name, 
                                                                                                                        g_variant_get_type_string(paramenters));

    int ret;
    GVariant *res_stop = nullptr, *res_start = nullptr;
    LBus::Transaction response;
    GVariantIter *iter;
    const gchar *name;
    g_variant_get(paramenters, "(as)", &iter); 
    
    if (g_strcmp0(method_name, "StopServices") == 0) {
        while (g_variant_iter_next(iter, "s", &name)) { 
            res_stop = g_variant_new("(s)", name); 
            ret = self->LBusNode::Client::Publish(KEEPALIVE_MODULE, "stop-service", LBus::SET, 50, {&res_stop, sizeof(&res_stop),[] (void * buffer) {
                                                                                                    auto res_stop = (GVariant **)buffer;
                                                                                                    g_variant_unref(*res_stop); 
                                                                                                    }}, 
                                                                                                    &response);
            if (ret != 0) {
                if (ret == 1) 
                    LOG_DBUG("Timeout");
                else 
                    LOG_ERRO("Dbus server error");
                goto out; 
            }
            auto builder = (char **)LBus::GetTransaction(&response); 
            LOG_INFO("Stop service %s %s", name, *builder); 
        }
    } 
    else if(g_strcmp0(method_name, "StartServices") == 0) {
        while (g_variant_iter_next(iter, "s", &name)) { 
            res_start = g_variant_new("(s)", name); 
            self->LBusNode::Client::Publish(KEEPALIVE_MODULE, "start-service", LBus::SET, 50, {&res_start, sizeof(&res_start),[] (void * buffer) {
                                                                                                auto res_start = (GVariant **)buffer;
                                                                                                g_variant_unref(*res_start); 
                                                                                                }}, 
                                                                                                &response); 
            if (ret != 0) {
                if (ret == 1) 
                    LOG_DBUG("Timeout");
                else 
                    LOG_ERRO("Dbus server error");
                goto out; 
            }
            auto builder = (char **)LBus::GetTransaction(&response); 
            LOG_INFO("Start service %s %s", name, *builder); 
        }
    } 
    else if(g_strcmp0(method_name, "RestartServices") == 0) {
        while (g_variant_iter_next(iter, "s", &name)) { 
            res_stop = g_variant_new("(s)", name); 
            self->LBusNode::Client::Publish(KEEPALIVE_MODULE, "stop-service", LBus::SET, 50, {&res_stop, sizeof(&res_stop),[] (void * buffer) {
                                                                                                auto res_stop = (GVariant **)buffer;
                                                                                                g_variant_unref(*res_stop); 
                                                                                                }}, 
                                                                                                &response); 
            if (ret != 0) {
                if (ret == 1) 
                    LOG_DBUG("Timeout");
                else 
                    LOG_ERRO("Dbus server error");
                goto out; 
            }
            auto builder = (char **)LBus::GetTransaction(&response); 
            LOG_INFO("Stop service %s %s", name, *builder);

            res_start = g_variant_new("(s)", name); 
            self->LBusNode::Client::Publish(KEEPALIVE_MODULE, "start-service", LBus::SET, 50, {&res_start, sizeof(&res_start),[] (void * buffer) {
                                                                                                auto res_start = (GVariant **)buffer;
                                                                                                g_variant_unref(*res_start); 
                                                                                                }}, 
                                                                                                &response); 
            if (ret != 0) {
                if (ret == 1) 
                    LOG_DBUG("Timeout");
                else 
                    LOG_ERRO("Dbus server error");
                goto out; 
            }
            builder = (char **)LBus::GetTransaction(&response); 
            LOG_INFO("Start service %s %s", name, *builder); 
        }
    }
    else {
        LOG_DBUG("This method had not yet handled"); 
    }
out: 
    g_dbus_method_invocation_return_value(invocation, nullptr); 
    g_variant_iter_free(iter);
}
    
gboolean
LocalNetwork::HandleControllerSetProperties(GDBusConnection *connection,
                                            const gchar *sender,
                                            const gchar *object_path,
                                            const gchar *interface_name,
                                            const gchar *property_name,
                                            GVariant *value,
                                            GError **error,
                                            gpointer user_data) {
    auto instance = (LocalNetwork *)user_data;

    LOG_INFO("%s, %s, %s, %s, %s", sender, object_path, interface_name, property_name, g_variant_get_type_string(value));
    LOG_INFO("Handler set property: sender (%s), obj_path (%s), interface_name (%s), property_name (%s), type_string (%s)", sender, 
                                                                                                                            object_path, 
                                                                                                                            interface_name, 
                                                                                                                            property_name, 
                                                                                                                            g_variant_get_type_string(value));
   
    
     
    return TRUE;
}  
     

GVariant *
LocalNetwork::HandleControllerGetProperties(GDBusConnection *connection,
                                                    const gchar *sender,
                                                    const gchar *object_path,
                                                    const gchar *interface_name,
                                                    const gchar *property_name,
                                                    GError **error,
                                                    gpointer user_data) {
    GVariant* ret = NULL;
    auto self = (LocalNetwork *) user_data; 
    LOG_INFO("Handler get the property: sender (%s), obj_path (%s), interface_name (%s), property_name (%s)", sender, object_path, interface_name, property_name);
   
    if(g_strcmp0(property_name, "GetListOfServices") == 0) {
         LBus::Transaction response; 
        auto res = g_variant_new_string("GetListOfServices"); 
        self->LBusNode::Client::Publish(KEEPALIVE_MODULE, "get-list-of-services", LBus::GET, 50, {&res, sizeof(&res),[] (void * buffer) {
            auto res = (GVariant **)buffer;
            g_variant_unref(*res); 
            }}, &response); 
        auto builder = (GVariant **)LBus::GetTransaction(&response); 
        ret = g_variant_new("(aa{sv})", *builder); 
    } 
    
    return ret; 
}
