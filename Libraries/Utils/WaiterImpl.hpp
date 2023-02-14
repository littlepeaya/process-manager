//
// Created by TrungTQ on 20 Mar 2017 15:32:46
//

#ifndef LIBX_UTILS_WAITERIMPL_HPP_
#define LIBX_UTILS_WAITERIMPL_HPP_

#include <pthread.h>
#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Utils/CoreImpl.hpp"
#include "Waiter.hpp"

namespace libX {
namespace utils {

class WaiterImpl : public utils::CoreImpl {
private:
    bool_t          m_boAuto;
    volatile bool_t m_boState;
    pthread_mutex_t m_mutex;
    pthread_cond_t  m_cond;

public:
    WaiterImpl(bool_t autoReset);

    virtual ~WaiterImpl();

    bool_t Set();

    void Reset();

    bool_t Wait();

    bool_t Wait(u32_t miliSecond);
};

typedef WaiterImpl  WaiterImpl_t;
typedef WaiterImpl* WaiterImpl_p;

}
}

#endif /* !LIBX_UTILS_WAITERIMPL_HPP_ */
