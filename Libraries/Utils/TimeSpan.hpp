//
// Created by TrungTQ on 20/04/17 08:46
//

#ifndef LIBX_UTILS_TIMESPAN_HPP_
#define LIBX_UTILS_TIMESPAN_HPP_

#include "Libraries/Utils/Typedefs.h"
#include "TimeStamp.hpp"

namespace libX {
namespace utils {

class TimeSpan {
protected:
    void Swap(TimeSpan& other);
public:
    typedef TimeStamp::TimeDiff TimeDiff;

    /// @fn     TimeSpan
    /// @brief  Constructor
    /// @param  None
    /// @return None
    TimeSpan();

    /// @fn     TimeSpan
    /// @brief  Constructor
    /// @param  None
    /// @return None
    TimeSpan(TimeDiff microSeconds);

    /// @fn     TimeSpan
    /// @brief  Constructor
    /// @param  IDWORD iDays
    /// @param  IDWORD iHours
    /// @param  IDWORD iMinutes
    /// @param  IDWORD iSeconds
    /// @param  IDWORD iMicroSeconds
    /// @return None
    TimeSpan(int_t iDays, int_t iHours, int_t iMinutes, int_t iSeconds, int_t iMicroSeconds);

    /// @fn     TimeSpan
    /// @brief  Constructor
    /// @param  None
    /// @return None
    TimeSpan(const TimeSpan& copied);

    /// @fn     ~TimeSpan
    /// @brief  Destructor
    /// @param  None
    /// @return None
    ~TimeSpan();

    /// Relation Operators
    bool_t operator == (const TimeSpan& timespan) const;
    bool_t operator != (const TimeSpan& timespan) const;
    bool_t operator >  (const TimeSpan& timespan) const;
    bool_t operator >= (const TimeSpan& timespan) const;
    bool_t operator <  (const TimeSpan& timespan) const;
    bool_t operator <= (const TimeSpan& timespan) const;

    bool_t operator == (TimeDiff microSeconds) const;
    bool_t operator != (TimeDiff microSeconds) const;
    bool_t operator >  (TimeDiff microSeconds) const;
    bool_t operator >= (TimeDiff microSeconds) const;
    bool_t operator <  (TimeDiff microSeconds) const;
    bool_t operator <= (TimeDiff microSeconds) const;

    TimeSpan operator+ (const TimeSpan& timespan) const;
    TimeSpan operator- (const TimeSpan& timespan) const;

    TimeSpan operator+ (TimeDiff microSeconds) const;
    TimeSpan operator- (TimeDiff microSeconds) const;

    TimeSpan& operator+= (const TimeSpan& timespan);
    TimeSpan& operator-= (const TimeSpan& timespan);

    TimeSpan& operator+= (TimeDiff microSeconds);
    TimeSpan& operator-= (TimeDiff microSeconds);

    TimeSpan& Assign(i64_t seconds, i64_t microSeconds);

    /// @fn     Days
    /// @brief  None
    /// @param  None
    /// @return None
    int_t Days() const;

    /// @fn     Hours
    /// @brief  None
    /// @param  None
    /// @return None
    int_t Hours() const;

    /// @fn     TotalHours
    /// @brief  None
    /// @param  None
    /// @return None
    int_t TotalHours() const;

    /// @fn     Minutes
    /// @brief  None
    /// @param  None
    /// @return None
    int_t Minutes() const;

    /// @fn     TotalMinutes
    /// @brief  None
    /// @param  None
    /// @return None
    int_t TotalMinutes() const;

    /// @fn     Seconds
    /// @brief  None
    /// @param  None
    /// @return None
    int_t Seconds() const;

    /// @fn     TotalSeconds
    /// @brief  None
    /// @param  None
    /// @return None
    int_t TotalSeconds() const;

    /// @fn     Milliseconds
    /// @brief  None
    /// @param  None
    /// @return None
    int_t Milliseconds() const;

    /// @fn     TotalMilliSeconds
    /// @brief  None
    /// @param  None
    /// @return None
    TimeDiff TotalMilliSeconds() const;

    /// @fn     MicroSeconds
    /// @brief  None
    /// @param  None
    /// @return None
    int_t MicroSeconds() const;

    /// @fn     USeconds
    /// @brief  None
    /// @param  None
    /// @return None
    int_t USeconds() const;

    /// @fn     DaysTotalMicroSeconds
    /// @brief  None
    /// @param  None
    /// @return None
    TimeDiff TotalMicroSeconds() const;

    static const TimeDiff MILLISECONDS;
    static const TimeDiff SECONDS;
    static const TimeDiff MINUTES;
    static const TimeDiff HOURS;
    static const TimeDiff DAYS;
private:
    TimeDiff m_span;
};

typedef TimeSpan  TimeSpan_t;
typedef TimeSpan* TimeSpan_p;

}
}

#endif //!LIBX_UTILS_TIMESPAN_HPP_
