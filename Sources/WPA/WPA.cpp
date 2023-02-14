#include "Libraries/WPA/WPA.hpp"

#include <sys/select.h>
#include <unistd.h>

#include <cstring>

#include "Libraries/Log/LogPlus.hpp"

#define REMAINING_CONNECTION_CYCLE_PER_TIME    60000

WPA::Client::Client() :
                        ctrl_iface_{nullptr},
                        event_index_(0),
                        ctrl_fd_(-1),
                        is_start_(false) {
    remaining_connect_timer_.RegisterTimerHandler(RemainConnection, this);
    remaining_connect_timer_.Start(30 * 1000, REMAINING_CONNECTION_CYCLE_PER_TIME);
}

WPA::Client::~Client() {
    if (is_start_ == true)
        Stop();
    
    remaining_connect_timer_.CancelTimerHandler(RemainConnection);
}

int 
WPA::Client::Start(std::string pathname) {
    int ret;

    if (is_start_ == true)
        return -1;

    ctrl_iface_[COMMAND_CTRL] = wpa_ctrl_open(pathname.c_str());
    ctrl_iface_[EVENT_CTRL] = wpa_ctrl_open(pathname.c_str());
    if (ctrl_iface_[COMMAND_CTRL] == nullptr || ctrl_iface_[EVENT_CTRL] == nullptr) {
        ctrl_iface_[COMMAND_CTRL] != nullptr ? wpa_ctrl_close(ctrl_iface_[COMMAND_CTRL]) : (void)nullptr;
        ctrl_iface_[EVENT_CTRL] != nullptr ? wpa_ctrl_close(ctrl_iface_[EVENT_CTRL]) : (void)nullptr;
        LOG_ERRO("Error opening control interface\n");
        return -1;
    }

    is_start_ = true;
    ctrl_iface_pathname_ = pathname;
    wpa_ctrl_attach(ctrl_iface_[EVENT_CTRL]);
    ctrl_fd_ = wpa_ctrl_get_fd(ctrl_iface_[EVENT_CTRL]);
    pthread_create(&routing_event_pthread_id_, nullptr, RoutePendingEvents, this);

    return 0;
}

void 
WPA::Client::Stop() {
    if (is_start_ == false)
        return;

    is_start_ = false;
    if (pthread_cancel(routing_event_pthread_id_) == 0)
        pthread_join(routing_event_pthread_id_, nullptr);

    wpa_ctrl_close(ctrl_iface_[COMMAND_CTRL]);
    wpa_ctrl_close(ctrl_iface_[EVENT_CTRL]);
    ctrl_iface_[COMMAND_CTRL] = nullptr;
    ctrl_iface_[EVENT_CTRL] = nullptr;
}

int 
WPA::Client::Scan(std::vector<BSS> *bsses, int timeout) {
    BSS bss;
    std::string reply;
    size_t pair_position = 0;
    std::string pair, key, value;
    uint32_t bss_index = 0;
    struct timespec current_time, expired_time;

    int index;
    bool scan_results = false;
    index = RegisterEvent(WPA_EVENT_SCAN_RESULTS, [&scan_results] (std::string event, void *user_data) {
        scan_results = true;
    }, nullptr);

    reply = Request(std::string("SCAN"));
    if (reply.find("OK") == std::string::npos) {
        LOG_ERRO("Error requesting to scan network\n");
        goto out;
    }

    if (timeout >= 0) {
        clock_gettime(CLOCK_REALTIME, &expired_time);
        expired_time.tv_sec += timeout / 1000;
        expired_time.tv_nsec += (timeout % 1000) * 1000000;
        if (expired_time.tv_nsec >= 1000000000) {
            expired_time.tv_sec += 1;
            expired_time.tv_nsec -= 1000000000;
        }

        while (scan_results == false) {
            clock_gettime(CLOCK_REALTIME, &current_time);

            if (current_time.tv_sec > expired_time.tv_sec)
                break;
            
            if (current_time.tv_sec == expired_time.tv_sec &&
                    current_time.tv_nsec > expired_time.tv_nsec) 
                break;
            
            usleep(100000);
        }
    } else {
        while (scan_results == false)
            usleep(100000);
    }

    if (scan_results == false)
        return -1;

    while (true) {
        reply = Request(std::string("BSS") + std::string(" ") + std::to_string(bss_index));
        if (reply.empty() == true)
            break;
        
        while ((pair_position = reply.find("\n")) != std::string::npos) {
            pair = reply.substr(0, pair_position);
            reply.erase(0, pair_position + strlen("\n"));

            key = pair.substr(0, pair.find("="));
            value = pair.substr(pair.find("=") + 1, std::string::npos);
            if (key == "ssid")
                bss.ssid.assign(value);
            else if (key == "bssid")
                bss.bssid.assign(value);
            else if (key == "freq")
                bss.frequency = std::atoi(value.c_str());
            else if (key == "flags")
                bss.flags.assign(value);
            else if (key == "level")
                bss.level = std::atoi(value.c_str());
        }

        bsses->push_back(bss);
        bss_index++;
    }

    return 0;

out:
    UnregisterEvent(index);
    return -1;
}

int 
WPA::Client::Connect(std::string ssid, std::string psk, int timeout) {
    std::string reply;
    uint32_t network_id;
    bool is_connect = false;
    int index;

    reply = Request(std::string("ADD_NETWORK"));
    if (reply.find("FAIL") != std::string::npos) {
        LOG_ERRO("Error requesting to add network\n");
        goto out;
    }
    LOG_INFO("%s %d \n", __FUNCTION__, __LINE__);

    network_id = std::atoi(reply.c_str());
    reply = Request(std::string("SET_NETWORK") + std::string(" ") + std::to_string(network_id) + std::string(" ") + std::string("ssid") + std::string(" ") + std::string("\"") + ssid + std::string("\""));
    if (reply.find("OK") == std::string::npos) {
        LOG_ERRO("Error setting ssid network\n");
        goto out;
    }
    LOG_INFO("%s %d \n", __FUNCTION__, __LINE__);
    reply = Request(std::string("SET_NETWORK") + std::string(" ") + std::to_string(network_id) + std::string(" ") + std::string("psk") + std::string(" ") + std::string("\"") + psk + std::string("\""));
    if (reply.find("OK") == std::string::npos) {
        LOG_ERRO("Error setting psk network\n");
        goto out;
    }
    LOG_INFO("%s %d \n", __FUNCTION__, __LINE__);
    index = RegisterEvent(WPA_EVENT_CONNECTED, [&is_connect] (std::string event, void *user_data) {
        is_connect = true;
    }, nullptr);
    LOG_INFO("%s %d \n", __FUNCTION__, __LINE__);
    reply = Request(std::string("SELECT_NETWORK") + std::string(" ") + std::to_string(network_id));
    if (reply.find("FAIL") != std::string::npos) {
        LOG_ERRO("Error requesting to add network\n");
        UnregisterEvent(index);
        goto out;
    }
    LOG_INFO("%s %d \n", __FUNCTION__, __LINE__);
    if (timeout >= 0) {
        struct timespec current_time, expired_time;
        clock_gettime(CLOCK_REALTIME, &expired_time);
        expired_time.tv_sec += timeout / 1000;
        expired_time.tv_nsec += (timeout % 1000) * 1000000;
        if (expired_time.tv_nsec >= 1000000000) {
            expired_time.tv_sec += 1;
            expired_time.tv_nsec -= 1000000000;
        }
    LOG_INFO("%s %d \n", __FUNCTION__, __LINE__);
        while (is_connect == false) {
            clock_gettime(CLOCK_REALTIME, &current_time);

            if (current_time.tv_sec > expired_time.tv_sec)
                break;
            
            if (current_time.tv_sec == expired_time.tv_sec &&
                    current_time.tv_nsec > expired_time.tv_nsec) 
                break;
            
            usleep(100000);
        }
    } else {
        while (is_connect == false)
            usleep(100000);
    }

    UnregisterEvent(index);
    if (is_connect == true) {
        Request(std::string("SAVE_CONFIG"));
        return 0;
    }
    LOG_INFO("%s %d \n", __FUNCTION__, __LINE__);
out:
    Request(std::string("SELECT_NETWORK") + std::string(" ") + std::to_string(network_id - 1));
    Request(std::string("REMOVE_NETWORK") + std::string(" ") + std::to_string(network_id));
    return -1;
}

void 
WPA::Client::Close() {
    std::string reply;

    reply = Request(std::string("REMOVE_NETWORK") + std::string(" ") + std::string("all"));
    if (reply.compare("OK") == false) {
        LOG_ERRO("Error requesting to remove network\n");
        return;
    }

    Request(std::string("SAVE_CONFIG"));
}

int 
WPA::Client::RegisterEvent(std::string event, EventHandler event_handle, void *user_data) {
    EventContainer *event_container = new EventContainer{event, event_handle, user_data, event_index_};
    event_containers_.push_back(event_container);
    return event_index_++;
}

void
WPA::Client::UnregisterEvent(uint32_t index) {
    for (auto it = event_containers_.begin(); it != event_containers_.end(); ++it) {
        if (index == (*it)->index) {
            delete *it;
            event_containers_.erase(it);
            break;
        }        
    }
}

void *
WPA::Client::RoutePendingEvents(void *arg) {
    auto instance = (WPA::Client *)arg;
    std::string pending_event;
    fd_set readfds;
    int ret;

    while (instance->is_start_ == true) {
        FD_ZERO(&readfds);
        FD_SET(instance->ctrl_fd_, &readfds);

        ret = select(instance->ctrl_fd_ + 1, &readfds, nullptr, nullptr, nullptr);
        if (ret < 0) {
            LOG_ERRO("Error monitoring events, err: %s\n", strerror(errno));
            continue;
        }

        pending_event = instance->Receive(); 
        if (pending_event.empty() == true)
            continue;

        for (auto it = instance->event_containers_.begin(); it != instance->event_containers_.end(); ++it) {
            if (pending_event.find((*it)->event) != std::string::npos)
                (*it)->event_handle(pending_event, (*it)->user_data);
        }
    }

    pthread_detach(pthread_self());
    pthread_exit(nullptr);
}

std::string 
WPA::Client::Request(std::string message) {
    char reply[4096] = "";
    size_t reply_len = sizeof(reply);
    int ret;

    if (is_start_ == false)
        return std::string("");

    ret = wpa_ctrl_request(ctrl_iface_[COMMAND_CTRL], message.c_str(), message.length(), reply, &reply_len, nullptr);
    if (ret != 0) {
        LOG_ERRO("Error requesting the '%s' command\n", message.c_str());
        return std::string("");
    }

    return std::string(reply, reply_len);
}

std::string 
WPA::Client::Receive() {
    char reply[128] = "";
    size_t reply_len = sizeof(reply);
    int ret;

    if (is_start_ == false)
        return std::string("");

    ret = wpa_ctrl_recv(ctrl_iface_[EVENT_CTRL], reply, &reply_len);
    if (ret != 0) {
        LOG_ERRO("Error receiving events\n");
        return std::string("");
    }

    return std::string(reply, reply_len);
}

int 
WPA::Client::GetStatus(Status *status) {
    std::string reply;

    reply = Request(std::string("STATUS"));
    if (reply.empty() == true) {
        LOG_ERRO("Error getting status\n");
        return -1;
    }    

    size_t pair_position = 0;
    std::string pair, key, value;

    while ((pair_position = reply.find("\n")) != std::string::npos) {
        pair = reply.substr(0, pair_position);
        reply.erase(0, pair_position + strlen("\n"));

        key = pair.substr(0, pair.find("="));
        value = pair.substr(pair.find("=") + 1, std::string::npos);
        if (key == "ssid")
            status->ssid.assign(value);
        else if (key == "bssid")
            status->bssid.assign(value);
        else if (key == "wpa_state")
            status->state.assign(value);
    }

    return 0;
}

std::string
WPA::Client::GetSsid() {
    std::string ssid, reply;

    reply = Request(std::string("STATUS"));
    if (reply.empty() == true) {
        LOG_ERRO("Error getting status\n");
        return std::string("");
    }    

    size_t pair_position = 0;
    std::string pair, key, value;
    uint32_t bss_index = 0;

    while ((pair_position = reply.find("\n")) != std::string::npos) {
        pair = reply.substr(0, pair_position);
        reply.erase(0, pair_position + strlen("\n"));

        key = pair.substr(0, pair.find("="));
        value = pair.substr(pair.find("=") + 1, std::string::npos);
        if (key == "ssid")
            ssid.assign(value);

    }

    return ssid;
}

int 
WPA::Client::RemainConnection(void *user_data) {
    auto instance = (WPA::Client *)user_data;
    std::string pathname = instance->ctrl_iface_pathname_;
    std::string reply;

    reply = instance->Request(std::string("PING"));
    if (reply.compare("PONG") == true)
        return 0;

    instance->Stop();
    if (instance->Start(pathname) < 0) {
        LOG_ERRO("Error starting wpa client\n");
        return -1;
    }

    return 0;
}