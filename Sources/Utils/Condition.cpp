/// Created by TrungTQ on 21 Mar 2017 15:30:11

#include "Libraries/Utils/Condition.hpp"

namespace libX {
namespace utils {

/// @fn     Condition
/// @brief  None
/// @param  None
/// @retval None
Condition:: Condition(
) {
}

/// @fn     ~Condition
/// @brief  None
/// @param  None
/// @retval None
Condition::~Condition(
) {
}

/// @fn     EnQueue
/// @brief  None
/// @param  None
/// @retval None
void
Condition::EnQueue(
    Waiter& waiter
) {
    m_waitQueue.push_back(&waiter);
}

/// @fn     DeQueue
/// @brief  None
/// @param  None
/// @retval None
void
Condition::DeQueue(
    Waiter& waiter
) {
    for (auto it = m_waitQueue.begin(); it != m_waitQueue.end(); it++) {
        if (*it == &waiter) {
            m_waitQueue.erase(it);
            break;
        }
    }
}

/// @fn     Signal
/// @brief  None
/// @param  None
/// @retval None
void
Condition::Signal(
) {
    AutoLocker<Locker> locker(m_locker);
    if (!m_waitQueue.empty()) {
        m_waitQueue.front()->Set();
        m_waitQueue.pop_front();
    }
}

/// @fn     BroadCast
/// @brief  None
/// @param  None
/// @retval None
void
Condition::BroadCast(
) {
    AutoLocker<Locker> locker(m_locker);
    for (auto it = m_waitQueue.begin(); it != m_waitQueue.end(); it++) {
        (*it)->Set();
    }
    m_waitQueue.clear();
}

}
}
