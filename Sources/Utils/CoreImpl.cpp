//
// Created by TrungTQ on 09/08/17.
//

#include "Libraries/Utils/CoreImpl.hpp"

namespace libX {
namespace utils {

AtomicCounter CoreImpl::m_counter = 0;

/// @fn     CoreImpl
/// @brief  Constructor
/// @param  None
/// @retuen None
CoreImpl::CoreImpl(
) : m_ptr (nullptr) {
    m_counter++;
}

/// @fn     ~CoreImpl
/// @brief  Destructor
/// @param  None
/// @retuen None
CoreImpl::~CoreImpl(
) {
    m_counter--;
}

/// @fn     Index()
/// @brief  Index of object
/// @param  None
/// @return IDWORD index of object
i32_t
CoreImpl::Index(
) const {
    return 0;
}

/// @fn     NbrObj
/// @brief  None
/// @param  None
/// @return IDWORD number of object
i32_t
CoreImpl::NbrObj(
) {
    return (i32_t) m_counter;
}

}
}
