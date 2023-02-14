//
// Created by TrungTQ on 2017-03-17 14:25:00
//

#include <errno.h>
//#include "Libraries/Log/LogStdo.hpp"
#include "Libraries/Utils/RwLockerImpl.hpp"

namespace libX {
namespace utils {

/// @fn     RwLockerImpl
/// @brief  None
/// @param  None
/// @return None
RwLockerImpl::RwLockerImpl(
) {
    if (pthread_rwlock_init(&m_rwlocker, NULL) != 0) {
//        STDO_ERRO("RwLockerImpl --- rwlock_init error [%03d]:<<%s>>", errno, strerror(errno));
    }
}

/// @fn     ~RwLockerImpl
/// @brief  None
/// @param  None
/// @return None
RwLockerImpl::~RwLockerImpl(
) {
    if (pthread_rwlock_destroy(&m_rwlocker) != 0) {
//        STDO_ERRO("RwLockerImpl --- rwlock_destroy error [%03d]:<<%s>>", errno, strerror(errno));
    }
}

/// @fn     ReadLock
/// @brief  None
/// @param  None
/// @return None
bool_t
RwLockerImpl::ReadLock(
) {
    if (pthread_rwlock_rdlock(&m_rwlocker) != 0) {
//        STDO_ERRO("RwLockerImpl --- rwlock_rdlock error [%03d]:<<%s>>", errno, strerror(errno));
        return FALSE;
    }
    return TRUE;
}

/// @fn     TryReadLock
/// @brief  None
/// @param  None
/// @return None
bool_t 
RwLockerImpl::TryReadLock(
) {
    if (pthread_rwlock_tryrdlock(&m_rwlocker) != 0) {
//        STDO_ERRO("RwLockerImpl --- rwlock_tryrdlock error [%03d]:<<%s>>", errno, strerror(errno));
        return FALSE;
    }
    return TRUE;
}

/// @fn     WriteLock
/// @brief  None
/// @param  None
/// @return None
bool_t
RwLockerImpl::WriteLock(
) {
    if (pthread_rwlock_wrlock(&m_rwlocker) != 0) {
//        STDO_ERRO("RwLockerImpl --- rwlock_wrlock error [%03d]:<<%s>>", errno, strerror(errno));
        return FALSE;
    }
    return TRUE;
}

/// @fn     TryWriteLock
/// @brief  None
/// @param  None
/// @return None
bool_t 
RwLockerImpl::TryWriteLock(
) {
    if (pthread_rwlock_trywrlock(&m_rwlocker) != 0) {
//        STDO_ERRO("RwLockerImpl --- rwlock_trywrlock error [%03d]:<<%s>>", errno, strerror(errno));
        return FALSE;
    }
    return TRUE;
}

/// @fn     UnLock
/// @brief  None
/// @param  None
/// @return None
bool_t
RwLockerImpl::UnLock(
) {
    if (pthread_rwlock_unlock(&m_rwlocker) != 0) {
//        STDO_ERRO("RwLockerImpl --- rwlock_unlock error [%03d]:<<%s>>", errno, strerror(errno));
        return FALSE;
    }
    return TRUE;
}

}
}
