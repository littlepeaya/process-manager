//
// Created by TrungTQ on 29/09/17.
//

#ifndef LIBX_UTILS_NOTIFICATIONQUEUE_HPP_
#define LIBX_UTILS_NOTIFICATIONQUEUE_HPP_

#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Utils/DQueue.hpp"
#include "Libraries/Utils/Waiter.hpp"
#include "Libraries/Utils/Locker.hpp"
#include "Notification.hpp"

namespace libX {
namespace utils {

class NotificationQueue {
public:
    /// @fn     NotificationQueue
    /// @brief  Constructor
    /// @param  None
    /// @return None
    NotificationQueue();

    /// @fn     ~NotificationQueue
    /// @brief  Destructor
    /// @param  None
    /// @return None
    ~NotificationQueue();

    /// @fn     Enqueue
    /// @brief  None
    /// @param  pNotification
    /// @return None
    void Enqueue(Notification::Ptr_t pNotification);

    /// @fn     EnqueueUrgent
    /// @brief  None
    /// @param  pNotification
    /// @return None
    void EnqueueUrgent(Notification::Ptr_t pNotification);

    /// @fn     Dequeue
    /// @brief  None
    /// @param  None
    /// @return None
    Notification* Dequeue();

    /// @fn     WaitDequeue
    /// @brief  None
    /// @param  None
    /// @return None
    Notification* WaitDequeue();

    /// @fn     WaitDequeue
    /// @brief  None
    /// @param  DWORD dwMiliSecs
    /// @return None
    Notification* WaitDequeue(u32_t miliSecs);

    /// @fn     WakeUpAll
    /// @brief  None
    /// @param  None
    /// @return None
    void WakeUpAll();

    /// @fn     Empty
    /// @brief  None
    /// @param  None
    /// @return None
    bool_t Empty() const;

    /// @fn     Size
    /// @brief  None
    /// @param  None
    /// @return None
    int_t Size() const;

    /// @fn     Clear
    /// @brief  None
    /// @param  None
    /// @return None
    void Clear();

    /// @fn     HasIdleThreads
    /// @brief  None
    /// @param  None
    /// @return None
    bool_t HasIdleThreads() const;

    /// @fn     Default
    /// @brief  None
    /// @param  None
    /// @return None
    static NotificationQueue& Default();

protected:
    Notification::Ptr_t DequeueOne();
private:
    typedef DQueue<Notification::Ptr_t> NfQueue_t;
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

#endif // LIBX_NOTIFICATION_QUEUE_HPP_
