//
// Created by TrungTQ on 20/04/17 08:46
//

#include "Libraries/Utils/TimeSpan.hpp"

namespace libX {
namespace utils {

const TimeSpan::TimeDiff TimeSpan::MILLISECONDS = 1000;
const TimeSpan::TimeDiff TimeSpan::SECONDS      = 1000 * TimeSpan::MILLISECONDS;
const TimeSpan::TimeDiff TimeSpan::MINUTES      =   60 * TimeSpan::SECONDS;
const TimeSpan::TimeDiff TimeSpan::HOURS        =   60 * TimeSpan::MINUTES;
const TimeSpan::TimeDiff TimeSpan::DAYS         =   24 * TimeSpan::HOURS;

/// @fn     TimeSpan
/// @brief  None
/// @param  None
/// @return None
TimeSpan::TimeSpan(
) : m_span (0) {
}

/// @fn     TimeSpan
/// @brief  None
/// @param  microSeconds
/// @return None
TimeSpan::TimeSpan(
    TimeDiff microSeconds
) : m_span (microSeconds) {
}

/// @fn     TimeSpan
/// @brief  None
/// @param  None
/// @return None
TimeSpan::TimeSpan(
    int_t iDays,
    int_t iHours,
    int_t iMinutes,
    int_t iSeconds,
    int_t iMicroSeconds
) : m_span (TimeDiff(iMicroSeconds) +
            TimeDiff(iSeconds) * SECONDS +
            TimeDiff(iMinutes) * MINUTES +
            TimeDiff(iHours)   * HOURS   +
            TimeDiff(iDays)    * DAYS){

}

/// @fn     TimeSpan
/// @brief  None
/// @param  None
/// @return None
TimeSpan::TimeSpan(
    const TimeSpan& copied
) : m_span (copied.m_span) {

}

/// @fn     ~TimeSpan
/// @brief  None
/// @param  None
/// @return None
TimeSpan::~TimeSpan(
) {
}

/// @fn     Swap
/// @brief  None
/// @param  None
/// @return None
void
TimeSpan::Swap(
    TimeSpan& other
) {
    std::swap(m_span, other.m_span);
}

/// @fn     operator==
/// @brief  None
/// @param  None
/// @return None
bool_t
TimeSpan::operator== (
    const TimeSpan& timespan
) const {
    return m_span == timespan.m_span;
}

/// @fn     operator!=
/// @brief  None
/// @param  None
/// @return None
bool_t
TimeSpan::operator!= (
    const TimeSpan& timespan
) const {
    return m_span != timespan.m_span;
}

/// @fn     operator>
/// @brief  None
/// @param  None
/// @return None
bool_t
TimeSpan::operator> (
    const TimeSpan& timespan
) const {
    return m_span > timespan.m_span;
}

/// @fn     operator>=
/// @brief  None
/// @param  None
/// @return None
bool_t
TimeSpan::operator>= (
    const TimeSpan& timespan
) const {
    return m_span >= timespan.m_span;
}

/// @fn     operator<
/// @brief  None
/// @param  None
/// @return None
bool_t
TimeSpan::operator< (
    const TimeSpan& timespan
) const {
    return m_span < timespan.m_span;
}

/// @fn     operator<=
/// @brief  None
/// @param  None
/// @return None
bool_t
TimeSpan::operator<= (
    const TimeSpan& timespan
) const {
    return m_span <= timespan.m_span;
}

/// @fn     operator==
/// @brief  None
/// @param  None
/// @return None
bool_t
TimeSpan::operator== (
    TimeDiff microSeconds
) const {
    return m_span == microSeconds;
}

/// @fn     operator!=
/// @brief  None
/// @param  None
/// @return None
bool_t
TimeSpan::operator!= (
    TimeDiff microSeconds
) const {
    return m_span != microSeconds;
}

/// @fn     operator>
/// @brief  None
/// @param  None
/// @return None
bool_t
TimeSpan::operator> (
    TimeDiff microSeconds
) const {
    return m_span > microSeconds;
}

/// @fn     operator>=
/// @brief  None
/// @param  None
/// @return None
bool_t
TimeSpan::operator>= (
    TimeDiff microSeconds
) const {
    return m_span >= microSeconds;
}

/// @fn     operator<
/// @brief  None
/// @param  None
/// @return None
bool_t
TimeSpan::operator< (
    TimeDiff microSeconds
) const {
    return m_span < microSeconds;
}

/// @fn     operator<=
/// @brief  None
/// @param  None
/// @return None
bool_t
TimeSpan::operator<= (
    TimeDiff microSeconds
) const {
    return m_span <= microSeconds;
}

/// @fn     operator+
/// @brief  None
/// @param  None
/// @return None
TimeSpan
TimeSpan::operator+ (
    const TimeSpan& timespan
) const {
    return TimeSpan(m_span + timespan.m_span);
}

/// @fn     operator-
/// @brief  None
/// @param  None
/// @return None
TimeSpan
TimeSpan::operator- (
    const TimeSpan& timespan
) const {
    return TimeSpan(m_span - timespan.m_span);
}

/// @fn     operator+
/// @brief  None
/// @param  None
/// @return None
TimeSpan
TimeSpan::operator+ (
    TimeDiff microSeconds
) const {
    return TimeSpan(m_span + microSeconds);
}

/// @fn     operator-
/// @brief  None
/// @param  None
/// @return None
TimeSpan
TimeSpan::operator- (
    TimeDiff microSeconds
) const {
    return TimeSpan(m_span - microSeconds);
}

/// @fn     operator+=
/// @brief  None
/// @param  None
/// @return None
TimeSpan&
TimeSpan::operator+= (
    const TimeSpan& timespan
) {
    m_span += timespan.m_span;
    return *this;
}

/// @fn     operator-=
/// @brief  None
/// @param  None
/// @return None
TimeSpan&
TimeSpan::operator-= (
    const TimeSpan& timespan
) {
    m_span -= timespan.m_span;
    return *this;
}

/// @fn     operator+=
/// @brief  None
/// @param  None
/// @return None
TimeSpan&
TimeSpan::operator+= (
    TimeDiff microSeconds
) {
    m_span += microSeconds;
    return *this;
}

/// @fn     operator-=
/// @brief  None
/// @param  None
/// @return None
TimeSpan&
TimeSpan::operator-= (
    TimeDiff microSeconds
) {
    m_span -= microSeconds;
    return *this;
}

TimeSpan&
TimeSpan::Assign(
    i64_t seconds,
    i64_t microSeconds
) {
    m_span = TimeDiff(seconds) * SECONDS + TimeDiff(microSeconds);
    return *this;
}

/// @fn     Days
/// @brief  None
/// @param  None
/// @return None
int_t
TimeSpan::Days(
) const {
    return (int_t) (m_span / DAYS);
}

/// @fn     Hours
/// @brief  None
/// @param  None
/// @return None
int_t
TimeSpan::Hours(
) const {
    return (int_t) ((m_span / HOURS) % 24);
}

/// @fn     TotalHours
/// @brief  None
/// @param  None
/// @return None
int_t
TimeSpan::TotalHours(
) const {
    return (int_t) (m_span / HOURS);
}

/// @fn     Minutes
/// @brief  None
/// @param  None
/// @return None
int_t
TimeSpan::Minutes(
) const {
    return (int_t) ((m_span / MINUTES) % 60);
}

/// @fn     TotalMinutes
/// @brief  None
/// @param  None
/// @return None
int_t
TimeSpan::TotalMinutes(
) const {
    return (int_t) (m_span / MINUTES);
}

/// @fn     Seconds
/// @brief  None
/// @param  None
/// @return None
int_t
TimeSpan::Seconds(
) const {
    return (int_t) ((m_span / SECONDS) % 60);
}

/// @fn     TotalSeconds
/// @brief  None
/// @param  None
/// @return None
int_t
TimeSpan::TotalSeconds(
) const {
    return (int_t) (m_span / SECONDS);
}

/// @fn     Milliseconds
/// @brief  None
/// @param  None
/// @return None
int_t
TimeSpan::Milliseconds(
) const {
    return (int_t) ((m_span / MILLISECONDS) % 1000);
}

/// @fn     TotalMilliseconds
/// @brief  None
/// @param  None
/// @return None
TimeSpan::TimeDiff
TimeSpan::TotalMilliSeconds(
) const {
    return (int_t) (m_span / MILLISECONDS);
}

/// @fn     Microseconds
/// @brief  None
/// @param  None
/// @return None
int_t
TimeSpan::MicroSeconds(
) const {
    return (int_t) (m_span % 1000);
}

/// @fn     USeconds
/// @brief  None
/// @param  None
/// @return None
int_t
TimeSpan::USeconds(
) const {
    return (int_t) (m_span % 1000000);
}

/// @fn     TotalMicroseconds
/// @brief  None
/// @param  None
/// @return None
TimeSpan::TimeDiff
TimeSpan::TotalMicroSeconds(
) const {
    return m_span;
}

}
}
