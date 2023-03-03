#ifndef LIBRARIES_LBUS_NODE_HPP
#define LIBRARIES_LBUS_NODE_HPP

#include <pthread.h>

#include <functional>
#include <string>
#include <map>

#include "Libraries/Queue/BlockingQueue.hpp" 
#include "LBus.hpp"

#define THREAD_HANDLE_MESSAGE_MAX   16

namespace LBusNode {

class Server {
public:
    typedef struct {
        std::function<void(const LBus::Message *message, void *user_data)> handle;
        pthread_mutex_t mutex;
        void *user_data;
    } EndpointHandle;

public:
    Server(LBus *main_bus);
    ~Server();

    int Start(std::string module, int threads = 1);
    void Stop();

    int Subscribe(const std::string& endpoint, 
                    LBus::Method method, 
                    std::function<void(const LBus::Message *message, void *user_data)> handle,
                    void *user_data);
                    
    void Unsubscribe(std::string endpoint, LBus::Method method);

    void Response(const LBus::Message *message, void *buffer, size_t buffer_size, LBus::FreeMessage free_buffer);

private:
    bool is_start_;

    int threads_;
    pthread_t handle_message_ptid_[THREAD_HANDLE_MESSAGE_MAX];

    LBus *main_bus_;
    std::string domain_;
    BlockingQueue<LBus::Message *> pending_message_queue_;
    std::map<std::string, EndpointHandle *> endpoint_to_handle_map_;

    static void *HandlePendingMessage(void *arg);

};

class Client {
public:
    Client(LBus *main_bus);
    ~Client();

    void Publish(const std::string& module,
                    const std::string& endpoint,
                    LBus::Method method, 
                    const LBus::Transaction& request);

    int Publish(const std::string& module,
                const std::string& endpoint,
                LBus::Method method,
                time_t timeout, 
                const LBus::Transaction& request,
                LBus::Transaction *response);

    
private:
    LBus *main_bus_;
    
};

}; //namespace LBusNode


#endif //LIBRARIES_LBUS_NODE_HPP