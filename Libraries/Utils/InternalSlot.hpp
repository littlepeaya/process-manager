//
// Created by TrungTQ on 1 Mar 2017 15:08:27
//

#ifndef LIBX_SIGNAL_INTERNALSLOT_HPP_
#define LIBX_SIGNAL_INTERNALSLOT_HPP_

#include <atomic>
#include "Libraries/Utils/Typedefs.h"
#include "Delegate.hpp"

namespace libX {
namespace utils {

template<typename RT, typename... Args>
struct InternalSlot {
    IDelegate<RT, Args...>* m_pDelegate;
    void_p m_pInstance;
    i32_t  m_id;
    i32_t  m_iPriority;
    std::atomic_flag m_flag = ATOMIC_FLAG_INIT;

    InternalSlot(
         IDelegate<RT, Args...>* pDelegate,
         void_p pInstance = NULL,
         i32_t id = -1,
         i32_t priority = -1);

    ~InternalSlot();

    void Kill();

    bool_t Alive();

    /// NonCopyable and NonMovable
    DISABLE_COPY(InternalSlot)
};

/// @fn     InternalSlot
/// @brief  None
/// @param  None
/// @retval None
template<typename RT, typename... Args>
inline
InternalSlot<RT, Args...>::InternalSlot(
    IDelegate<RT, Args...>* pDelegate,
    void_p pInstance,
    i32_t id,
    i32_t priority
) : m_pDelegate (pDelegate)
  , m_pInstance (pInstance)
  , m_id (id)
  , m_iPriority (priority) {
    m_flag.test_and_set();
}

/// @fn     ~Slot
/// @brief  None
/// @param  None
/// @retval None
template<typename RT, typename... Args>
inline
InternalSlot<RT, Args...>::~InternalSlot(
) {
}

/// @fn     Kill
/// @brief  None
/// @param  None
/// @retval None
template<typename RT, typename... Args>
inline void
InternalSlot<RT, Args...>::Kill(
) {
    m_flag.clear();
}

/// @fn     Alive
/// @brief  None
/// @param  None
/// @retval None
template<typename RT, typename... Args>
inline bool_t
InternalSlot<RT, Args...>::Alive(
) {
    return m_flag.test_and_set();
}

}
}

#endif /* !LIBX_SIGNAL_INTERNALSLOT_HPP_ */
