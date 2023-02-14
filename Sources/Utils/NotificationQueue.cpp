//
// Created by TrungTQ on 29/09/17.
//

#include "Libraries/Utils/Singleton.hpp"
#include "Libraries/Utils/Converter.hpp"
#include "Libraries/Utils/AutoLocker.hpp"
#include "Libraries/Utils/NotificationQueue.hpp"

namespace libX {
namespace utils {

NotificationQueue::NotificationQueue() {}

NotificationQueue::~NotificationQueue() {
    Clear();
}

void
NotificationQueue::Enqueue(
        Notification::Ptr_t pNotification
) {
    assert(pNotification != NULL);
    AutoLocker<Locker> locker(m_locker);
    if (m_waitQueue.empty()) {
        m_nfQueue.push_back(pNotification);
    } else {
        WaitInfo *pWaitInfo = m_waitQueue.front();
        m_waitQueue.pop_front();
        pWaitInfo->pNf = pNotification;
        pWaitInfo->nfWaiter.Set();
    }
}

void
NotificationQueue::EnqueueUrgent(
        Notification::Ptr_t pNotification
) {
    assert(pNotification != NULL);
    AutoLocker<Locker> locker(m_locker);
    if (m_waitQueue.empty()) {
        m_nfQueue.push_front(pNotification);
    } else {
        WaitInfo *pWaitInfo = m_waitQueue.front();
        m_waitQueue.pop_front();
        pWaitInfo->pNf = pNotification;
        pWaitInfo->nfWaiter.Set();
    }
}

Notification *
NotificationQueue::Dequeue(
) {
    AutoLocker<Locker> locker(m_locker);
    return DequeueOne().Duplicate();
}

Notification *
NotificationQueue::WaitDequeue(
) {
    Notification::Ptr_t pNotification;
    WaitInfo *pWaitInfo = NULL;
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

Notification *
NotificationQueue::WaitDequeue(
        u32_t miliSecs
) {
    Notification::Ptr_t pNotification;
    WaitInfo *pWaitInfo = NULL;
    {
        AutoLocker<Locker> locker(m_locker);
        pNotification = DequeueOne();
        if (pNotification != NULL) { return pNotification.Duplicate(); }
        pWaitInfo = new WaitInfo();
        m_waitQueue.push_back(pWaitInfo);
    }
    if (pWaitInfo->nfWaiter.Wait(miliSecs)) {
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
NotificationQueue::WakeUpAll(
) {
    AutoLocker<Locker> locker(m_locker);
    for (auto it = m_waitQueue.begin(); it != m_waitQueue.end();) {
        (*it)->nfWaiter.Set();
    }
    m_waitQueue.clear();
}

bool_t
NotificationQueue::Empty(
) const {
    AutoLocker<Locker> locker(m_locker);
    return Converter::ToByte(m_nfQueue.empty());
}

int_t
NotificationQueue::Size(
) const {
    AutoLocker<Locker> locker(m_locker);
    return static_cast<int_t>(m_nfQueue.size());
}

void
NotificationQueue::Clear(
) {
    AutoLocker<Locker> locker(m_locker);
    m_nfQueue.clear();
}

bool_t
NotificationQueue::HasIdleThreads(
) const {
    AutoLocker<Locker> locker(m_locker);
    return Converter::ToByte(!m_waitQueue.empty());
}

namespace {
    static Singleton<NotificationQueue> sh;
}


NotificationQueue &
NotificationQueue::Default(
) {
    return *sh.Get();
}

Notification::Ptr_t
NotificationQueue::DequeueOne(
) {
    Notification::Ptr_t pNf;
    if (!m_nfQueue.empty()) {
        pNf = m_nfQueue.front();
        m_nfQueue.pop_front();
    }
    return pNf;
}

}}
