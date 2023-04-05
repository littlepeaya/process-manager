#include <unistd.h>
#include <getopt.h>
#include <glib.h>

#include <iostream>

#include "Session/Session.hpp"
#include "Controllers/Controllers.hpp"
#include "Libraries/JsonConfiguration/JsonConfiguration.hpp"
#include <unistd.h>
#include <getopt.h>
#include <glib.h>

#include <iostream>

#include "Session/Session.hpp"
#include "Controllers/Controllers.hpp"
#include "Libraries/JsonConfiguration/JsonConfiguration.hpp"
#include "Libraries/Log/Log.hpp"
#include "Generic.hpp"


#define WLAN_IFACE_ADDRESS_PATH     "/sys/class/net/wlan0/address" 

#define WLAN_IFACE_ADDRESS_LEN      17 

void PrintHelp() {
    std::cout << "Usage: audio-manager [OPTIONS]" << std::endl;
    std::cout << std::endl;
    std::cout << "\t-c CONFIG\t specify the config file" << std::endl;
    std::cout << "\t-h\t display the help" << std::endl;
}


int UpdateMainAddress() {
    int fd, len;
    char macaddr[64] = {0};

    fd = open(WLAN_IFACE_ADDRESS_PATH, O_RDONLY);
    if (fd < 0) {
        LOG_ERRO("Could not open the '%s'.", WLAN_IFACE_ADDRESS_PATH);
        return -1;
    }

    len = read(fd, macaddr, WLAN_IFACE_ADDRESS_LEN);
    close(fd);
    if (len != WLAN_IFACE_ADDRESS_LEN) {
        LOG_ERRO("Error reading the '%s'.", WLAN_IFACE_ADDRESS_PATH);
        return -1;
    }

    auto root = JsonConfiguration::GetInstance()->Read();
    root["macaddr"] = std::string(macaddr, WLAN_IFACE_ADDRESS_LEN);
    JsonConfiguration::GetInstance()->Write(root);
    return 0;
}

int main(int argc, char *argv[])
{
    int opt;
    bool is_option = false;
    int retval;

    std::string log_path, config_path, name_service, properties;
    struct option long_options[] = {
            {"config", required_argument, nullptr, 'c'},
            {"help",   no_argument,       nullptr, 'h'},
            {"stopservice", required_argument, nullptr, 's'},
            {nullptr, 0,                  nullptr, 0}
    };

    while ((opt = getopt_long(argc, argv, "hc:s:", long_options, nullptr)) != -1) {
        is_option = true;
        switch (opt) {
            case 'c':
                config_path.assign(optarg);
                break;
            case 's' : 
                properties.assign(optarg); 
                break; 
            case 'h':
                PrintHelp();
                break;
            default:
                fprintf(stderr, "audio-manager missing operand\n");
                fprintf(stderr, "Try 'audio-manager --help' for more information\n");
                exit(0);
        }
    }

    if (!is_option) {
        fprintf(stderr, "audio-manager missing operand\n");
        fprintf(stderr, "Try 'audio-manager --help' for more information\n");
        exit(0);
    }

    GDBusProxy *proxy_;
    GVariant *res;
    proxy_ = GDBusProxyConnect(COM_AUDIO_PROCESS_BUS_NAME, COM_AUDIO_PROCESS_OBJECT_PATH, COME_AUDIO_PROCESS_CONTROLLER_INTERFACE);
    if (proxy_ == nullptr) {
        LOG_ERRO("Failed to create proxy");
    }
    GVariant *request;
    const gchar *name[] = {"audio-manager", "io-manager"};
    GVariantBuilder builder;
    GVariant *value;
    g_variant_builder_init(&builder, G_VARIANT_TYPE("as"));
    g_variant_builder_add(&builder, "s", "io-manager");
    g_variant_builder_add(&builder, "s", "audio-manager");
    request = g_variant_new("(as)", &builder);
    printf("%s\n", g_variant_print(request, true));
    g_variant_builder_clear(&builder);
    res = GDBusProxyCallMethod(proxy_, "GetListOfService", request);
    if (res == nullptr)
        printf("error\n");

    //main loop 
    GMainLoop *loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(loop);
   
    return 0;
}

