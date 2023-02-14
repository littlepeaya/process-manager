//
// Created by TrungTQ on 20/04/17 07:23
//

#ifndef LIBX_UTILS_TIMEZONE_HPP_
#define LIBX_UTILS_TIMEZONE_HPP_

#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Utils/String.hpp"
#include "TimeStamp.hpp"
#include "TimeZoneInfo.hpp"

namespace libX {
namespace utils {

class TimeZone {
private:
    static TimeZoneInfo m_timezoneinfo;
public:
    static int_t UtcOffset();
    static int_t Dst(); // Daylight saving time
    static bool_t IsDst(const TimeStamp& timestamp);
    static int Tzd(); // time zone differential
    static String NameZone();
    static String StandardName();
    static String DstName();
};

}
}

#endif //!LIBX_UTILS_TIMEZONE_HPP_
