#ifndef __BLOCKINGQUEUE_HPP__
#define __BLOCKINGQUEUE_HPP__

#include <iostream>
#include <queue>
#include <pthread.h>

#include "Libraries/Log/LogPlus.hpp"

template <typename T>
class BlockingQueue {
private:
    std::queue<T> m_queue;
    pthread_mutex_t m_mutex;
    pthread_cond_t m_cond;

public:
    BlockingQueue();
    ~BlockingQueue() = default;

    bool isEmpty();
    size_t Size();

    void Push(T val);
    T Pop();
    bool TryPop(T *val, time_t timeout);
};

template <typename T>
inline BlockingQueue<T>::BlockingQueue() {
    m_mutex = PTHREAD_MUTEX_INITIALIZER;
    m_cond = PTHREAD_COND_INITIALIZER;
}

template <typename T>
inline bool BlockingQueue<T>::isEmpty() {
    return m_queue.empty();
}

template <typename T>
inline size_t BlockingQueue<T>::Size() {
    return m_queue.size();
}

template <typename T>
inline void BlockingQueue<T>::Push(T val) {
    pthread_mutex_lock(&m_mutex);
    m_queue.push(val);
    pthread_cond_signal(&m_cond);
    pthread_mutex_unlock(&m_mutex);
}

template <typename T>
inline T BlockingQueue<T>::Pop() {
    pthread_mutex_lock(&m_mutex);
    while (isEmpty() == true) 
        pthread_cond_wait(&m_cond, &m_mutex);

    T value = m_queue.front();
    m_queue.pop();
    pthread_mutex_unlock(&m_mutex);

    return value;
}

template <typename T>
inline bool BlockingQueue<T>::TryPop(T *val, time_t timeout) {
    struct timespec abstime;
    int ret;

    if (clock_gettime(CLOCK_REALTIME, &abstime) < 0) {
        LOG_ERRO("Failed to clock_gettime, err=%s", strerror(errno));
        return false;
    }

    abstime.tv_nsec += (timeout % 1000) * 1000000;
    if (abstime.tv_nsec >= 1000000000) {
        abstime.tv_sec += (timeout / 1000) + 1;
        abstime.tv_nsec -= 1000000000;   
    } else
        abstime.tv_sec += (timeout / 1000);

    pthread_mutex_lock(&m_mutex);
    if (isEmpty()) {
        ret = pthread_cond_timedwait(&m_cond, &m_mutex, &abstime);
        if (ret != 0) {
            LOG_ERRO("Failed to pthread_cond_timedwait, err=%s", strerror(ret));
            pthread_mutex_unlock(&m_mutex);
            return false;
        }
    }

    *val = m_queue.front();
    m_queue.pop();
    pthread_mutex_unlock(&m_mutex);

    return true;
}


#endif // __BLOCKINGQUEUE_HPP__
