//
// Created by TrungTQ on 2 Mar 2017 09:13:48
//

#include "Libraries/Utils/Counter.hpp"
#include "Libraries/Utils/ObjLocker.hpp"

namespace libX {
namespace utils {

/// @func   Counter
/// @brief  None
/// @param  None
/// @retval None
Counter::Counter(
) : m_iCount (0) {

}

/// @func   ~Counter
/// @brief  None
/// @param  None
/// @retval None
Counter::~Counter() {

}

/// @func   Incement
/// @brief  None
/// @param  None
/// @retval None
void
Counter::Incement() {
    ++*ObjLocker<int_t>(m_iCount, m_locker);
}

/// @func   Decement
/// @brief  None
/// @param  None
/// @retval None
void
Counter::Decement() {
    --*ObjLocker<int_t>(m_iCount, m_locker);
}

}
}
