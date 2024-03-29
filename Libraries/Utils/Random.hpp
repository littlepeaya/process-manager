//
// Created by TrungTQ on 2016-11-01 11:30:00
//

#ifndef LIBX_UTILS_RANDOM_HPP_
#define LIBX_UTILS_RANDOM_HPP_

#include <assert.h>
#include "Libraries/Utils/Core.hpp"
#include "Libraries/Utils/Typedefs.h"

namespace libX {
namespace utils {

class Random : public Core {
private:
    enum {
        MAX_TYPES = 5,
        NSHUFF = 50
    };

    u32_p   m_pFptr;
    u32_p   m_pRptr;
    u32_p   m_pState;
    int_t   m_iRandType;
    int_t   m_iRandDeg;
    int_t   m_iRandSep;
    u32_p   m_pEndPtr;
    u8_p    m_pBuffer;
public:
    enum Type {
        RND_STATE_0   =   8,  /// linear congruential
        RND_STATE_32  =  32,  /// x**7 + x**3 + 1
        RND_STATE_64  =  64,  /// x**15 + x + 1
        RND_STATE_128 = 128,  /// x**31 + x**3 + 1
        RND_STATE_256 = 256   /// x**63 + x + 1
    };

    /// @fn     Random
    /// @brief  Constructor
    /// @param  IDWORD iSize
    /// @return None
    Random(int_t iSize = 256);

    /// @fn     ~Random
    /// @brief  Destructor
    /// @param  None
    /// @return None
    virtual ~Random();

    /// @fn     GoodRand
    /// @brief  None
    /// @param  IDWORD x
    /// @return DWORD
    u32_t GoodRand(i32_t x);

    /// @fn     Seed
    /// @brief  None
    /// @param  IDWORD x
    /// @return None
    void Seed(i32_t x);

    /// @fn     Seed
    /// @brief  None
    /// @param  None
    /// @return None
    void Seed();

    /// @fn     GetSeed
    /// @brief  None
    /// @param  PBYTE pSeed
    /// @param  DWORD dwLength
    /// @return None
    void GetSeed(u8_p pSeed, u32_t dwLength);

    /// @fn     InitState
    /// @brief  None
    /// @param  DWORD  s
    /// @param  PBYTE  pArgState
    /// @param  IDWORD n
    /// @return None
    void InitState(u32_t s, u8_p pArgState, i32_t n);

    /// @fn     Next
    /// @brief  None
    /// @param  None
    /// @return DWORD
    u32_t Next();

    /// @fn     Next
    /// @brief  Returns the next 31-bit pseudo random number modulo n.
    /// @param  DWORD n
    /// @return DWORD
    u32_t Next(u32_t n);

    /// @fn     NextChar
    /// @brief  Returns the next pseudo random character.
    /// @param  None
    /// @return BYTE
    u8_t NextChar();

    /// @fn     NextBool
    /// @brief  Returns the next boolean pseudo random value.
    /// @param  None
    /// @return BOOL
    bool_t NextBool();

    /// @fn     NextFloat
    /// @brief  Returns the next float pseudo random number between 0.0 and 1.0.
    /// @param  None
    /// @return FLOAT
    flo_t NextFloat();

    /// @fn     NextDouble
    /// @brief  Returns the next double pseudo random number between 0.0 and 1.0.
    /// @param  None
    /// @return DOUBLE
    dob_t NextDouble();
};

typedef Random  Random_t;
typedef Random* Random_p;

#define RANDOM_ASSERT(x)    assert(x)

}
}

#endif /* LIBX_RANDOM_HPP_ */

// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
//
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
