#include "Libraries/LBus/LBus.hpp"

#include <unistd.h>

std::string LBus::method_to_string[] = {
    "GET",
    "SET",
    "CREATE",
    "DELETE",
    "CUSTOM"
};

LBus::LBus() : 
                serial_(0),
                serial_to_response_mutex_(PTHREAD_MUTEX_INITIALIZER) {

}

LBus::~LBus() { 

}

bool 
LBus::InsertResponse(uint64_t serial, Transaction *transaction) {
    bool ret = false;

    transaction->buffer = nullptr;
    transaction->buffer_size = 0;
    transaction->free_buffer = nullptr;

    pthread_mutex_lock(&serial_to_response_mutex_);
    if (serial_to_response_map_.find(serial) == serial_to_response_map_.end()) {
        serial_to_response_map_.insert(std::pair<uint64_t, Transaction *>(serial, transaction));
        ret = true;
    }

    pthread_mutex_unlock(&serial_to_response_mutex_);

    return ret;
}

bool 
LBus::ContainResponse(uint64_t serial) {
    bool ret = false;

    pthread_mutex_lock(&serial_to_response_mutex_);
    if (serial_to_response_map_.find(serial) != serial_to_response_map_.end()) {
        ret = true;
    }

    pthread_mutex_unlock(&serial_to_response_mutex_);

    return ret;
}

bool 
LBus::EraseResponse(uint64_t serial) {
    bool ret = false;

    pthread_mutex_lock(&serial_to_response_mutex_);
    if (serial_to_response_map_.find(serial) != serial_to_response_map_.end()) {
        serial_to_response_map_.erase(serial);
        ret = true;
    }

    pthread_mutex_unlock(&serial_to_response_mutex_);

    return ret;
}

void 
LBus::UpdateAndEraseResponse(uint64_t serial, Transaction *transaction) {
    pthread_mutex_lock(&serial_to_response_mutex_);
    if (serial_to_response_map_.find(serial) != serial_to_response_map_.end()) {
        if (transaction != nullptr) {
            auto utemp = serial_to_response_map_[serial];

            if (utemp == nullptr) {
                if (transaction->free_buffer) 
                    transaction->free_buffer(transaction->buffer);
                
            } else {
                utemp->buffer = malloc(transaction->buffer_size);
                memcpy(utemp->buffer, transaction->buffer, transaction->buffer_size);
                utemp->buffer_size = transaction->buffer_size;
                utemp->free_buffer = transaction->free_buffer;
            }
        }

        serial_to_response_map_.erase(serial);
    } else {
        if (transaction && transaction->free_buffer) 
            transaction->free_buffer(transaction->buffer);
        
    }

    pthread_mutex_unlock(&serial_to_response_mutex_);
}

uint64_t
LBus::GetSerial() {
    return ++serial_;
}

void *
LBus::GetTransaction(const Transaction *transaction) {
    return transaction->buffer;
}

void 
LBus::

FreeTransaction(LBus::Transaction *transaction) {
    if (transaction->buffer && transaction->buffer_size) {
        if (transaction->free_buffer != nullptr)
            transaction->free_buffer(transaction->buffer);

        free(transaction->buffer);
    }
}