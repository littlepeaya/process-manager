//
// Created by TrungTQ on 07/08/17.
//

#ifndef LIBX_UTILS_VECTOR_HPP_
#define LIBX_UTILS_VECTOR_HPP_

#include <vector>
#include "Libraries/Utils/Typedefs.h"

namespace libX {
namespace utils {

template<typename T> using Vector = std::vector<T>;

template<typename T>
inline void
AddRange(
    Vector<T>& input,
    Vector<T>& data
) {
    input.insert(input.end(), data.begin(), data.end());
}

template<typename T>
inline void
AddRange(
    Vector<T>& input,
    const Vector<T>& data
) {
    input.insert(input.end(), data.begin(), data.end());
}

#define MAKE_TYPE_VECTOR(Type, TypeSuffix)       \
typedef Vector<Type>   v##TypeSuffix##_t;

MAKE_TYPE_VECTOR(u8_t,   u8)
MAKE_TYPE_VECTOR(i8_t,   i8)
MAKE_TYPE_VECTOR(u16_t, u16)
MAKE_TYPE_VECTOR(i16_t, i16)
MAKE_TYPE_VECTOR(u32_t, u32)
MAKE_TYPE_VECTOR(i32_t, i32)

#undef MAKE_TYPE_VECTOR

}
}

#endif // !LIBX_UTILS_VECTOR_HPP_
