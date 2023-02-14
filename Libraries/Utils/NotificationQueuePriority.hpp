//
// Created by TrungTQ on 29/09/17.
//

#ifndef LIBX_UTILS_NOTIFICATIONQUEUEPRIORITY_HPP_
#define LIBX_UTILS_NOTIFICATIONQUEUEPRIORITY_HPP_

#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Utils/DQueue.hpp"
#include "Libraries/Utils/Locker.hpp"
#include "Libraries/Utils/Waiter.hpp"
#include "Notification.hpp"

namespace libX {
namespace utils {

class NotificationQueuePriority {
public:
    NotificationQueuePriority();

    ~NotificationQueuePriority();

    void Enqueue(Notification::Ptr_t pNotification, int_t iPriority);

    Notification* Dequeue();

    Notification* WaitDequeue();

    Notification* WaitDequeue(u32_t dwMiliSecs);

    void WakeUpAll();

    bool_t Empty() const;

    int_t Size() const;

    void Clear();

    static NotificationQueuePriority& Default();

protected:
    Notification::Ptr_t DequeueOne();
private:
    typedef std::multimap<int_t, Notification::Ptr_t> NfQueue_t;
    struct WaitInfo {
        Notification::Ptr_t pNf;
        Waiter_t nfWaiter;
    };
    typedef DQueue<WaitInfo*> WaitQueue_t;
    NfQueue_t m_nfQueue;
    WaitQueue_t m_waitQueue;
    mutable Locker_t m_locker;
};

}
}

#endif // LIBX_UTILS_NOTIFICATIONQUEUEPRIORITY_HPP_
