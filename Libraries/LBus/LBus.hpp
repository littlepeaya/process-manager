#ifndef LIBRARIES_LBUS_HPP
#define LIBRARIES_LBUS_HPP

#include <string>
#include <map>
#include <atomic>

#include "Libraries/Queue/BlockingQueue.hpp"

class LBus {
public:
    typedef enum {
        GET = 0,
        SET,
        CREATE,
        DELETE,
        CUSTOM
    } Method;

    typedef std::function<void(void *)> FreeMessage;

    // Use to contain primitive type or pointer
    typedef struct {
        void *buffer;
        size_t buffer_size;
        FreeMessage free_buffer;
    } Transaction;

    typedef struct {
        std::string module;
        std::string endpoint;
        Method method;
        time_t timeout; // unit in 100ms
        uint64_t serial; // identifier of message
        Transaction request;
    } Message;

public:
    LBus();
    ~LBus();

    std::atomic<uint64_t> serial_;
    std::map<std::string, BlockingQueue<Message *> *> domain_to_queue_map_;
    std::map<uint64_t, Transaction *> serial_to_response_map_;
    pthread_mutex_t serial_to_response_mutex_;
    static std::string method_to_string[];

    bool InsertResponse(uint64_t serial, Transaction *response);
    bool ContainResponse(uint64_t serial);
    bool EraseResponse(uint64_t serial);
    void UpdateAndEraseResponse(uint64_t serial, Transaction *response);

    uint64_t GetSerial();

    static void *GetTransaction(const Transaction *transaction);
    static void FreeTransaction(Transaction *transaction);
    
};


#endif //LIBRARIES_LBUS_HPP