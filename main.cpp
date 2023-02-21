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

int main(int argc, char *argv[]) {
    int opt;
    bool is_option = false;
    int retval;
    
    std::string log_path, config_path;
    struct option long_options[] = {
            {"config", required_argument, nullptr, 'c'},
            {"help",   no_argument,       nullptr, 'h'},
            {nullptr, 0,                  nullptr, 0}
    };

    while ((opt = getopt_long(argc, argv, "hc:", long_options, nullptr)) != -1) {
        is_option = true;
        switch (opt) {
            case 'c':
                config_path.assign(optarg);
                break;
            case 'h':
                PrintHelp();
                exit(0);
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

    JsonConfiguration *json_configuration = JsonConfiguration::GetInstance();
    for (int i = 0; json_configuration->Open(config_path) < 0; ++i) {
        fprintf(stderr, "Couldn't open the '%s'\n", config_path.c_str());

        switch (i) {
            case 0:
                fprintf(stdout, "Try repair '%s' from '%s'", config_path.c_str(), (config_path + ".copied").c_str());
                retval = Copy((config_path + ".copied").c_str(), config_path.c_str());
                if (retval < 0)
                    fprintf(stderr, "Couldn't copy from '%s' to '%s'\n", (config_path + ".copied").c_str(), config_path.c_str());
                else 
                    close(retval);

                break;
            
            case 1:
                fprintf(stdout, "Try repair '%s' from '%s'", config_path.c_str(), (config_path + ".orig").c_str());
                retval = Copy((config_path + ".orig").c_str(), config_path.c_str());
                if (retval < 0)
                    fprintf(stderr, "Couldn't copy from '%s' to '%s'\n", (config_path + ".orig").c_str(), config_path.c_str());
                else 
                    close(retval);
                    
                break;
            
            default:
                fprintf(stderr, "Couldn't repair '%s'", config_path.c_str());
                exit(1);
        }
    }

    if (UpdateMainAddress() < 0) {
        fprintf(stderr, "Failed to update main address\n");
        exit(1);
    }
    
    auto root = json_configuration->Read();
    Log::Level level = (Log::Level)root["log"]["level"].asInt();
    Log::Create(root["log"]["path"].asString(), true, true, level, level);
    LOG_INFO("The log is contained in the '%s'.", root["log"]["path"].asCString());

    // Session session; 
    // if (session.Start() < 0) {
    //     fprintf(stderr, "Error starting session\n");
    //     exit(1);
    // }

    Controllers controller;
    if (controller.Start() < 0) {
        fprintf(stderr, "Error starting controller\n");
        exit(1);
    }

    controller.Run(); 

    GMainLoop *loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(loop);

    return 0;
}