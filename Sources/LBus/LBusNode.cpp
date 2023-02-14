#include "Libraries/LBus/LBusNode.hpp"

#include <unistd.h>
#include <time.h>
#include <sys/time.h>

// TODO: Server
LBusNode::Server::Server(LBus *main_bus) :
                                            main_bus_(main_bus) {

}

LBusNode::Server::~Server() {

}

int 
LBusNode::Server::Start(std::string module, int threads) {
    int ret;

    if (threads > THREAD_HANDLE_MESSAGE_MAX) {
        LOG_ERRO("The number of threads over the number of allowing threads");
        return -1;
    }

    if (main_bus_->domain_to_queue_map_.find(module) != main_bus_->domain_to_queue_map_.end()) {
        LOG_ERRO("The module '%s' has existed");
        return -1;
    }

    is_start_ = true;
    threads_ = threads;
    for (int i = 0; i < threads_; ++i) {
        ret = pthread_create(&handle_message_ptid_[i], nullptr, HandlePendingMessage, this);
        if (ret < 0) {
            LOG_ERRO("Failed to pthread_create, err: %s", strerror(ret));
            goto err;
        }
    }

    domain_ = module;
    main_bus_->domain_to_queue_map_.insert(std::pair<std::string, BlockingQueue<LBus::Message *> *>(domain_, &pending_message_queue_));

    return 0;

err:
    is_start_ = false;
    for (int i = 0; i < threads_ && handle_message_ptid_[i] != 0; ++i) {
        pthread_join(handle_message_ptid_[i], nullptr);
    }

    threads_ = 0;

    return -1;
}

void 
LBusNode::Server::Stop() {
    main_bus_->domain_to_queue_map_.erase(domain_);
    domain_.clear();

    is_start_ = false;
    for (int i = 0; i < threads_; ++i) {
        pthread_join(handle_message_ptid_[i], nullptr);
    }

    threads_ = 0;
}

int 
LBusNode::Server::Subscribe(const std::string& endpoint, 
                            LBus::Method method, 
                            std::function<void(const LBus::Message *message, void *user_data)> handle, 
                            void *user_data) {
    std::string key = LBus::method_to_string[method] + "_" + endpoint;

    if (endpoint_to_handle_map_.find(key) != endpoint_to_handle_map_.end()) {
        LOG_ERRO("The '%s' has been subscribed", key.c_str());
        return -1;
    }

    auto endpoint_handle = new EndpointHandle{handle, PTHREAD_MUTEX_INITIALIZER, user_data};
    endpoint_to_handle_map_.insert(std::pair<std::string, EndpointHandle *>(key, endpoint_handle));
    return 0;
}

void 
LBusNode::Server::Unsubscribe(std::string endpoint, LBus::Method method) {
    std::string key = LBus::method_to_string[method] + "_" + endpoint;

    if (endpoint_to_handle_map_.find(key) == endpoint_to_handle_map_.end()) {
        LOG_ERRO("The '%s' doesn't exist", key);
        return;
    }

    auto endpoint_handle = endpoint_to_handle_map_[key];
    endpoint_to_handle_map_.erase(key);
    delete endpoint_handle;
}

void *


LBusNode::Server::HandlePendingMessage(void *arg) {
    time_t t; 
    struct timeval tv; 
    auto self = (LBusNode::Server *)arg;
    EndpointHandle *endpoint_handle;
    LBus::Message *message;
    std::string key;

    while (self->is_start_) {
        message = self->pending_message_queue_.Pop();
          if(gettimeofday(&tv, NULL) == -1)
            printf("error\n");
        LOG_INFO("time return %ld secs %ld microsecs\n", (long) tv.tv_sec, (long) tv.tv_usec);

        LOG_DBUG("[%lld] the message '%s' - '%s' to the '%s' module", message->serial, 
                                                            LBus::method_to_string[message->method].c_str(), 
                                                            message->endpoint.c_str(), 
                                                            message->module.c_str());
       
        key = LBus::method_to_string[message->method] + "_" + message->endpoint;
        if (self->endpoint_to_handle_map_.find(key) == self->endpoint_to_handle_map_.end()) 
            goto out;
        
        endpoint_handle = self->endpoint_to_handle_map_[key];
        pthread_mutex_lock(&endpoint_handle->mutex);
        endpoint_handle->handle(message, endpoint_handle->user_data);
        pthread_mutex_unlock(&endpoint_handle->mutex);

out:
        LBus::FreeTransaction(&message->request);
        delete message;
    }

    pthread_exit(nullptr);
}

void
LBusNode::Server::Response(const LBus::Message *message, void *buffer, size_t buffer_size, LBus::FreeMessage free_buffer) {
    LBus::Transaction transaction{buffer, buffer_size, free_buffer};
    main_bus_->UpdateAndEraseResponse(message->serial, (!buffer || !buffer_size) ? nullptr : &transaction);
}


// TODO: Client
LBusNode::Client::Client(LBus *main_bus) :
                                            main_bus_(main_bus) {

}
    
LBusNode::Client::~Client() {

}

void 
LBusNode::Client::Publish(const std::string& module, 
                            const std::string& endpoint,
                            LBus::Method method, 
                            const LBus::Transaction& request) {
    if (main_bus_->domain_to_queue_map_.find(module) == main_bus_->domain_to_queue_map_.end()) {
        if (request.buffer && request.buffer_size && request.free_buffer)
            request.free_buffer(request.buffer);

        return;
    }

    auto message = new LBus::Message;
    message->module = module;
    message->endpoint = endpoint;
    message->method = method;
    message->serial = main_bus_->GetSerial();
    message->request = {0};
    message->timeout = 0;

    // allocate memory to copy the request, and it is going free after routing to module 
    if (request.buffer && request.buffer_size) {
        message->request.buffer = malloc(request.buffer_size);
        memcpy(message->request.buffer, request.buffer, request.buffer_size);
        message->request.buffer_size = request.buffer_size;
        message->request.free_buffer = request.free_buffer;
    }

    auto queue = main_bus_->domain_to_queue_map_[module];
    queue->Push(message);
}

int 
LBusNode::Client::Publish(const std::string& module,
                            const std::string& endpoint,
                            LBus::Method method,
                            time_t timeout,
                            const LBus::Transaction& request,
                            LBus::Transaction *response) {
    bool have_response = false;
    int ret = 1;

    if (main_bus_->domain_to_queue_map_.find(module) == main_bus_->domain_to_queue_map_.end()) {
        if (request.buffer && request.buffer_size && request.free_buffer)
            request.free_buffer(request.buffer);

        return -1;
    }

    auto serial = main_bus_->GetSerial();
    auto message = new LBus::Message();
    message->module = module;
    message->endpoint = endpoint;
    message->method = method;
    message->serial = serial;
    message->request = {0};
    message->timeout = timeout;

    // allocate memory to copy the request, and it is going free after routing to module 
    if (request.buffer && request.buffer_size) {
        message->request.buffer = malloc(request.buffer_size);
        memcpy(message->request.buffer, request.buffer, request.buffer_size);
        message->request.buffer_size = request.buffer_size;
        message->request.free_buffer = request.free_buffer;
    }

    main_bus_->InsertResponse(serial, response);

    // add message to queue of module;
    auto queue = main_bus_->domain_to_queue_map_[module];
    queue->Push(message);

    while (true) {
        if (timeout == 0) {
            if (main_bus_->EraseResponse(serial) == false) {
                have_response = true;
            } else {
                have_response = false;
            }

            break;
        }

        if (main_bus_->ContainResponse(serial) == false) {
            have_response = true;
            break;
        }

        usleep(100000); // 10ms
        timeout--;

    }

    return have_response == true ? 0 : 1;
}
