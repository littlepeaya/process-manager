//
// Created by TrungTQ on 2 Mar 2017 09:07:50
//

#ifndef LIBX_UTILS_COUNTER_HPP_
#define LIBX_UTILS_COUNTER_HPP_

#include "Libraries/Utils/Typedefs.h"
#include "Locker.hpp"

namespace libX {
namespace utils {

class Counter {
private:
    int_t    m_iCount;
    utils::Locker_t m_locker;
public:
    Counter();
    virtual ~Counter();

    void Incement();
    void Decement();
};

typedef Counter  Counter_t;
typedef Counter* Counter_p;

}
}

#endif /* !LIBX_UTILS_COUNTER_HPP_ */
