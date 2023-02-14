//
// Created by TrungTQ on 21 Mar 2017 15:48:19.
//

#ifndef LIBX_UTILS_AUTOUNLOCKER_HPP_
#define LIBX_UTILS_AUTOUNLOCKER_HPP_

#include "Libraries/Utils/Typedefs.h"
#include "Locker.hpp"

namespace libX {
namespace utils {

template<class M>
class AutoUnLocker {
private:
    M& m_locker;
    DISABLE_COPY(AutoUnLocker)
public:
    AutoUnLocker(M& locker, bool_t boUnLockNow = FALSE);
    virtual ~AutoUnLocker();
};

/// @fn     AutoUnLocker
/// @brief  None
/// @param  None
/// @return None
template<class M>
inline
AutoUnLocker<M>::AutoUnLocker(
    M& locker,
    bool_t boUnLockNow
) : m_locker (locker) {
    if (boUnLockNow) {
        m_locker.UnLock();
    }
}

/// @fn     ~AutoLocker
/// @brief  None
/// @param  None
/// @return None
template<class M>
inline
AutoUnLocker<M>::~AutoUnLocker(
) {
    m_locker.Lock();
}

}
}

#endif /* !LIBX_UTILS_AUTOUNLOCKER_HPP_ */
