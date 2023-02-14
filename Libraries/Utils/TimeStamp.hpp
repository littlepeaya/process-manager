//
// Created by TrungTQ on 20/04/17 07:25
//

#ifndef LIBX_UTILS_TIMESTAMP_HPP_
#define LIBX_UTILS_TIMESTAMP_HPP_

#include "Libraries/Utils/Typedefs.h"

namespace libX {
namespace utils {

class TimeSpan;

class TimeStamp {
protected:
    void Swap(TimeStamp& other);
public:
    typedef i64_t TimeVal;
    typedef i64_t UtcTimeVal;
    typedef i64_t TimeDiff;

    static const TimeVal TIMEVAL_MIN;
    static const TimeVal TIMEVAL_MAX;

    TimeStamp();
    TimeStamp(TimeVal timeval);
    TimeStamp(const TimeStamp& copied);

    ~TimeStamp();

    TimeStamp& operator= (const TimeStamp& copied);
    TimeStamp& operator= (TimeVal timeval);

    void Update();

    bool_t operator== (const TimeStamp& timestamp) const;
    bool_t operator!= (const TimeStamp& timestamp) const;
    bool_t operator>  (const TimeStamp& timestamp) const;
    bool_t operator<  (const TimeStamp& timestamp) const;
    bool_t operator>= (const TimeStamp& timestamp) const;
    bool_t operator<= (const TimeStamp& timestamp) const;

    TimeStamp operator+  (TimeDiff timediff) const;
    TimeStamp operator-  (TimeDiff timediff) const;

    TimeDiff  operator- (const TimeStamp& timestamp) const;

    TimeStamp operator+  (const TimeSpan& timespan) const;
    TimeStamp operator-  (const TimeSpan& timespan) const;

    TimeStamp& operator+= (TimeDiff timediff);
    TimeStamp& operator-= (TimeDiff timediff);

    TimeStamp& operator+= (const TimeSpan& timespan);
    TimeStamp& operator-= (const TimeSpan& timespan);

    /// @return
    TimeVal Raw() const;

    TimeDiff Elapsed() const;

    time_t EpochTime() const;

    UtcTimeVal UtcTime() const;

    /// @fn      IsElapsed
    /// @brief  None
    /// @param  TIMEDIFF interval
    /// @return BOOL
    bool_t IsElapsed(TimeDiff interval) const;

    static TimeStamp FromEpochTime(time_t t);

    static TimeStamp FromUtcTime(UtcTimeVal timeval);

    static TimeDiff Resolution();
private:
    TimeVal  m_ts;
};

typedef TimeStamp  TimeStamp_t;
typedef TimeStamp* TimeStamp_p;

}
}

#endif //!LIBX_UTILS_TIMESTAMP_HPP_
