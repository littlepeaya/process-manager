//
// Created by TrungTQ on 18 Feb 2017 16:25:43.
//

#ifndef LIBX_UTILS_LOCKER_HPP_
#define LIBX_UTILS_LOCKER_HPP_

#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Utils/Core.hpp"
#include "Libraries/Utils/ScopedPtr.hpp"

namespace libX {
namespace utils {

class LockerImpl;

class Locker : public utils::Core {
private:
    /// @fn     PtrImpl
    /// @brief  None
    /// @param  None
    /// @return IMPL*
    LockerImpl* PtrImpl();

    /// @fn     PtrImpl
    /// @brief  None
    /// @param  None
    /// @return IMPL*
    const LockerImpl* PtrImpl() const;

    DISABLE_COPY(Locker)

protected:
    Locker(LockerImpl& d);

public:
    Locker();
    
    virtual ~Locker();

    bool_t Lock();
    bool_t UnLock();
    bool_t TryLock();
    bool_t TryLock(u32_t dwMiliSecs);
    bool_t IsLocked();

    LockerImpl* GetPtr();

    const LockerImpl* GetPtr() const;
};

typedef Locker  Locker_t;
typedef Locker* Locker_p;

}
}

#endif /* !LIBX_SYNCTHREAD_LOCKER_HPP_ */
