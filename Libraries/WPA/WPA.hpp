#ifndef LIBRARIES_WPA_HPP
#define LIBRARIES_WPA_HPP

#include <wpa_ctrl.h>
#include <pthread.h>

#include <string>
#include <vector>
#include <map>
#include <functional>

#include "Libraries/Timer/Timer.hpp"

namespace WPA {

class Client {
public:
    typedef struct {
        std::string bssid, ssid, flags;
        int frequency, level;
    } BSS;

    typedef std::function<void(std::string event, void *user_data)> EventHandler;

    typedef struct {
        std::string event;
        EventHandler event_handle;
        void *user_data;
        uint32_t index;
    } EventContainer;

    typedef struct {
        std::string bssid, ssid, state;
    } Status;

public:
    Client();
    ~Client();

    int Start(std::string pathname);
    void Stop();

    int Connect(std::string ssid, std::string psk, int timeout);
    void Close();

    int Scan(std::vector<BSS> *bsses, int timeout);

    int RegisterEvent(std::string event, EventHandler event_handle, void *user_data);
    void UnregisterEvent(uint32_t index);

    int GetStatus(Status *status);
    std::string GetSsid();
private:
    enum {
        COMMAND_CTRL = 0,
        EVENT_CTRL
    };

    struct wpa_ctrl *ctrl_iface_[2];
    std::string ctrl_iface_pathname_;
    bool is_start_;

    int ctrl_fd_;
    uint32_t event_index_;
    std::vector<EventContainer *> event_containers_;
    pthread_t routing_event_pthread_id_;
    Timer remaining_connect_timer_;

    static void *RoutePendingEvents(void *arg);

    std::string Receive();
    std::string Request(std::string command);

    static int RemainConnection(void *user_data);
};

} // WPA
#endif // LIBRARIES_WPA_HPP