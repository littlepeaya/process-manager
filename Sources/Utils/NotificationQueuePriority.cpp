//
// Created by TrungTQ on 29/09/17.
//

#include "Libraries/Utils/AutoLocker.hpp"
#include "Libraries/Utils/Singleton.hpp"
#include "Libraries/Utils/NotificationQueuePriority.hpp"

namespace libX {
namespace utils {

NotificationQueuePriority::NotificationQueuePriority() {}

NotificationQueuePriority::~NotificationQueuePriority() {
    Clear();
}

void
NotificationQueuePriority::Enqueue(
    Notification::Ptr_t pNotification,
    int_t iPriority
) {
    assert(pNotification != NULL);
    AutoLocker<Locker> locker(m_locker);
    if (m_waitQueue.empty()) {
        m_nfQueue.insert(NfQueue_t::value_type(iPriority, pNotification));
    } else {
        WaitInfo* pWaitInfo = m_waitQueue.front();
        m_waitQueue.pop_front();
        pWaitInfo->pNf = pNotification;
        pWaitInfo->nfWaiter.Set();
    }
}

Notification*
NotificationQueuePriority::Dequeue(
) {
    AutoLocker<Locker> locker(m_locker);
    return DequeueOne().Duplicate();
}

Notification*
NotificationQueuePriority::WaitDequeue(
) {
    Notification::Ptr_t pNotification;
    WaitInfo* pWaitInfo = NULL;
    {
        AutoLocker<Locker> locker(m_locker);
        pNotification = DequeueOne();
        if (pNotification != NULL) { return pNotification.Duplicate(); }
        pWaitInfo = new WaitInfo();
        m_waitQueue.push_back(pWaitInfo);
    }
    pWaitInfo->nfWaiter.Wait();
    pNotification = pWaitInfo->pNf;
    delete pWaitInfo;
    return pNotification.Duplicate();
}

Notification*
NotificationQueuePriority::WaitDequeue(
    u32_t dwMiliSecs
) {
    Notification::Ptr_t pNotification;
    WaitInfo* pWaitInfo = NULL;
    {
        AutoLocker<Locker> locker(m_locker);
        pNotification = DequeueOne();
        if (pNotification != NULL) { return pNotification.Duplicate(); }
        pWaitInfo = new WaitInfo();
        m_waitQueue.push_back(pWaitInfo);
    }
    if (pWaitInfo->nfWaiter.Wait(dwMiliSecs)) {
        pNotification = pWaitInfo->pNf;
    } else {
        AutoLocker<Locker> locker(m_locker);
        pNotification = pWaitInfo->pNf;
        for (auto it = m_waitQueue.begin(); it != m_waitQueue.end();) {
            if (*it == pWaitInfo) {
                m_waitQueue.erase(it);
                break;
            }
        }
    }
    delete pWaitInfo;
    return pNotification.Duplicate();
}

void
NotificationQueuePriority::WakeUpAll(
) {
    AutoLocker<Locker> locker(m_locker);
    for (auto it = m_waitQueue.begin(); it != m_waitQueue.end(); ) {
        (*it)->nfWaiter.Set();
    }
    m_waitQueue.clear();
}

bool_t
NotificationQueuePriority::Empty(
) const {
    AutoLocker<Locker> locker(m_locker);
    return m_nfQueue.empty();
}

int_t
NotificationQueuePriority::Size(
) const {
    AutoLocker<Locker> locker(m_locker);
    return static_cast<int_t>(m_nfQueue.size());
}

void
NotificationQueuePriority::Clear(
) {
    AutoLocker<Locker> locker(m_locker);
    m_nfQueue.clear();
}

namespace  {
    static Singleton<NotificationQueuePriority> sh;
}


NotificationQueuePriority&
NotificationQueuePriority::Default() {
    return *sh.Get();
}

Notification::Ptr_t
NotificationQueuePriority::DequeueOne(
) {
    Notification::Ptr_t pNf;
    auto it = m_nfQueue.begin();
    if (it != m_nfQueue.end()) {
        pNf = it->second;
        m_nfQueue.erase(it);
    }
    return pNf;
}}}