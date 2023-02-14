/// Created by TrungTQ on 9 Mar 2018 22:11:01

#ifndef LIBX_UTILS_UNIQUEPTR_HPP_
#define LIBX_UTILS_UNIQUEPTR_HPP_

#include <memory>

namespace libX {
namespace utils {
template<typename T> using UniquePtr = std::unique_ptr<T>;

}
}

#endif /* LIBX_UTILS_UNIQUEPTR_HPP_ */
