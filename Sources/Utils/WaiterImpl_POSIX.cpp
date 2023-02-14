//
// Created by TrungTQ on 22 20 Mar 2017 16:32:04
//

#include <errno.h>
//#include "Libraries/Log/LogStdo.hpp"
#include "Libraries/Utils/WaiterImpl.hpp"

namespace libX {
namespace utils {

/// @fn     WaiterImpl
/// @brief  None
/// @param  None
/// @return None
WaiterImpl::WaiterImpl(
    bool_t autoReset
) : m_boAuto (autoReset)
  , m_boState (FALSE) {
    pthread_mutex_init(&m_mutex, NULL);
    if (pthread_cond_init(&m_cond, NULL) != 0) {
        pthread_mutex_destroy(&m_mutex);
    }
}

/// @fn     ~WaiterImpl
/// @brief  None
/// @param  None
/// @return None
WaiterImpl::~WaiterImpl(
) {
    pthread_cond_destroy(&m_cond);
    pthread_mutex_destroy(&m_mutex);
}

/// @fn     Set
/// @brief  None
/// @param  None
/// @return None
bool_t
WaiterImpl::Set(
) {
    pthread_mutex_lock(&m_mutex);
    m_boState = TRUE;
    if (pthread_cond_broadcast(&m_cond) != 0) {
//        STDO_ERRO("cond_broadcast error [%03d]:<<%s>>", errno, strerror(errno));
        pthread_mutex_unlock(&m_mutex);
        return FALSE;
    }
    pthread_mutex_unlock(&m_mutex);
    return TRUE;
}

/// @fn     Reset
/// @brief  None
/// @param  None
/// @return None
void
WaiterImpl::Reset(
) {
    pthread_mutex_lock(&m_mutex);
    m_boState = FALSE;
    pthread_mutex_unlock(&m_mutex);
}

/// @fn     Wait
/// @brief  None
/// @param  None
/// @return None
bool_t
WaiterImpl::Wait(
) {
    if (pthread_mutex_lock(&m_mutex) != 0) {
//        STDO_ERRO("mutex_lock error [%03d]:<<%s>>", errno, strerror(errno));
        return FALSE;
    }

    while (!m_boState) {
        if (pthread_cond_wait(&m_cond, &m_mutex)) {
            pthread_mutex_unlock(&m_mutex);
//            STDO_ERRO("cond_wait error [%03d]:<<%s>>", errno, strerror(errno));
            return FALSE;
        }
    }

    if (m_boAuto) {
        m_boState = FALSE;
    }
    pthread_mutex_unlock(&m_mutex);
    return TRUE;
}

/// @fn     Wait
/// @brief  None
/// @param  DWORD dwMiliSecs
/// @return None
bool_t
WaiterImpl::Wait(
    u32_t dwMiliSecs
) {
    struct timeval now;
    struct timespec abstime;

    if (pthread_mutex_lock(&m_mutex) != 0) {
//        STDO_ERRO("mutex_lock error [%03d]:<<%s>>", errno, strerror(errno));
        return FALSE;
    }

    gettimeofday(&now, NULL);

    abstime.tv_sec = now.tv_sec + (dwMiliSecs / 1000);
    now.tv_usec += (dwMiliSecs % 1000) * 1000;

    while (now.tv_usec >= (1000 * 1000)) {
        now.tv_usec -= (1000 * 1000);
        abstime.tv_sec++;
    }
    abstime.tv_nsec = now.tv_usec * 1000;

    int_t iRet = 0;

    while (!m_boState) {
        if ((iRet = pthread_cond_timedwait(&m_cond, &m_mutex, &abstime))) {
            if (iRet == ETIMEDOUT) {
//                STDO_WARN("cond_timedwait timeout");
                break;
            }
            pthread_mutex_unlock(&m_mutex);
//            STDO_ERRO("cond_wait error [%03d]:<<%s>>", errno, strerror(errno));
            return FALSE;
        }
    }

    if ((iRet == 0) && m_boAuto) {
        m_boState = FALSE;
    }
    pthread_mutex_unlock(&m_mutex);
    return (iRet == 0);
}

}
}
