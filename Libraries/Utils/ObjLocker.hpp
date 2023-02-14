//
// Created by TrungTQ on 1 Mar 2017 23:38:29
//

#ifndef LIBX_UTILS_OBJLOCKER_HPP_
#define LIBX_UTILS_OBJLOCKER_HPP_

#include "Libraries/Utils/Typedefs.h"
#include "Locker.hpp"

namespace libX {
namespace utils {

template<typename Type>
class ObjLocker {
private:
    Type* m_pObj;
    Locker_p m_pLocker;
public:
    ObjLocker(Type& obj, Locker_t& locker);
    virtual ~ObjLocker();
    Type& operator* ();
    Type* operator->();
};

/// @fn     ObjLocker
/// @brief  None
/// @param  None
/// @retval None
template<typename Type>
inline
ObjLocker<Type>::ObjLocker(
    Type& obj,
    Locker_t& locker
) : m_pObj (const_cast<Type*>(&obj)),
    m_pLocker (&locker) {
    m_pLocker->Lock();
}

/// @fn     ~ObjLocker
/// @brief  None
/// @param  None
/// @retval None
template<typename Type>
inline
ObjLocker<Type>::~ObjLocker(
) {
    m_pLocker->UnLock();
}

/// @fn     operator* ()
/// @brief  None
/// @param  None
/// @retval None
template<typename Type>
inline Type&
ObjLocker<Type>::operator* (
) {
    return *m_pObj;
}

/// @fn     operator->()
/// @brief  None
/// @param  None
/// @retval None
template<typename Type>
inline Type*
ObjLocker<Type>::operator->(
) {
    return m_pObj;
}

}
}

#endif /* !LIBX_UTILS_OBJLOCKER_HPP_ */
