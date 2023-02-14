//
// Created by TrungTQ on 2017-03-17 14:25:00
//

#ifndef LIBX_UTILS_RWLOCKER_HPP_
#define LIBX_UTILS_RWLOCKER_HPP_

#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Utils/Core.hpp"
#include "Libraries/Utils/ScopedPtr.hpp"

namespace libX {
namespace utils {

class RwLockerImpl;

class RwLocker : public utils::Core {
protected:

    /// @fn     PtrImpl
    /// @brief  None
    /// @param  None
    /// @return IMPL*
    RwLockerImpl* PtrImpl();

    /// @fn     PtrImpl
    /// @brief  None
    /// @param  None
    /// @return IMPL*
    const RwLockerImpl* PtrImpl() const;
    
    RwLocker(RwLockerImpl& d);

private:
    DISABLE_COPY(RwLocker)

public:
    RwLocker();

    virtual ~RwLocker();

    bool_t ReadLock();
    bool_t TryReadLock();
    bool_t WriteLock();
    bool_t TryWriteLock();
    bool_t UnLock();
};

typedef RwLocker  RwLocker_t;
typedef RwLocker* RwLocker_p;

}
}

#endif /* !LIBX_UTILS_RWLOCKER_HPP_ */
