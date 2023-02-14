//
// Created by TrungTQ on 21 Oct 2017 23:30:22
//

#ifndef LIBX_UTILS_RUNNABLE_HPP_
#define LIBX_UTILS_RUNNABLE_HPP_

#include "Libraries/Utils/Typedefs.h"

namespace libX {
namespace utils {

class Runnable {
private:
    DISABLE_COPY(Runnable)
public:
    Runnable();
    virtual ~Runnable();
    virtual void_p Run(void_p);
};

}
}

#endif /* LIBX_UTILS_RUNNABLE_HPP_ */
