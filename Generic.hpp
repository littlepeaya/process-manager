#ifndef GENERIC_HPP
#define GENERIC_HPP

#include <gio/gio.h>
#include <json/json.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <curl/curl.h>
#include <fcntl.h>
#include <time.h>
#include <string>
#include <functional>
#include <sys/time.h>

#include "Libraries/Log/LogPlus.hpp"

#define WLAN_IFACE_ADDRESS_PATH     "/sys/class/net/wlan0/address" 

#define WLAN_IFACE_ADDRESS_LEN 17

// TODO: owntone
#define COM_AUDIO_OWNTONE_BUS_NAME                  "com.audio.owntone"

#define COM_AUDIO_OWNTONE_OBJECT_PATH               "/"

#define COM_AUDIO_OWNTONE_PLAYER_INTERFACE          "com.audio.owntone.Player1"

#define COM_AUDIO_OWNTONE_LOCAL_INTERFACE           "com.audio.owntone.Local1"

#define COM_AUDIO_OWNTONE_PLAYLIST_INTERFACE        "com.audio.owntone.Playlist1"

#define COM_AUDIO_OWNTONE_OUTPUT_INTERFACE          "com.audio.owntone.Output1"

#define COM_AUDIO_OWNTONE_QUEUE_INTERFACE           "com.audio.owntone.Queue1"

#define COM_AUDIO_OWNTONE_CLOUD_INTERFACE           "com.audio.owntone.Cloud1"


// TODO: bluetooth
#define COM_AUDIO_BLUETOOTH_BUS_NAME                "com.audio.bluetooth"

#define COM_AUDIO_BLUETOOTH_OBJECT_PATH             "/"

#define COM_AUDIO_BLUETOOTH_PLAYER_INTERFACE        "com.audio.bluetooth.Player1"

#define COM_AUDIO_BLUETOOTH_CONTROLLER_INTERFACE    "com.audio.bluetooth.Controller1"

// TODO: airplay
#define COM_AUDIO_AIRPLAY_BUS_NAME                  "com.audio.airplay"

#define COM_AUDIO_AIRPLAY_OBJECT_PATH               "/"

#define COM_AUDIO_AIRPLAY_PLAYER_INTERFACE          "com.audio.airplay.Player1"

#define COM_AUDIO_AIRPLAY_CONTROLLER_INTERFACE      "com.audio.airplay.Controller1"

// TODO: io
#define COM_AUDIO_IO_BUS_NAME                       "com.audio.io"

#define COM_AUDIO_IO_OBJECT_PATH                    "/"

#define COM_AUDIO_IO_CONTROLLER_INTERFACE           "com.audio.io.Controller1"

// TODO: OTA
#define COM_AUDIO_OTA_BUS_NAME                      "com.audio.upgrade"

#define COM_AUDIO_OTA_OBJECT_PATH                   "/"

#define COM_AUDIO_OTA_CONTROLLER_INTERFACE          "com.audio.upgrade.Controller1"

//TODO: PROCESS-MANAGER 
#define COM_AUDIO_PROCESS_BUS_NAME                  "com.audio.process"

#define COM_AUDIO_PROCESS_OBJECT_PATH               "/"

#define COME_AUDIO_PROCESS_CONTROLLER_INTERFACE     "com.audio.process.Controller1"

// TODO: modules
#define PLAYERS_MODULE                              "players"

#define AIRPLAY_MODULE                              "airplay"

#define BLUETOOTH_MODULE                            "bluetooth"

#define AUTOMATION_MODULE                           "automation"

#define AUTOMATION_MAIN_PLAYER_MODULE               "automation-main-player"

#define NOTIFIER_MODULE                             "notifier"

#define DEVICE_MODULE                               "device"

#define NETWORK_MODULE                              "network"

#define LOCAL_MODULE                                "local"

#define CLOUD_MODULE                                "cloud"

#define OUTPUT_MODULE                               "output"

#define QUEUE_MODULE                                "queue"

#define PLAYLIST_MODULE                             "playlist"

#define OTA_MODULE                                  "ota"

#define EQUALIZER_MODULE                            "equalizer"

#define IO_MODULE                                   "io"

#define PROCESS_MODULE                              "process"

typedef enum {
    OWNTONE = 0,
    BLUETOOTH,
    AIRPLAY
} KindOfSpeaker;

static std::string speaker_to_string[] = {
    "owntone",
    "bluetooth",
    "airplay"
};

typedef enum {
    PLAYER = 0,
    OUTPUTS ,
    QUEUE,
    PLAYLIST,
    LOCAL,
    DOWNLOAD
} NotifyingEvents;

static const std::string notifying_events_to_string[] = {
    "player",
    "outputs",
    "queue",
    "playlist",
    "local",
    "download"
};

typedef struct {
    uint32_t id, progress, length;
    std::string title, artist, album;
} PlayingTrack;

typedef struct {
    KindOfSpeaker kind_of_speaker;
    std::string state, repeat;
    bool shuffle;
    uint8_t volume;
    PlayingTrack playing_track;
} PlayerStatus;

inline int
GDBusProxySetProperty(GDBusProxy *proxy, std::string property, GVariant *value) {
    GError *error = nullptr;

    GVariant *response = g_dbus_proxy_call_sync(proxy,
                                                "org.freedesktop.DBus.Properties.Set",
                                                g_variant_new("(ssv)",
                                                              g_dbus_proxy_get_interface_name(proxy),
                                                              property.c_str(),
                                                              value),
                                                G_DBUS_CALL_FLAGS_NONE,
                                                -1,
                                                nullptr,
                                                &error);
    if (response == nullptr) {
        error != nullptr ? g_error_free(error) : (void)nullptr;
        return -1;
    }

    g_variant_unref(response);
    return 0;
}

inline GVariant *
GDBusProxyGetProperty(GDBusProxy *proxy, std::string property) {
    GError *error = nullptr;

    GVariant *response = g_dbus_proxy_call_sync(proxy,
                                                "org.freedesktop.DBus.Properties.Get",
                                                g_variant_new("(ss)",
                                                              g_dbus_proxy_get_interface_name(proxy),
                                                              property.c_str()),
                                                G_DBUS_CALL_FLAGS_NONE,
                                                -1,
                                                nullptr,
                                                &error);
    if (response == nullptr) {
        error != nullptr ? g_error_free(error) : (void)nullptr;
        return nullptr;
    }

    GVariant *utemp;
    g_variant_get(response, "(v)", &utemp);
    g_variant_unref(response);
    return utemp;
}

inline GDBusProxy *
GDBusProxyConnect(std::string bus_name, std::string object_path, std::string interface) {
    GError *error = nullptr;
    
    GDBusProxy *proxy = g_dbus_proxy_new_for_bus_sync(G_BUS_TYPE_SYSTEM,
                                                        G_DBUS_PROXY_FLAGS_NONE,
                                                        nullptr,
                                                        bus_name.c_str(),
                                                        object_path.c_str(),
                                                        interface.c_str(),
                                                        nullptr,
                                                        &error);
    if (proxy == nullptr) {
        error != nullptr ? g_error_free(error) : (void)nullptr;
        return nullptr;
    }

    return proxy;
}

inline void
GDBusProxyClose(GDBusProxy *proxy) {
    if (proxy != nullptr)
         g_object_unref(proxy);
}

inline GVariant *
GDBusProxyCallMethod(GDBusProxy *proxy, std::string method, GVariant *request) {
    GError *error = nullptr;

    GVariant *response = g_dbus_proxy_call_sync(proxy,
                                                method.c_str(),
                                                request,
                                                G_DBUS_CALL_FLAGS_NONE,
                                                -1,
                                                nullptr,
                                                &error);
    if (response == nullptr) {
        error != nullptr ? g_error_free(error) : (void)nullptr;
        return nullptr;
    }

    return response;
}

inline GVariant *
JsonToGVariant(const Json::Value &data) {
    GVariantBuilder builder;
    g_variant_builder_init(&builder,  G_VARIANT_TYPE_ARRAY);

    try {
        for (auto it = data.begin(); it != data.end(); ++it) {
            GVariant *value = nullptr;

            switch (it->type()) {
                case Json::booleanValue:
                    value = g_variant_new_boolean(it->asBool());
                    break;
                
                case Json::intValue:
                    value = it->isInt() == true ? g_variant_new_int32(it->asInt()) : g_variant_new_int64(it->asInt64());
                    break;

                case Json::uintValue:
                    value = it->isUInt() == true ? g_variant_new_uint32(it->asUInt()) : g_variant_new_uint64(it->asUInt64());
                    break;

                case Json::realValue:
                    value = g_variant_new_double(it->asDouble());
                    break;

                case Json::stringValue:
                    value = g_variant_new_string(it->asString().c_str());
                    break;

                case Json::objectValue:
                    value = JsonToGVariant(*it);
                    break;

                case Json::arrayValue:
                {
                    GVariantBuilder subbuilder;
                    g_variant_builder_init(&subbuilder,  G_VARIANT_TYPE_ARRAY);
                    for (auto subit = it->begin(); subit != it->end(); ++subit) {
                        switch (subit->type()) {
                            case Json::booleanValue:
                                g_variant_builder_add(&subbuilder, "b", subit->asBool());
                                break;
                        
                            case Json::intValue:
                                it->isInt() == true ? g_variant_builder_add(&subbuilder, "i", subit->asInt()) : g_variant_builder_add(&subbuilder, "x", subit->asInt64());
                                break;

                            case Json::uintValue:
                                it->isUInt() == true ? g_variant_builder_add(&subbuilder, "u", subit->asUInt()) : g_variant_builder_add(&subbuilder, "t", subit->asUInt64());
                                break;

                            case Json::realValue:
                                g_variant_builder_add(&subbuilder, "d", subit->asDouble());
                                break;

                            case Json::stringValue:
                                g_variant_builder_add(&subbuilder, "s", subit->asString().c_str());
                                break; 

                            case Json::objectValue:
                                g_variant_builder_add_value(&subbuilder, JsonToGVariant(*subit));
                                break;
                        }
                    }

                    value = g_variant_builder_end(&subbuilder);
                    break;
                }

                default:
                    value = g_variant_new_string("");
                    break;                 
            }

            g_variant_builder_add(&builder, "{sv}", it.name().c_str(), value);
        }
    } catch (const Json::Exception& ex) {
        LOG_ERRO("%s.", ex.what());
        g_variant_builder_clear(&builder);
        return nullptr;
    }

    return g_variant_builder_end(&builder);
}


inline int
GVariantToJson(GVariant *builder, Json::Value& data) {
    GVariant *value, *unpack;
    GVariantIter iter;
    char *key;

    if (g_variant_is_of_type(builder, G_VARIANT_TYPE_TUPLE) == TRUE) {
        g_variant_get(builder, "(@*)", &unpack);
        g_variant_unref(builder);
        builder = unpack;
    }

    if (g_variant_is_of_type(builder, G_VARIANT_TYPE_VARDICT) == TRUE) {
        g_variant_iter_init(&iter, builder);
        while (g_variant_iter_next (&iter, "{sv}", &key, &value)) {
            if (g_variant_is_of_type(value, G_VARIANT_TYPE_BOOLEAN) == TRUE)
                data[key] = g_variant_get_boolean(value) == 0 ? false : true;
            else if (g_variant_is_of_type(value, G_VARIANT_TYPE_BYTE) == TRUE)
                data[key] = g_variant_get_byte(value);
            else if (g_variant_is_of_type(value, G_VARIANT_TYPE_INT16) == TRUE)
                data[key] = g_variant_get_int16(value);
            else if (g_variant_is_of_type(value, G_VARIANT_TYPE_UINT16) == TRUE)
                data[key] = g_variant_get_uint16(value);
            else if (g_variant_is_of_type(value, G_VARIANT_TYPE_INT32) == TRUE)
                data[key] = g_variant_get_int32(value);
            else if (g_variant_is_of_type(value, G_VARIANT_TYPE_UINT32) == TRUE)
                data[key] = g_variant_get_uint32(value);
            else if (g_variant_is_of_type(value, G_VARIANT_TYPE_INT64) == TRUE)
                data[key] = g_variant_get_int64(value);
            else if (g_variant_is_of_type(value, G_VARIANT_TYPE_UINT64) == TRUE)
                data[key] = g_variant_get_uint64(value);
            else if (g_variant_is_of_type(value, G_VARIANT_TYPE_DOUBLE) == TRUE)
                data[key] = g_variant_get_double(value);
            else if (g_variant_is_of_type(value, G_VARIANT_TYPE_STRING) == TRUE)
                data[key] = g_variant_get_string(value, nullptr);
            else if (g_variant_is_of_type(value, G_VARIANT_TYPE_OBJECT_PATH) == TRUE)
                data[key] = g_variant_get_string(value, nullptr);
            else if (g_variant_is_of_type(value, G_VARIANT_TYPE_SIGNATURE) == TRUE)
                data[key] = g_variant_get_string(value, nullptr);
            else if (g_variant_is_of_type(value, G_VARIANT_TYPE_VARDICT) == TRUE)
                GVariantToJson(g_variant_ref(value), data[key]);
            else if (g_variant_is_of_type(value, G_VARIANT_TYPE_ARRAY) == TRUE) {
                GVariant *subchild;
                GVariantIter subiter;
                uint32_t position = 0;
                data[key] = Json::arrayValue;

                g_variant_iter_init(&subiter, value);
                while ((subchild = g_variant_iter_next_value(&subiter))) {
                    if (g_variant_is_of_type(subchild, G_VARIANT_TYPE_BOOLEAN) == TRUE)
                        data[key][position] = g_variant_get_boolean(subchild);
                    else if (g_variant_is_of_type(subchild, G_VARIANT_TYPE_BYTE) == TRUE)
                        data[key][position] = g_variant_get_byte(subchild);
                    else if (g_variant_is_of_type(subchild, G_VARIANT_TYPE_INT16) == TRUE)
                        data[key][position] = g_variant_get_int16(subchild);
                    else if (g_variant_is_of_type(subchild, G_VARIANT_TYPE_UINT16) == TRUE)
                        data[key][position] = g_variant_get_uint16(subchild);
                    else if (g_variant_is_of_type(subchild, G_VARIANT_TYPE_INT32) == TRUE)
                        data[key][position] = g_variant_get_int32(subchild);
                    else if (g_variant_is_of_type(subchild, G_VARIANT_TYPE_UINT32) == TRUE)
                        data[key][position] = g_variant_get_uint32(subchild);
                    else if (g_variant_is_of_type(subchild, G_VARIANT_TYPE_INT64) == TRUE)
                        data[key][position] = g_variant_get_int64(subchild);
                    else if (g_variant_is_of_type(subchild, G_VARIANT_TYPE_UINT64) == TRUE)
                        data[key][position] = g_variant_get_uint64(subchild);
                    else if (g_variant_is_of_type(subchild, G_VARIANT_TYPE_DOUBLE) == TRUE)
                        data[key][position] = g_variant_get_double(subchild);
                    else if (g_variant_is_of_type(subchild, G_VARIANT_TYPE_STRING) == TRUE)
                        data[key][position] = g_variant_get_string(subchild, nullptr);
                    else if (g_variant_is_of_type(subchild, G_VARIANT_TYPE_OBJECT_PATH) == TRUE)
                        data[key][position] = g_variant_get_string(subchild, nullptr);
                    else if (g_variant_is_of_type(subchild, G_VARIANT_TYPE_SIGNATURE) == TRUE)
                        data[key][position] = g_variant_get_string(subchild, nullptr);
                    else if (g_variant_is_of_type(subchild, G_VARIANT_TYPE_VARDICT) == TRUE)
                        GVariantToJson(g_variant_ref(subchild), data[key][position]);
                    
                    g_variant_unref(subchild);
                    position += 1;
                }

            }

            g_variant_unref(value);
            g_free(key);
        }
    }

    g_variant_unref(builder);
    return 0;
}

inline std::string
GetIp(std::string network_interface)
{
    struct ifreq ifr;
    struct timeval tv; 
    time_t t; 
    int sock;
    int retval;

    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, network_interface.c_str(), IFNAMSIZ - 1);

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        LOG_ERRO("Error creating socket, err: %s\n", strerror(errno));
        return std::string("");
    }

    retval = ioctl(sock, SIOCGIFADDR, &ifr);
    if (retval < 0) {
       
          if(gettimeofday(&tv, NULL) == -1)
            printf("error\n");
        LOG_INFO("time return %ld secs %ld microsecs\n", (long) tv.tv_sec, (long) tv.tv_usec);
        
        LOG_ERRO("Error getting ip address, err: %s\n", strerror(errno));
        goto out;
    }

    LOG_INFO("IP address is %s - %s\n", network_interface.c_str(), inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));

    close(sock);
    return inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);

out:
    close(sock);
    return std::string("");
}

inline std::string
GetNetmask(std::string network_interface)
{
    struct timeval tv; 
    struct ifreq ifr;
    int sock;
    int retval;

    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, network_interface.c_str(), IFNAMSIZ - 1);

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        LOG_ERRO("Error creating socket, err: %s\n", strerror(errno));
        return std::string("");
    }

    retval = ioctl(sock, SIOCGIFNETMASK, &ifr);
    if (retval < 0) {
        if(gettimeofday(&tv, NULL) == -1)
            printf("error\n");
        LOG_INFO("time return %ld secs %ld microsecs\n", (long) tv.tv_sec, (long) tv.tv_usec); 
        LOG_ERRO("Error getting ip netmask, err: %s\n", strerror(errno));
        goto out;
    }

    LOG_INFO("Netmask is %s - %s\n", network_interface.c_str(), inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
    close(sock);
    return inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);

out:
    close(sock);
    return std::string("");
}

inline bool 
Execute(const String& command) {
    // LOG_INFO("Execute: %s", command.data());
    int result = system(command.c_str());
    if (WIFEXITED(result) && WEXITSTATUS(result) == EXIT_SUCCESS)
        return true;

    LOG_ERRO("Could not execute command: %d - %d - %s", result, errno, strerror(errno));
    return false;
}

inline bool
CheckNetwork() {
    CURL *curl;
    CURLcode res;
    bool retval = false;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://google.com");
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            goto out;
    }

    retval = true;

out:
    curl_easy_cleanup(curl);
    return retval;
}

inline int
Copy(const char *oldpath, const char *newpath) {
    int oldfd, newfd;

    oldfd = open(oldpath, O_RDONLY);
    newfd = open(newpath, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (oldfd < 0 || newfd < 0) {
        LOG_ERRO("Could not open the '%s' & '%s' files, error: %s\n", oldpath, newpath, strerror(errno));
        oldfd >= 0 ? close(oldfd) : (int)0;
        newfd >= 0 ? close(newfd) : (int)0;
        return -1;
    }

    char oldpath_data[4096] = {0};
    size_t oldpath_len, newpath_len;
    while (true) {
        oldpath_len = read(oldfd, oldpath_data, sizeof(oldpath_data));
        if (oldpath_len < 0) {
            LOG_ERRO("Error reading the '%s' file, error: %s\n", oldpath, strerror(errno));
            goto out;
        } 

        if (oldpath_len == 0) {
            sync();
            break;
        }

        newpath_len = write(newfd, oldpath_data, oldpath_len);
        if (newpath_len != oldpath_len) {
            LOG_ERRO("Error copying '%s' to '%s'\n", oldpath, newpath);
            goto out;
        }
    }

    LOG_INFO("Success duplicating '%s' to '%s'\n", oldpath, newpath);
    close(oldfd);
    return newfd;

out:
    close(oldfd);
    close(newfd);
    return -1;
}

inline std::string
ExecuteCommand(const char *cmd) {
    try {
        std::array<char, 128> buffer{};
        std::string result;
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
        if (!pipe) {
            THROW_EXCEPTION();  
        }
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }
        return result;
    }catch (std::exception& ex) {
        LOG_ERRO(ex.what());
        return String();
    }
}

inline time_t
GetLocalTimestamp() {
    time_t utc_timestamp = time(NULL);    
    struct tm *local_time = localtime(&utc_timestamp);

    return utc_timestamp + local_time->tm_gmtoff;
}

inline std::string 
GetLocalTime() {
    std::string result = ""; 
    char buf[100]; 
    time_t time_; 
    struct tm *tm;

    time_ = time(NULL); 
    tm = localtime(&time_); 
    if (tm == NULL)
        return NULL;
    strftime(buf, 100,"%a_%d_%b_%T_%Y", tm); 
    result = std::string(buf); 
    return result; 
}

inline 
std::string getMacAddress() {
    int fd, len;
    char macaddr[64] = {0};

    fd = open(WLAN_IFACE_ADDRESS_PATH, O_RDONLY);
    if (fd < 0) {
        LOG_ERRO("Could not open the '%s'.", WLAN_IFACE_ADDRESS_PATH);
    }

    len = read(fd, macaddr, WLAN_IFACE_ADDRESS_LEN);
    close(fd);
    if (len != WLAN_IFACE_ADDRESS_LEN) {
        LOG_ERRO("Error reading the '%s'.", WLAN_IFACE_ADDRESS_PATH);
    }

    return std::string(macaddr); 
}

typedef struct {
    std::string execute; 
    std::string kill; 
    int priority; 
    std::string logpath; 
} Service; 



#endif // GENERIC_HPP