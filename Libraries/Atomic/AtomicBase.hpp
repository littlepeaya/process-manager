//
// Created by TrungTQ on 09/08/17.
//

#ifndef LIBX_ATOMIC_BASE_HPP_
#define LIBX_ATOMIC_BASE_HPP_

#include "Libraries/Atomic/AtomicOp.hpp"

template<typename T>
class AtomicBase {
public:
    typedef AtomicOp<T> Op;
    typename Op::Type m_qValue;

    T Load() const noexcept { return Op::Load(m_qValue); }

    void Store(T newValue) noexcept { Op::Store(m_qValue, newValue); }

    T LoadAcquire() const noexcept { return Op::LoadAcquire(m_qValue); }

    void StoreRelease(T newValue) noexcept { Op::StoreRelease(m_qValue, newValue); }

    operator T() const noexcept { return LoadAcquire(); }

    T operator=(T newValue) noexcept { StoreRelease(newValue); return newValue; }

    T FetchAndStoreRelaxed(T newValue) noexcept { return Op::FetchAndStoreRelaxed(m_qValue, newValue); }

    T FetchAndStoreAcquire(T newValue) noexcept { return Op::FetchAndStoreAcquire(m_qValue, newValue); }

    T FetchAndStoreRelease(T newValue) noexcept { return Op::FetchAndStoreRelease(m_qValue, newValue); }

    T FetchAndStoreOrdered(T newValue) noexcept { return Op::FetchAndStoreOrdered(m_qValue, newValue); }
};

#endif // LIBX_ATOMIC_BASE_HPP_
