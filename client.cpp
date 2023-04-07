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
    std::cout << "Usage: client [OPTIONS] [service1] [serivce2] [...]" << std::endl;
    std::cout << std::endl;
    std::cout << "\t-x, --stop \t to stop services by name" << std::endl;
    std::cout << "\t-s, --start \t to start services by name" << std::endl;
    std::cout << "\t-r, --restart \t to restart services by name" << std::endl;
    std::cout << "\t-g, --getlist \t to get list of services" << std::endl;
}


int main(int argc, char *argv[]) {
    int opt;
    bool is_option = false;

    std::string name_services, properties, method;
    std::vector<char *> name; 
    struct option long_options[] = {
            {"start",     required_argument, nullptr, 's'},
            {"stop",      required_argument, nullptr, 'x'},
            {"restart",   required_argument, nullptr, 'r'},
            {"getlist",   no_argument,       nullptr, 'g'}, 
            {"help",      no_argument,       nullptr, 'h'},
            {nullptr,     0,                 nullptr,  0 }
    };

    while ((opt = getopt_long(argc, argv, "hx:s:r:g", long_options, nullptr)) != -1) {
        is_option = true;
        switch (opt) {
            case 'x' :
                method = "StopServices"; 
                optind--;
                for (; optind < argc && *argv[optind] != '-'; optind++) {
                    name.push_back(argv[optind]);
                }
                break;
            case 'r' :
                method = "RestartServices"; 
                optind--;
                for (; optind < argc && *argv[optind] != '-'; optind++) {
                    name.push_back(argv[optind]);
                }
                break;
            case 'g' : 
                properties.assign(optarg);
                break;
            case 's' : 
                method = "StartServices"; 
                optind--; 
                for (; optind < argc && *argv[optind] != '-'; optind++) {
                    name.push_back(argv[optind]);
                }
                break; 
            case 'h':
                PrintHelp();
            default:
                fprintf(stderr, "client missing operand\n");
                fprintf(stderr, "Try 'client --help' for more information\n");
                exit(0);
        }
    }

    if (!is_option) {
        fprintf(stderr, "client missing operand\n");
        fprintf(stderr, "Try 'client --help' for more information\n");
        exit(0);
    }

    GDBusProxy *proxy_;
    GVariant *res;
    proxy_ = GDBusProxyConnect(COM_AUDIO_PROCESS_BUS_NAME, COM_AUDIO_PROCESS_OBJECT_PATH, COME_AUDIO_PROCESS_CONTROLLER_INTERFACE);
    if (proxy_ == nullptr) {
        printf("Failed to create proxy");
    }
    GVariant *request;
    GVariantBuilder builder;
    GVariant *value;
    g_variant_builder_init(&builder, G_VARIANT_TYPE("as"));
    while(!name.empty()) {
        g_variant_builder_add(&builder, "s", name.back());
        name.pop_back(); 
    }
    request = g_variant_new("(as)", &builder);
    g_variant_builder_clear(&builder);
    res = GDBusProxyCallMethod(proxy_, method, request);
    if (res == nullptr)
        printf("%s FAILED\n", method.c_str());
    else 
        printf("%s SUCCESSFULLY\n", method.c_str()); 
   
    return 0;
}

