//
// Created by TrungTQ on 07/08/17.
//

#ifndef LIBX_UTILS_STRING_HPP_
#define LIBX_UTILS_STRING_HPP_

#include <string>
#include "Libraries/Utils/Typedefs.h"

namespace libX {
namespace utils {

using String = std::string;

inline bool_t
StartWith(
    const String& input,
    const String& value
) {
    return input.find(value) == 0;
}

inline void
Replace(
    String& input,
    const String& oldStr,
    const String& newStr
) {
    std::string::size_type pos = 0u;
    while ((pos = input.find(oldStr, pos)) != String::npos){
        input.replace(pos, oldStr.length(), newStr);
        pos += newStr.length();
    }
}

}
}

#endif //LIBX_STRING_HPP_
