//
// Created by TrungTQ on 20/04/17 15:20
//

#include "Libraries/Utils/AutoLocker.hpp"
#include "Libraries/Utils/TimeZoneInfo.hpp"

namespace libX {
namespace utils {

/// @fn     TimeZoneInfo
/// @brief  None
/// @param  None
/// @return None
TimeZoneInfo::TimeZoneInfo(
) {
    tzset();
}

/// @fn     ~TimeZoneInfo
/// @brief  None
/// @param  None
/// @return None
TimeZoneInfo::~TimeZoneInfo(
) {

}

/// @fn     TimeZone
/// @brief  None
/// @param  None
/// @return None
int_t
TimeZoneInfo::TimeZone(
) {
    utils::AutoLocker<utils::Locker> locker(m_locker);
    time_t  now = time(NULL);
    struct tm t;
    gmtime_r(&now, &t);
    time_t utc = mktime(&t);
    return now - utc;
}

/// @fn     NameZone
/// @brief  None
/// @param  None
/// @return None
String
TimeZoneInfo::NameZone(
    bool_t boIsDst
) {
    utils::AutoLocker<utils::Locker> locker(m_locker);
    tzset();
    return tzname[boIsDst ? 1 : 0];
}

}
}
