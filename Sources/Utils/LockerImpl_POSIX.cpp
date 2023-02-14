//
// Created by TrungTQ on 17 Mar 2017 14:40:00
//

#include <errno.h>
//#include "Libraries/Log/LogStdo.hpp"
#include "Libraries/Utils/LockerImpl.hpp"

namespace libX {
namespace utils {

/// @fn     LockerImpl
/// @brief  None
/// @param  None
/// @retval None
LockerImpl::LockerImpl(
) {
    if (pthread_mutex_init(&m_locker_impl, NULL) != 0) {
//        STDO_ERRO("LockerImpl --- mutex_init error [%03d]:<<%s>>", errno, strerror(errno));
    }
}

/// @fn     ~LockerImpl
/// @brief  None
/// @param  None
/// @retval None
LockerImpl::~LockerImpl(
) {
    if (pthread_mutex_destroy(&m_locker_impl) != 0) {
//        STDO_ERRO("LockerImpl --- mutex_destroy error [%03d]:<<%s>>", errno, strerror(errno));
    }
}

/// @fn     Lock
/// @brief  None
/// @param  None
/// @retval None
bool_t 
LockerImpl::Lock(
) {
    if (pthread_mutex_lock(&m_locker_impl) != 0) {
//        STDO_ERRO("LockerImpl --- mutex_lock error [%03d]:<<%s>>", errno, strerror(errno));
        return FALSE;
    }
    return TRUE;
}

/// @fn     UnLock
/// @brief  None
/// @param  None
/// @retval None
bool_t 
LockerImpl::UnLock(
) {
    if (pthread_mutex_unlock(&m_locker_impl) != 0) {
//        STDO_ERRO("LockerImpl --- mutex_unlock error [%03d]:<<%s>>", errno, strerror(errno));
        return FALSE;
    }
    return TRUE;
}

/// @fn     TryLock
/// @brief  None
/// @param  None
/// @retval None
bool_t 
LockerImpl::TryLock(
) {
    if (pthread_mutex_trylock(&m_locker_impl) != 0) {
//        STDO_ERRO("LockerImpl --- mutex_trylock error [%03d]:<<%s>>", errno, strerror(errno));
        return FALSE;
    }
    return TRUE;
}

/// @fn     TryLock
/// @brief  None
/// @param  DWORD miliSecs
/// @retval None
bool_t 
LockerImpl::TryLock(
    u32_t miliSecs
) {
    struct timeval  now;
    struct timespec abstime;

    gettimeofday(&now, NULL);
    abstime.tv_sec = now.tv_sec + (miliSecs / 1000);
    now.tv_usec += (miliSecs % 1000) * 1000;

    while (now.tv_usec >= (1000 * 1000)) {
        now.tv_usec -= (1000 * 1000);
        abstime.tv_sec++;
    }

    if (pthread_mutex_timedlock(&m_locker_impl, &abstime) != 0) {
//        STDO_ERRO("LockerImpl --- mutex_timedlock error [%03d]:<<%s>>", errno, strerror(errno));
        return FALSE;
    }
    return TRUE;
}

/// @fn     IsLocked
/// @brief  None
/// @param  None
/// @retval None
bool_t
LockerImpl::IsLocked(
) {
    if (LockerImpl::TryLock() !=  TRUE) {
        return TRUE;
    } else {
        LockerImpl::UnLock();
        return FALSE;
    }
}

}
}
