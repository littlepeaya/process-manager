//
// Created by TrungTQ on 22 Jan 2017 07:40:04
//

#include <typeinfo>
#include "Libraries/Utils/Core.hpp"
#include "Libraries/Utils/CoreImpl.hpp"

namespace libX {
namespace utils {

/// @fn     Core
/// @brief  Contructor
/// @param  None
/// @retval None
Core::Core(
) : m_ptrImpl (new CoreImpl) {
    m_ptrImpl->m_ptr = this;
}

/// @fn     Core
/// @brief  Constructor
/// @param  COREIMPL&
/// @retuen None
Core::Core(
    CoreImpl& d
) : m_ptrImpl (&d) {
    m_ptrImpl->m_ptr = this;
}

/// @fn     Core
/// @brief  Constructor
/// @param  COREIMPL* ptr
/// @return None
Core::Core(
    CoreImpl* ptr
) : m_ptrImpl (ptr) {
    if (!m_ptrImpl.IsNull())
        m_ptrImpl->m_ptr = this;
}

/// @fn     ~Core
/// @brief  Destructor
/// @param  None
/// @retval None
Core::~Core(
) {
}

/// @fn     PtrImpl
/// @brief  None
/// @param  None
/// @return IMPL*
CoreImpl*
Core::PtrImpl(
) {
    return reinterpret_cast<CoreImpl*>(m_ptrImpl.Data());
}

/// @fn     PtrImpl
/// @brief  None
/// @param  None
/// @return IMPL*
const CoreImpl*
Core::PtrImpl(
) const {
    return reinterpret_cast<const CoreImpl*>(m_ptrImpl.Data());
}

/// @fn     Index
/// @brief  None
/// @param  None
/// @retval None
i32_t
Core::Index(
) const {
    return PtrImpl()->Index();
}

/// @fn     NbrObj
/// @brief  None
/// @param  None
/// @retval None
i32_t
Core::NbrObj(
) {
    return CoreImpl::NbrObj();
}

/// @fn     ObjName
/// @brief  None
/// @param  None
/// @retval None
String
Core::ObjName(
) const {
    return typeid(*this).name();
}

}
}
