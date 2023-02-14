//
// Created by TrungTQ on 07/08/17.
//

#ifndef LIBX_UTILS_LIST_HPP_
#define LIBX_UTILS_LIST_HPP_

#include <list>
#include "Libraries/Utils/Vector.hpp"
#include "Libraries/Values/ValueArray.hpp"

namespace libX {
namespace utils {

template<typename T> using List = std::list<T>;

template<typename T>
inline ValueArray<T>
ToArray(
    List<T>& input
) {
    Vector<T> v {std::begin(input), std::end(input)};
    return ValueArray<T>(v);
}

template<typename T>
inline void
AddRange(
    List<T>& input,
    Vector<T>& data
) {
    input.insert(input.end(), data.begin(), data.end());
}

template<typename T>
inline void
AddRange(
    List<T>& input,
    List<T>& data
) {
    input.insert(input.end(), data.begin(), data.end());
}

}
}

#endif //LIST_HPP_
