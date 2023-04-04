#ifndef SESSION_LOCALNETWORK_H_
#define SESSION_LOCALNETWORK_H_

#include <gio/gio.h>

#include "Generic.hpp"
#include "Libraries/LBus/LBusNode.hpp"
#include "Session/LMainBus/LMainBus.hpp"

class LocalNetwork : public LBusNode::Server, public LBusNode::Client {
public:
    LocalNetwork();
    ~LocalNetwork();

    int Start();
    void Stop();
    
private:
    guint bus_id_;
    GDBusConnection *connection_; 

    static GDBusNodeInfo *controller_introspection_data_;
    static const gchar *controller_introspection_xml_;
    static GDBusInterfaceVTable controller_interface_vtable_;

    static void GDBusAcquired(GDBusConnection *connection,
                                const gchar *name,
                                gpointer user_data);
    static void GDBusNameAcquired(GDBusConnection *connection,
                                const gchar *name,
                                gpointer user_data);
    static void GDBusNameLost(GDBusConnection *connection,
                                const gchar *name,
                                gpointer user_data);
    static void HandleControllerMethods(GDBusConnection *connection,
                                        const gchar *sender,
                                        const gchar *object_path,
                                        const gchar *interface_name,
                                        const gchar *method_name,
                                        GVariant *paramenter,
                                        GDBusMethodInvocation *invocation,
                                        gpointer user_data);
                                
    static gboolean HandleControllerSetProperties(GDBusConnection *connection,
                                                const gchar *sender,
                                                const gchar *object_path,
                                                const gchar *interface_name,
                                                const gchar *property_name,
                                                GVariant *value,
                                                GError **error,
                                                gpointer user_data);

    static GVariant *HandleControllerGetProperties(GDBusConnection *connection,
                                                    const gchar *sender,
                                                    const gchar *object_path,
                                                    const gchar *interface_name,
                                                    const gchar *proterty_name,
                                                    GError **error,
                                                    gpointer user_data);
};

#endif // 