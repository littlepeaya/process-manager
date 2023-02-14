//
// Created by TrungTQ on 09/08/17.
//

#ifndef LIBX_ATOMIC_CORE_HPP_
#define LIBX_ATOMIC_CORE_HPP_

#include <atomic>
#include "Libraries/Utils/Typedefs.h"

template<typename X>
struct AtomicOp {
    typedef std::atomic<X> Type;

    template<typename T> static inline
    T Load(
        const std::atomic<T>& qValue
    ) noexcept {
        return qValue.load(std::memory_order_relaxed);
    }

    template<typename T> static inline
    T Load(
        const volatile std::atomic<T>& qValue
    ) noexcept {
        return qValue.load(std::memory_order_relaxed);
    }

    template<typename T> static inline
    T LoadAcquire(
        const std::atomic<T>& qValue
    ) noexcept {
        return qValue.load(std::memory_order_acquire);
    }

    template<typename T>
    static inline T
    LoadAcquire(
        const volatile std::atomic<T>& qValue
    ) noexcept {
        return qValue.load(std::memory_order_acquire);
    }

    template<typename T>
    static inline void
    Store(
        std::atomic<T>& qValue, T newValue
    ) noexcept {
        qValue.store(newValue, std::memory_order_relaxed);
    }

    template<typename T>
    static inline void
    StoreRelease(
        std::atomic<T>& qValue,
        T newValue
    ) noexcept {
        qValue.store(newValue, std::memory_order_release);
    }

    template<typename T>
    static inline T
    FetchAndStoreRelaxed(
        std::atomic<T>& qValue,
        T newValue
    ) noexcept {
        qValue.fetch_add(newValue, std::memory_order_relaxed);
    }

    template<typename T>
    static inline T
    FetchAndStoreAcquire(
        std::atomic<T>& qValue,
        T newValue
    ) noexcept {
        qValue.fetch_add(newValue, std::memory_order_acquire);
    }

    template<typename T>
    static inline T
    FetchAndStoreRelease(
        std::atomic<T>& qValue,
        T newValue
    ) noexcept {
        qValue.fetch_add(newValue, std::memory_order_release);
    }

    template<typename T>
    static inline T
    FetchAndStoreOrdered(
        std::atomic<T>& qValue,
        T newValue
    ) noexcept {
        qValue.fetch_add(newValue, std::memory_order_acq_rel);
    }
};

#endif // LIBX_ATOMIC_CORE_HPP_
