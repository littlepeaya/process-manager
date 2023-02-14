//
// Created by TrungTQ on 2017-03-17 14:25:00
//

#include "Libraries/Utils/RwLocker.hpp"
#include "Libraries/Utils/RwLockerImpl.hpp"

namespace libX {
namespace utils {

/// @fn     PtrImpl
/// @brief  None
/// @param  None
/// @return IMPL*
RwLockerImpl*
RwLocker::PtrImpl(
) {
    return reinterpret_cast<RwLockerImpl*>(m_ptrImpl.Data());
}

/// @fn     PtrImpl
/// @brief  None
/// @param  None
/// @return IMPL*
const RwLockerImpl* 
RwLocker::PtrImpl(
) const {
    return reinterpret_cast<const RwLockerImpl*>(m_ptrImpl.Data());
}

/// @fn     RwLocker
/// @brief  None
/// @param  None
/// @retval None
RwLocker::RwLocker(
) : Core (*new RwLockerImpl()) {
}

RwLocker::RwLocker(
    RwLockerImpl& d
) : Core (d) {
}

/// @fn     ~RwLocker
/// @brief  None
/// @param  None
/// @retval None
RwLocker::~RwLocker(
) {
}

/// @fn     ReadLock
/// @brief  None
/// @param  None
/// @retval None
bool_t
RwLocker::ReadLock(
) {
    auto ptr = PtrImpl();
    return ptr->ReadLock();
}

/// @fn     TryReadLock
/// @brief  None
/// @param  None
/// @retval None
bool_t 
RwLocker::TryReadLock(
) {
    auto ptr = PtrImpl();
    return ptr->TryReadLock();
}

/// @fn     WriteLock
/// @brief  None
/// @param  None
/// @retval None
bool_t
RwLocker::WriteLock(
) {
    auto ptr = PtrImpl();
    return ptr->WriteLock();
}

/// @fn     TryWriteLock
/// @brief  None
/// @param  None
/// @retval None
bool_t 
RwLocker::TryWriteLock(
) {
    auto ptr = PtrImpl();
    return ptr->TryWriteLock();
}

/// @fn     UnLock
/// @brief  None
/// @param  None
/// @retval None
bool_t
RwLocker::UnLock(
) {
    auto ptr = PtrImpl();
    return ptr->UnLock();
}

}
}
