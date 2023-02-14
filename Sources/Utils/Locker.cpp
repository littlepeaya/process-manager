//
// Created by TrungTQ on 18 Feb 2017 16:25:43
//

#include <iostream>
#include "Libraries/Utils/Locker.hpp"
#include "Libraries/Utils/LockerImpl.hpp"

namespace libX {
namespace utils {

/// @fn     Locker
/// @brief  None
/// @param  None
/// @return None
Locker::Locker(
) : Core (*new LockerImpl()) {
}

Locker::Locker(
    LockerImpl& d
) : Core (d) {
}

/// @fn     ~Locker
/// @brief  None
/// @param  None
/// @return None
Locker::~Locker(
) {
}

/// @fn     PtrImpl
/// @brief  None
/// @param  None
/// @return IMPL*
LockerImpl* 
Locker::PtrImpl(
) {
    return reinterpret_cast<LockerImpl*>(m_ptrImpl.Data());
}

/// @fn     PtrImpl
/// @brief  None
/// @param  None
/// @return IMPL*
const LockerImpl* 
Locker::PtrImpl(
) const {
    return reinterpret_cast<const LockerImpl*>(m_ptrImpl.Data());
}

LockerImpl*
Locker::GetPtr(
) {
    return reinterpret_cast<LockerImpl*>(m_ptrImpl.Data());
}

const LockerImpl*
Locker::GetPtr(
) const {
    return reinterpret_cast<const LockerImpl*>(m_ptrImpl.Data());
}

/// @fn     Lock
/// @brief  None
/// @param  None
/// @return None
bool_t
Locker::Lock(
) {
    auto ptr = PtrImpl();
    return ptr->Lock();
}

/// @fn     UnLock
/// @brief  None
/// @param  None
/// @return None
bool_t
Locker::UnLock(
) {
    auto ptr = PtrImpl();
    return ptr->UnLock();
}

/// @fn     TryLock
/// @brief  None
/// @param  None
/// @return None
bool_t
Locker::TryLock(
) {
    auto ptr = PtrImpl();
    return ptr->TryLock();
}

/// @fn     TryLock
/// @brief  None
/// @param  None
/// @return None
bool_t 
Locker::TryLock(
    u32_t miliSecs
) {
    auto ptr = PtrImpl();
    return ptr->TryLock(miliSecs);
}

/// @fn     IsLocked
/// @brief  None
/// @param  None
/// @return None
bool_t
Locker::IsLocked(
) {
    auto ptr = PtrImpl();
    return ptr->IsLocked();
}

}
}
