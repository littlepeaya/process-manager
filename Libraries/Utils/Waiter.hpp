//
// Created by TrungTQ on 22 Jan 2017 07:40:04
//

#ifndef LIBX_UTILS_WAITER_HPP_
#define LIBX_UTILS_WAITER_HPP_

#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Utils/Core.hpp"
#include "Libraries/Utils/ScopedPtr.hpp"

namespace libX {
namespace utils {

class WaiterImpl;

class Waiter : public utils::Core {
private:
    DISABLE_COPY(Waiter)

    /// @fn     PtrImpl
    /// @brief  None
    /// @param  None
    /// @return IMPL*
    WaiterImpl* PtrImpl();

    /// @fn     PtrImpl
    /// @brief  None
    /// @param  None
    /// @return IMPL*
    const WaiterImpl* PtrImpl() const;

protected:
    /// @fn     Waiter
    /// @brief  Constructor
    /// @param  IMPL&
    /// @return None
    Waiter(WaiterImpl& d);

public:
    /// @fn     Waiter
    /// @brief  Constructor
    /// @param  BOOL boAutoReset
    /// @return None
    Waiter(bool_t boAutoReset = TRUE);

    /// @fn     ~Waiter
    /// @brief  Destructor
    /// @param  None
    /// @return None
    virtual ~Waiter();

    /// @fn     Set
    /// @brief  None
    /// @param  None
    /// @return None
    bool_t Set();

    /// @fn     Reset
    /// @brief  None
    /// @param  None
    /// @return None
    void Reset();

    /// @fn     Wait
    /// @brief  None
    /// @param  None
    /// @return None
    bool_t Wait();

    /// @fn     Wait
    /// @brief  None
    /// @param  DWORD miliSecs
    /// @return None
    bool_t Wait(u32_t miliSecs);
};

typedef Waiter  Waiter_t;
typedef Waiter* Waiter_p;

}
}

#endif // !LIBX_UTILS_WAITER_HPP_
