//
// Created by TrungTQ on 17 Mar 2017 14:35:00
//

#ifndef LIBX_UTILS_LOCKERIMPL_HPP_
#define LIBX_UTILS_LOCKERIMPL_HPP_

#include <pthread.h>
#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Utils/CoreImpl.hpp"
#include "Locker.hpp"

namespace libX {
namespace utils {

class LockerImpl : public utils::CoreImpl {
public:
    typedef pthread_mutex_t locker_impl_t;

    locker_impl_t m_locker_impl;

    LockerImpl();

    virtual ~LockerImpl();

    bool_t Lock();
    bool_t UnLock();
    bool_t TryLock();
    bool_t TryLock(u32_t dwMiliSecs);
    bool_t IsLocked();
};

typedef LockerImpl  LockerImpl_t;
typedef LockerImpl* LockerImpl_p;

}
}

#endif /* !LIBX_UTILS_LOCKERIMPL_HPP_ */
