//
// Created by TrungTQ on 2016-05-16 11:45:00
//

#ifndef LIBX_UTILS_TYPEDEFS_H_
#define LIBX_UTILS_TYPEDEFS_H_

#include <bits/wordsize.h>
#include <stddef.h>
#include <utility>
#include <sys/time.h>
#include <iostream>
#include <exception>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <atomic>
#include <string>
#include <vector>
#include <string.h>
#include <functional>
#include <algorithm>

#ifdef MT7688
#include <string>
#include <sstream>
#include <cstdlib>
#endif /* MT7688 */

typedef unsigned char           byte_t;     // 1 byte
typedef unsigned short          word_t;     // 2 byte
typedef unsigned long           dword_t;    // 8 byte
typedef unsigned int            uint_t;     // 4 byte
typedef char                    char_t;     // 1 byte
//typedef void                    void;

typedef byte_t*                 byte_p;
typedef word_t*                 word_p;
typedef dword_t*                dword_p;
typedef uint_t*                 uint_p;
typedef char_t*                 char_p;
typedef void*                 void_p;

typedef unsigned char           u8_t;       // 1 byte
typedef unsigned char*          u8_p;       // 1 byte

typedef signed char             i8_t;       // 1 byte
typedef signed char*            i8_p;       // 1 byte

typedef unsigned char**         u8_pp;

typedef unsigned short          u16_t;      // 2 byte
typedef unsigned short*         u16_p;      // 2 byte

typedef signed short            i16_t;      // 2 byte
typedef signed short*           i16_p;      // 2 byte

typedef unsigned int            uint_t;
typedef signed int              int_t;

typedef unsigned int*           uint_p;
typedef signed int*             int_p;

typedef float                   flo_t;
typedef float*                  flo_p;

typedef double                  dob_t;
typedef double*                 dob_p;

typedef const char              const_char_t;
typedef const char*             const_char_p;

typedef const void              const_void;
typedef const void*             const_void_p;

typedef void const              void_const_t;
typedef void const*             void_const_p;

#if __WORDSIZE == 64
typedef unsigned int            u32_t;      // 4 byte
typedef unsigned long int       u64_t;

typedef signed int              i32_t;      // 4 byte
typedef signed long int         i64_t;

typedef unsigned int*           u32_p;      // 4 byte
typedef unsigned long int*      u64_p;

typedef signed int*             i32_p;      // 4 byte
typedef signed long int*        i64_p;

#ifndef PROCESSOR_WORDSIZE
#define PROCESSOR_WORDSIZE      8
#endif

#elif __WORDSIZE == 32
typedef unsigned int            u32_t;      // 4 byte
typedef unsigned long long      u64_t;

typedef signed int              i32_t;      // 4 byte
typedef signed long long        i64_t;

typedef unsigned int*           u32_p;      // 4 byte
typedef unsigned long int*      u64_p;

typedef signed int*             i32_p;      // 4 byte
typedef signed long long*       i64_p;

#ifndef PROCESSOR_WORDSIZE
#define PROCESSOR_WORDSIZE      4
#endif

#endif

#ifndef __cplusplus
typedef unsigned char           bool_t;
#ifndef TRUE
#define TRUE                    (1)
#endif
#ifndef FALSE
#define FALSE                   (0)
#endif
#else
typedef bool                    bool_t;
#ifndef TRUE
#define TRUE                    true
#endif
#ifndef FALSE
#define FALSE                   false
#endif
#endif

#ifndef NULL
#define NULL                    (0)
#endif

#ifndef BV
#define BV(n)                   (1 << (n))
#endif

#ifndef ST
#define ST(x)                   do { x } while (__LINE__ == -1)
#endif

#ifndef HI_UINT16
#define HI_UINT16(a)            (((a) >> 8) & 0xFF)
#endif

#ifndef LO_UINT16
#define LO_UINT16(a)            ((a) & 0xFF)
#endif

#ifndef MERGE
#define MERGE(h,l)              (((h) << 8) | (l))
#endif

#ifndef UNUSED
#define UNUSED(x)               (x) = (x)
#endif

#ifndef INVALID_SOCKET
#define INVALID_SOCKET          (-1)
#endif

#ifndef RTIMER_ERROR
#define RTIMER_ERROR            (-1)
#endif

#define LIBX_VERSION            LIBX_VERSION_CHECK(LIBX_VERSION_MAJOR, LIBX_VERSION_MINOR, LIBX_VERSION_PATCH)

#define LIBX_VERSION_CHECK(major, minor, patch) ((major << 16) | (minor << 8) | (path))

template<typename T> using identity = T;

#ifndef __FILENAME__
#define __FILENAME__    (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif /* __FILENAME__ */

#define DISABLE_COPY(Class) \
    Class(const Class &) = delete;\
    Class &operator=(const Class &) = delete;

#if 0
template<typename T>
inline T ABS(const T& t) { return t >= 0 ? t : -t; }
template <typename T>
inline const T& MIN(const T& a, const T& b) { return (a < b) ? a : b; }
template <typename T>
inline const T& MAX(const T &a, const T& b) { return (a < b) ? b : a; }
template <typename T>
inline const T& BOUND(const T& min, const T& val, const T& max)
{ return MAX(min, MIN(max, val)); }

#endif // 0

#ifdef MT7688
namespace std {
    inline std::string to_string(int i) {
        std::stringstream ss;
        ss << i;
        return ss.str();
    }

    inline int stoi(std::string string) {
        return atoi(string.c_str());
    }

    inline int stol(std::string string) {
        return atol(string.c_str());
    }
}
#endif /* MT7688 */

namespace libX {
namespace utils {}
namespace net {}
namespace ipc {}
namespace db {}
namespace val {}
namespace curl {}
namespace uv {}
namespace thread {}
}

using namespace libX;
using namespace libX::utils;
using namespace libX::net;
using namespace libX::ipc;
using namespace libX::db;
using namespace libX::val;
using namespace libX::curl;
using namespace libX::uv;
using namespace libX::thread;

#endif /* LIBX_UTILS_TYPEDEFS_H_ */
