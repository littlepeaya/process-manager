/// Created by TrungTQ on 12/07/17 10:16

#ifndef LIBX_VAL_VALUEDEFAULT_HPP_
#define LIBX_VAL_VALUEDEFAULT_HPP_

#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Utils/String.hpp"

namespace libX {
namespace val {

template<typename T>
struct ValueDefault {
    static T Default() { return 0; }
};

template<>
struct ValueDefault<String> {
    static String Default() { return String(); }
};

}
}

#endif // !LIBX_VAL_VALUEDEFAULT_HPP_
