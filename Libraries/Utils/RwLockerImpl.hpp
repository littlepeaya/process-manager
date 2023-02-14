//
// Created by TrungTQ on 2017-03-17 14:25:00
//

#ifndef LIBX_UTILS_RWLOCKERIMPL_HPP_
#define LIBX_UTILS_RWLOCKERIMPL_HPP_

#include <pthread.h>
#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Utils/CoreImpl.hpp"
#include "RwLocker.hpp"

namespace libX {
namespace utils {

class RwLockerImpl : public utils::CoreImpl {
private:
    typedef pthread_rwlock_t  rw_locker_t;
    rw_locker_t m_rwlocker;
    
public:
    RwLockerImpl();

    virtual ~RwLockerImpl();

    bool_t ReadLock();
    bool_t TryReadLock();
    bool_t WriteLock();
    bool_t TryWriteLock();
    bool_t UnLock();
};

typedef RwLockerImpl  RwLockerImpl_t;
typedef RwLockerImpl* RwLockerImpl_p;

}
}

#endif /* LIBX_UTILS_RWLOCKERIMPL_HPP_ */
