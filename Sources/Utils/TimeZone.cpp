//
// Created by TrungTQ on 20/04/17 07:23
//

#include "Libraries/Utils/Exception.hpp"
#include "Libraries/Utils/TimeZone.hpp"

namespace libX {
namespace utils {

TimeZoneInfo TimeZone::m_timezoneinfo;

/// @fn     UtcOffset
/// @brief  None
/// @param  None
/// @return None

int_t
TimeZone::UtcOffset(
) {
    return m_timezoneinfo.TimeZone();
}

/// @fn     Dst
/// @brief  None
/// @param  None
/// @return None
int_t
TimeZone::Dst(
) {
    time_t now = time(NULL);
    struct tm t;
    if (!localtime_r(&now, &t)) {
        THROW_MESSAGE("cann't get local time DST offset");
    }
    return t.tm_isdst == 1 ? 3600 : 0;
}

/// @fn     IsDst
/// @brief  None
/// @param  None
/// @return None
bool_t
TimeZone::IsDst(
    const TimeStamp& timestamp
) {
    time_t time = timestamp.EpochTime();
    struct tm *ptm = localtime(&time);
    if (ptm == NULL) {
        THROW_MESSAGE("cann't get lcal time DST flag");
    }
    return (ptm->tm_isdst > 0);
}


/// @fn     Tzd
/// @brief  None
/// @param  None
/// @return None
int_t
TimeZone::Tzd(
) {
    return UtcOffset() + Dst();
}

/// @fn     NameZone
/// @brief  None
/// @param  None
/// @return None
String
TimeZone::NameZone(
) {
    return m_timezoneinfo.NameZone(Dst() != 0);
}


/// @fn     StandardName
/// @brief  None
/// @param  None
/// @return None
String
TimeZone::StandardName(
) {
    return m_timezoneinfo.NameZone(FALSE);
}

/// @fn     DstName
/// @brief  None
/// @param  None
/// @return None
String
TimeZone::DstName(
) {
    return m_timezoneinfo.NameZone(TRUE);
}

}
}
