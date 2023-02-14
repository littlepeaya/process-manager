//
// Created by TrungTQ on 20/04/17 07:25
//

#include <limits>
#include "Libraries/Utils/Exception.hpp"
#include "Libraries/Utils/TimeSpan.hpp"
#include "Libraries/Utils/TimeStamp.hpp"

namespace libX {
namespace utils {

const TimeStamp::TimeVal TimeStamp::TIMEVAL_MIN = std::numeric_limits<TimeStamp::TimeVal>::min();
const TimeStamp::TimeVal TimeStamp::TIMEVAL_MAX = std::numeric_limits<TimeStamp::TimeVal>::max();

/// @fn     TimeStamp
/// @brief  None
/// @param  None
/// @return None
TimeStamp::TimeStamp(
) {
    TimeStamp::Update();
}

/// @fn     TimeStamp
/// @brief  None
/// @param  timeval
/// @return None
TimeStamp::TimeStamp(
    TimeStamp::TimeVal timeval
) : m_ts (timeval) {
}

/// @fn     TimeStamp
/// @brief  None
/// @param  None
/// @return None
TimeStamp::TimeStamp(
    const TimeStamp& copied
) : m_ts (copied.m_ts) {
}

/// @fn     ~TimeStamp
/// @brief  None
/// @param  None
/// @return None
TimeStamp::~TimeStamp(
) {
}

/// @fn     TimeStamp
/// @brief  None
/// @param  None
/// @return None
TimeStamp&
TimeStamp::operator= (
    const TimeStamp& copied
) {
    TimeStamp temp(copied);
    TimeStamp::Swap(temp);
    return *this;
}

/// @fn     TimeStamp
/// @brief  None
/// @param  None
/// @return None
TimeStamp&
TimeStamp::operator= (
    TimeStamp::TimeVal timeval
) {
    m_ts = timeval;
    return *this;
}

/// @fn     Swap
/// @brief  None
/// @param  None
/// @return None
void
TimeStamp::Swap(
    TimeStamp& other
) {
    std::swap(m_ts, other.m_ts);
}

/// @fn     Update
/// @brief  None
/// @param  None
/// @return None
void
TimeStamp::Update(
) {
    struct timespec ts;
    if (clock_gettime(CLOCK_REALTIME, &ts)) {
        THROW_MESSAGE("cann't get time of day");
    }
    m_ts = (TimeVal) (ts.tv_sec) * 1000000 + ts.tv_nsec / 1000;
}

/// @fn     operator==
/// @brief  None
/// @param  None
/// @return None
bool_t
TimeStamp::operator== (
    const TimeStamp& timestamp
) const {
    return m_ts == timestamp.m_ts;
}

/// @fn     operator!=
/// @brief  None
/// @param  None
/// @return None
bool_t
TimeStamp::operator!= (
    const TimeStamp& timestamp
) const {
    return m_ts != timestamp.m_ts;
}

/// @fn     operator>
/// @brief  None
/// @param  None
/// @return None
bool_t
TimeStamp::operator> (
    const TimeStamp& timestamp
) const {
    return m_ts > timestamp.m_ts;
}

/// @fn     operator<
/// @brief  None
/// @param  None
/// @return None
bool_t
TimeStamp::operator< (
    const TimeStamp &timestamp
) const {
    return m_ts < timestamp.m_ts;
}

/// @fn     operator>=
/// @brief  None
/// @param  None
/// @return None
bool_t
TimeStamp::operator>= (
    const TimeStamp &timestamp
) const {
    return m_ts >= timestamp.m_ts;
}

/// @fn     operator<=
/// @brief  None
/// @param  None
/// @return None
bool_t
TimeStamp::operator<= (
    const TimeStamp &timestamp
) const {
    return m_ts <= timestamp.m_ts;
}

/// @fn     operator+
/// @brief  None
/// @param  None
/// @return None
TimeStamp
TimeStamp::operator+ (
    TimeStamp::TimeDiff timediff
) const {
    return TimeStamp(m_ts + timediff);
}

/// @fn     operator-
/// @brief  None
/// @param  None
/// @return None
TimeStamp
TimeStamp::operator- (
    TimeStamp::TimeDiff timediff
) const {
    return TimeStamp(m_ts - timediff);
}

/// @fn     operator-
/// @brief  None
/// @param  None
/// @return None
TimeStamp::TimeDiff
TimeStamp::operator- (
    const TimeStamp& timestamp
) const {
    return m_ts - timestamp.m_ts;
}

/// @fn     operator+
/// @brief  None
/// @param  None
/// @return None
TimeStamp
TimeStamp::operator+ (
    const TimeSpan& timespan
) const {
    return TimeStamp(m_ts + timespan.TotalMicroSeconds());
}

/// @fn     operator-
/// @brief  None
/// @param  None
/// @return None
TimeStamp
TimeStamp::operator- (
    const TimeSpan& timespan
) const {
    return TimeStamp(m_ts - timespan.TotalMicroSeconds());
}

/// @fn     operator+=
/// @brief  None
/// @param  None
/// @return None
TimeStamp&
TimeStamp::operator+= (
    TimeDiff timediff
) {
    m_ts += timediff;
    return *this;
}

/// @fn     operator-=
/// @brief  None
/// @param  None
/// @return None
TimeStamp&
TimeStamp::operator-= (
    TimeDiff timediff
) {
    m_ts -= timediff;
    return *this;
}

/// @fn     operator+=
/// @brief  None
/// @param  None
/// @return None
TimeStamp&
TimeStamp::operator+= (
    const TimeSpan& timespan
) {
    m_ts += timespan.TotalMicroSeconds();
    return *this;
}

/// @fn     operator-=
/// @brief  None
/// @param  None
/// @return None
TimeStamp&
TimeStamp::operator-= (
    const TimeSpan& timespan
) {
    m_ts -= timespan.TotalMicroSeconds();
    return *this;
}

/// @fn     TimeVal
/// @brief  None
/// @param  None
/// @return None
TimeStamp::TimeVal
TimeStamp::Raw(
) const {
    return m_ts;
}

/// @fn     IsElapsed
/// @brief  None
/// @param  None
/// @return None
bool_t
TimeStamp::IsElapsed(
    TimeDiff interval
) const {
    TimeStamp now;
    TimeStamp::TimeDiff diff = now - *this;
    return diff >= interval;
}

/// @fn     FromEpochTime
/// @brief  None
/// @param  None
/// @return None
TimeStamp
TimeStamp::FromEpochTime(
    time_t t
) {
    return TimeStamp(TimeVal(t) * TimeStamp::Resolution());
}

/// @fn     FromUtcTime
/// @brief  None
/// @param  None
/// @return None
TimeStamp
TimeStamp::FromUtcTime(
    UtcTimeVal timeval
) {
    timeval -= (TimeDiff(0x01b21dd2) << 32) + 0x13814000;
    timeval /= 10;
    return TimeStamp(timeval);
}

/// @fn     Resolution
/// @brief  None
/// @param  None
/// @return None
TimeStamp::TimeDiff
TimeStamp::Resolution(
) {
    return 1000000;
}

/// @fn     Elapsed
/// @brief  None
/// @param  None
/// @return None
TimeStamp::TimeDiff
TimeStamp::Elapsed(
) const {
    TimeStamp now;
    return now - *this;
}

/// @fn     EpochTime
/// @brief  None
/// @param  None
/// @return None
time_t
TimeStamp::EpochTime(
) const {
    time_t t = m_ts / 1000000;
    return t;
}

/// @fn     UtcTime
/// @brief  None
/// @param  None
/// @return None
TimeStamp::UtcTimeVal
TimeStamp::UtcTime(
) const {
    return m_ts * 10 + (TimeDiff(0x01b21dd2) << 32) + 0x13814000;
}

}
}

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

