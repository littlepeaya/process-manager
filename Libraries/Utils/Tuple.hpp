//
// Created by TrungTQ on 14 Dec 2017 10:03:59
//

#ifndef LIBX_UTILS_TUPLE_HPP_
#define LIBX_UTILS_TUPLE_HPP_

#include <tuple>

namespace libX {
namespace utils {

template<typename... T> using Tuple = std::tuple<T>;

}
}

#endif /* TUPLE_HPP_ */
