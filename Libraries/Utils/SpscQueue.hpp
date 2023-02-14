/// Created by TrungTQ on 10 Mar 2017 18:02:53

#ifndef LIBX_UTILS_SPSCQUEUE_HPP_
#define LIBX_UTILS_SPSCQUEUE_HPP_

#include <stdlib.h>
#include <assert.h>
#include <atomic>
#include "Libraries/Utils/Typedefs.h"

namespace libX {
namespace utils {

template<class C, u32_t N = 1024>
struct SpscQueue {
    typedef C           value_type;
    C* const            m_record;
    std::atomic<u32_t>  m_readId;
    std::atomic<u32_t>  m_writeId;
    
    SpscQueue(const SpscQueue& copied) = delete;
    SpscQueue& operator= (const SpscQueue& copied) = delete;

    explicit SpscQueue();
    ~SpscQueue();

    C* front();
    void pop();
    bool_t empty();
    bool_t full();
    bool_t read(C& record);
    template<class... Args>
    bool_t push(Args&&... args);
};


/// @fn     SpscQueue
/// @brief  None
/// @param  None
/// @return None
template<class C, u32_t N>
SpscQueue<C, N>::SpscQueue(
) : m_record  (static_cast<C*>(std::malloc(sizeof(C) * N)))
  , m_readId  (0)
  , m_writeId (0) {
}

/// @fn     ~SpscQueue
/// @brief  None
/// @param  None
/// @return None
template<class C, u32_t N>
SpscQueue<C, N>::~SpscQueue(
) {
    if (!std::is_trivially_destructible<C>::value) {
        size_t read = m_readId;
        size_t end = m_writeId;
        while (read != end) {
            m_record[read].~C();
            if (++read == N) {
                read = 0;
            }
        }
   }

   std::free(m_record);
}

/// @fn     font
/// @brief  None
/// @param  None
/// @return None
template<class C, u32_t N>
inline C*
SpscQueue<C, N>::front() {
    auto const currentRead = m_readId.load(std::memory_order_relaxed);
    if (currentRead == m_writeId.load(std::memory_order_acquire)) {
        return NULL;
    }
    return &m_record[currentRead];
}

/// @fn     pop
/// @brief  None
/// @param  None
/// @return None
template<class C, u32_t N>
inline void
SpscQueue<C, N>::pop() {
    auto const currentRead = m_readId.load(std::memory_order_relaxed);
    assert(currentRead != m_writeId.load(std::memory_order_acquire));

    auto nextRecord = currentRead + 1;
    if (nextRecord == N) {
        nextRecord = 0;
    }
    
    m_record[currentRead].~C();
    m_readId.store(nextRecord, std::memory_order_release);

}

/// @fn     empty
/// @brief  None
/// @param  None
/// @return None
template<class C, u32_t N>
inline bool_t
SpscQueue<C, N>::empty() {
    return (m_readId.load(std::memory_order_acquire) == m_writeId.load(std::memory_order_acquire));
}


/// @fn     empty
/// @brief  None
/// @param  None
/// @return None
template<class C, u32_t N>
inline bool_t
SpscQueue<C, N>::full() {
    auto nextRecord = m_writeId.load(std::memory_order_acquire) + 1;
    if (nextRecord == N) {
        nextRecord = 0;
    }
    if (nextRecord != m_readId.load(std::memory_order_acquire)) {
        return FALSE;
    }
    return TRUE; // utils is full
}

/// @fn     read
/// @brief  None
/// @param  None
/// @return None
template<class C, u32_t N>
inline bool_t
SpscQueue<C, N>::read(
    C& record
) {
    auto const currentRead = m_readId.load(std::memory_order_relaxed);
    if (currentRead == m_writeId.load(std::memory_order_acquire)) {
        return FALSE;
    }
    auto nextRecord = currentRead + 1;
    if (nextRecord == N) {
        nextRecord = 0;
    }
    record = std::move(m_record[currentRead]);
    m_record[currentRead].~C();
    m_readId.store(nextRecord, std::memory_order_release);
    return TRUE;
}


/// @fn     write
/// @brief  None
/// @param  None
/// @return None
template<class C, u32_t N>
template<class... Args>
inline bool_t
SpscQueue<C, N>::push(
    Args&&... args
) {
    auto const currentWrite = m_writeId.load(std::memory_order_relaxed);
    auto nextRecord = currentWrite + 1;
    if (nextRecord == N) {
        nextRecord = 0;
    }
    if (nextRecord != m_readId.load(std::memory_order_acquire)) {
        new (&m_record[currentWrite]) C(std::forward<Args>(args)...);
        m_writeId.store(nextRecord, std::memory_order_release);
        return TRUE;
    }
    return FALSE;
}

}
}

#endif /* !LIBX_UTILS_SPSCQUEUE_HPP_ */
