//
// Created by TrungTQ on 9 Mar 2017 06:26:48
//

#ifndef LIBX_UTILS_SINGLETON_HPP_
#define LIBX_UTILS_SINGLETON_HPP_

#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Utils/SharedPtr.hpp"
#include "Locker.hpp"
#include "AutoLocker.hpp"

namespace libX {
namespace utils {

template<class C>
class Singleton {
private:
    C* m_ptr;
    utils::Locker_t m_locker;
public:
    Singleton();
    ~Singleton();
    C* Get();
    C* Swap(C* newPtr);
    void Destroy();
};

/// @fn     Singleton
/// @brief  None
/// @param  None
/// @return None
template<class C>
inline
Singleton<C>::Singleton(
) : m_ptr (NULL) {
}

/// @fn     ~Singleton
/// @brief  None
/// @param  None
/// @return None
template<class C>
inline
Singleton<C>::~Singleton(
) {
}

/// @fn     Download
/// @brief  None
/// @param  None
/// @return None
template<class C>
inline C*
Singleton<C>::Get(
) {
    utils::AutoLocker<utils::Locker> locker(m_locker);
    if (m_ptr == NULL) {
        m_ptr = new C();
    }
    return m_ptr;
}

/// @fn     Swap
/// @brief  None
/// @param  None
/// @return None
template<class C>
inline C*
Singleton<C>::Swap(
    C* newPtr
) {
    utils::AutoLocker<utils::Locker> locker(m_locker);
    C* oldPtr = m_ptr;
    m_ptr = newPtr;
    return oldPtr;
}

/// @fn     Destroy
/// @brief  None
/// @param  None
/// @return None
template<class C>
inline void
Singleton<C>::Destroy(
) {
    utils::AutoLocker<utils::Locker> locker(m_locker);
    if (m_ptr!= NULL) {
        delete m_ptr;
    }
    m_ptr = NULL;
}

}
}

#endif /* !LIBX_SINGLETON_HPP_ */
