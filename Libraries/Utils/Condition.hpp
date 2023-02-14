/// Created by TrungTQ on 21 Mar 2017 15:29:33

#ifndef LIBX_UTILS_CONDITION_HPP_
#define LIBX_UTILS_CONDITION_HPP_

#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Utils/DQueue.hpp"
#include "Libraries/Utils/SharedPtr.hpp"
#include "Waiter.hpp"
#include "Locker.hpp"
#include "AutoLocker.hpp"
#include "AutoUnLocker.hpp"

namespace libX {
namespace utils {

class Condition {
private:
    typedef DQueue<Waiter*> WaitQueue_t;
    WaitQueue_t m_waitQueue;
    Locker_t  m_locker;

    DISABLE_COPY(Condition)
private:
    void EnQueue(Waiter& waiter);
    void DeQueue(Waiter& waiter);
public:
    Condition();
    ~Condition();

    template<class M>
    void Wait(M& mutex);

    template<class M>
    bool_t Wait(M& mutex, u32_t miliSecs);

    void Signal();
    void BroadCast();
};

typedef Condition  Condition_t;
typedef Condition* Condition_p;

/// @fn     Wait
/// @brief  None
/// @param  None
/// @return None
template<class M>
inline void
Condition::Wait(
    M& mutex
) {
    AutoUnLocker<Locker> unlocker(mutex, FALSE);
    Waiter waiter;
    {
        AutoLocker<Locker> locker(m_locker);
        mutex.UnLock();
        Condition::EnQueue(waiter);
    }
    waiter.Wait();
}

/// @fn     Wait
/// @brief  None
/// @param  mutex
/// @param  miliSecs
/// @return None
template<class M>
inline bool_t 
Condition::Wait(
    M& mutex,
    u32_t miliSecs
) {
    AutoUnLocker<Locker> unlocker(mutex, FALSE);
    Waiter waiter;
    {
        AutoLocker<Locker> locker(m_locker);
        mutex.UnLock();
        Condition::EnQueue(waiter);
    }
    
    if (!waiter.Wait(miliSecs)) {
        AutoLocker<Locker> locker(m_locker);
        Condition::DeQueue(waiter);
        return FALSE;
    }
    return TRUE;
}

}
}

#endif /* !LIBX_UTILS_CONDITION_HPP_ */
