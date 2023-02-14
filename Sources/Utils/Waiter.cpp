//
// Created by TrungTQ on 22 Jan 2017 07:40:04
//

#include <sys/time.h>
#include <pthread.h>
#include <errno.h>

#include "Libraries/Utils/Waiter.hpp"
#include "Libraries/Utils/WaiterImpl.hpp"

namespace libX {
namespace utils {

/// @fn     Waiter
/// @brief  Constructor
/// @param  BOOL boAutoReset
/// @return None
Waiter::Waiter(
    bool_t boAutoReset
) : Core (*new WaiterImpl(boAutoReset)) {
}

/// @fn     Waiter
/// @brief  Constructor
/// @param  IMPL&
/// @retuen None
Waiter::Waiter(
    WaiterImpl& d
) : Core (d) {
}

/// @fn     PtrImpl
/// @brief  None
/// @param  None
/// @return IMPL*
WaiterImpl*
Waiter::PtrImpl(
) {
    return reinterpret_cast<WaiterImpl*>(m_ptrImpl.Data());
}

/// @fn     PtrImpl
/// @brief  None
/// @param  None
/// @return IMPL*
const WaiterImpl*
Waiter::PtrImpl(
) const {
    return reinterpret_cast<const WaiterImpl*>(m_ptrImpl.Data());
}

/// @func   ~Waiter
/// @brief  None
/// @param  None
/// @retval None
Waiter::~Waiter(
) {
}

/// @fn     Set
/// @brief  None
/// @param  None
/// @retval None
bool_t
Waiter::Set(
) {
    auto ptr = PtrImpl();
    return ptr->Set();
}

/// @fn     ReSet
/// @brief  None
/// @param  None
/// @retval None
void
Waiter::Reset(
) {
    auto ptr = PtrImpl();
    ptr->Reset();
}

/// @func   Wait
/// @brief  None
/// @param  None
/// @retval None
bool_t
Waiter::Wait(
) {
    auto ptr = PtrImpl();
    return ptr->Wait();
}

/// @fn     Wait
/// @brief  None
/// @param  DWORD miliSecs
/// @return None
bool_t
Waiter::Wait(
    u32_t miliSecs
) {
    auto ptr = PtrImpl();
    return ptr->Wait(miliSecs);
}

}
}
