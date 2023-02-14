//
// Created by TrungTQ on 20/04/17 15:20
//

#ifndef LIBX_UTILS_TIMEZONEINFO_HPP_
#define LIBX_UTILS_TIMEZONEINFO_HPP_

#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Utils/Core.hpp"
#include "Libraries/Utils/String.hpp"
#include "Libraries/Utils/Locker.hpp"

namespace libX {
namespace utils {

class TimeZoneInfo {
private:
    utils::Locker_t m_locker;
public:
    TimeZoneInfo();
    virtual~ TimeZoneInfo();

    int_t TimeZone();
    String NameZone(bool_t boIsDst);
};

}
}

#endif //!LIBX_UTILS_TIMEZONEINFO_HPP_
