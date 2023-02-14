//
// Created by TrungTQ on 2016-10-31 13:30:00
//

#ifndef LIBX_UTILS_TIMETICK_HPP_
#define LIBX_UTILS_TIMETICK_HPP_

#include "Libraries/Utils/Core.hpp"
#include "Signal.hpp"
#include "TimeStamp.hpp"

namespace libX {
namespace utils {

typedef Signal<void(void_p)>::SlotPtr_t timer_func_t;

class TimeTick {
private:
    enum class State {
        Idle = 0x00,
        Running = 0x01,
    };
    struct BaseTimer {
        enum class State {
            Idle = 0x00,
            Running = 0x01,
            Killed = 0x02,
            Attached = 0x04,
            Detached = 0x08,
        };
        TimeStamp start;
        timer_func_t func;
        i32_t id;
        u32_t timeout;
        i8_t repeats;
        State state;
        void_p param;

        BaseTimer(timer_func_t func_, u32_t timeout_, i8_t repeats_, void_p params_);

        ~BaseTimer();

        bool_t IsExpired() const;
    };

    MAKE_TYPE_SHAREPTR(BaseTimer, BaseTimer)

    typedef Vector<ShareBaseTimer_t> lstBaseTimer_t;
    typedef Vector<timer_func_t> lstTimeFunc_t;
    Locker_t m_attachedLocker;
    Locker_t m_detachedLocker;
    lstBaseTimer_t  m_lstRunning;
    lstBaseTimer_t  m_lstAttached;
    lstTimeFunc_t   m_lstDetached;
    i32_t m_lastId;

    i32_t  Attach(timer_func_t func, u32_t timeOutMiliSec, i8_t repeats, void_p param);

    void Detach(timer_func_t func);

    bool_t Kill(timer_func_t func);

    void Attach();

    void Detach();

    DISABLE_COPY(TimeTick)
public:
    enum Repeat {
        ONE_TIME = 0,
        FOREVER = 0xFF
    };

    TimeTick();

    virtual ~TimeTick();

    i32_t Attach(void (*func)(void_p), u32_t timeOutMiliSec, i8_t repeats, void_p param);

    template<class Class>
    i32_t Attach(Class *pInstance, void (Class::*completed)(void_p), u32_t timeOutMiliSec, i8_t repeats, void_p param);

    void Detach(void (*func)(void_p));

    template<class Class>
    void Detach(Class *pInstance, void (Class::*completed)(void_p));

    bool_t Kill(void (*func)(void_p));

    template<class Class>
    bool_t Kill(Class *pInstance, void (Class::*completed)(void_p));

    void Process();

    static TimeTick& Default();
};

typedef TimeTick  TimeTick_t;
typedef TimeTick* TimeTick_p;

template<class Class>
inline i32_t
TimeTick::Attach(
    Class *pInstance,
    void (Class::*completed)(void_p),
    u32_t timeOutMiliSec,
    i8_t repeats,
    void_p param
) {
    Detach(Slot(pInstance, completed));
    return Attach(Slot(pInstance, completed), timeOutMiliSec, repeats, param);
}

template<class Class>
inline void
TimeTick::Detach(
    Class *pInstance,
    void (Class::*completed)(void_p)
) {
    Detach(Slot(pInstance, completed));
}

template<class Class>
inline bool_t
TimeTick::Kill(
    Class *pInstance,
    void (Class::*completed)(void_p)
) {
    return Kill(Slot(pInstance, completed));
}

}
}

#endif /* !LIBX_UTILS_TIMETICK_HPP_ */
