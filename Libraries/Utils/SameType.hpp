/// Lumi, JSC. Created by TrungTQ on 7 Mar 2017 16:25:45

#ifndef LIBX_UTILS_SAMETYPE_HPP_
#define LIBX_UTILS_SAMETYPE_HPP_

#include "Libraries/Utils/Typedefs.h"

namespace libX {
namespace utils {

template<class T1, class T2>
struct SameType {
    static const bool_t value = FALSE;
};

template<class T>
struct SameType<T, T> {
    static const bool_t value = TRUE;
};

}
}

#endif /* !LIBX_UTILS_SAMETYPE_HPP_ */
