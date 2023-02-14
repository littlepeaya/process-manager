//
// Created by TrungTQ on 19 Jul 2017 09:20:57.
//

#ifndef LIBX_RANDOM_UTILS_HPP_
#define LIBX_RANDOM_UTILS_HPP_

#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Values/ValueArray.hpp"

namespace libX {
namespace utils {

class RandomUtils {
public:
    /// @fn     RandomNumber
    /// @brief  None
    /// @param  None
    /// @return DWORD
    static u32_t RandomNumber();

    /// @fn     RandomString
    /// @brief  None
    /// @param  None
    /// @return STRING
    static String RandomString(i32_t iSize);

    /// @fn     RandomBinaryString
    /// @brief  Readmore en.wikipedia.org/wiki/Base64
    /// @param  DWORD  dwSize
    /// @param  BOOL   boDoBase64
    /// @return STRING
    static String RandomBinaryString(i32_t iSize, bool_t boDoBase64);

    /// @fn     NextBytes
    /// @brief  None
    /// @param  lstu8_t& rbyte
    /// @return None
    static void NextBytes(lstu8_t& rbyte);
};

}
}

#endif /* !LIBX_RANDOMUTILS_HPP_ */
