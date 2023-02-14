//
// Created by TrungTQ on 1 Mar 2017 16:29:39.
//

#ifndef LIBX_UTILS_AUTOLOCKER_HPP_
#define LIBX_UTILS_AUTOLOCKER_HPP_

#include "Libraries/Utils/Typedefs.h"
#include "Locker.hpp"

namespace libX {
namespace utils {

template<class M>
class AutoLocker {
private:
    M& m_locker;
    DISABLE_COPY(AutoLocker)
public:
    AutoLocker(M& locker);
    AutoLocker(M& locker, u32_t miliSecond);
    virtual ~AutoLocker();
};

/// @fn     AutoLocker
/// @brief  None
/// @param  None
/// @return None
template<class M>
inline
AutoLocker<M>::AutoLocker(
    M& locker
) : m_locker (locker) {
    m_locker.Lock();
}

/// @fn     AutoLocker
/// @brief  None
/// @param  None
/// @return None
template<class M>
inline
AutoLocker<M>::AutoLocker(
    M& locker, 
    u32_t miliSecond
) : m_locker (locker){
    m_locker.Lock(miliSecond);
}

/// @fn     ~AutoLocker
/// @brief  None
/// @param  None
/// @return None
template<class M>
inline
AutoLocker<M>::~AutoLocker(
) {
    m_locker.UnLock();
}

}
}

#endif /* !LIBX_UTILS_AUTOLOCKER_HPP_ */
