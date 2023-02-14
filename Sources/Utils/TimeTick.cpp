//
// Created by TrungTQ on 30/09/17.
//

#include "Libraries/Utils/Singleton.hpp"
#include "Libraries/Utils/TimeTick.hpp"

namespace libX {
namespace utils {

TimeTick::BaseTimer::BaseTimer(
    timer_func_t func_,
    u32_t timeout_,
    i8_t repeats_,
    void_p param_
) : func (func_)
  , id (-1)
  , timeout (timeout_)
  , repeats (repeats_)
  , state (State::Idle)
  , param (param_) {}

TimeTick::BaseTimer::~BaseTimer() {}

bool_t
TimeTick::BaseTimer::IsExpired(
) const {
    return start.IsElapsed(1000 * timeout);
}

TimeTick::TimeTick(
) : m_lastId (0) {}

TimeTick::~TimeTick() {}

namespace  {
    static Singleton<TimeTick> sh;
}

TimeTick&
TimeTick::Default(
) {
    return *sh.Get();
}

i32_t
TimeTick::Attach(
    timer_func_t func,
    u32_t timeOutMiliSec,
    i8_t repeats,
    void_p param
) {
    AutoLocker<Locker> locker(m_attachedLocker);
    auto it = new BaseTimer(func, timeOutMiliSec, repeats, param);
    it->id = m_lastId++;
    it->state = BaseTimer::State::Attached;
    m_lstAttached.emplace_back(ShareBaseTimer_t(it));
    return it->id;
}

void
TimeTick::Detach(
    timer_func_t func
) {
    AutoLocker<Locker> locker(m_detachedLocker);
    m_lstDetached.emplace_back(func);
}

bool_t
TimeTick::Kill(
    timer_func_t func
) {
    for (auto & it : m_lstRunning) {
        if (*(it->func->m_pDelegate) == *(func->m_pDelegate)) {
            it->state = BaseTimer::State::Killed;
            return TRUE;
        }
    }
    return FALSE;
}

bool_t
TimeTick::Kill(
    void (*func)(void_p)
) {
    return Kill(Slot(func));
}

void
TimeTick::Detach(
    void (*func)(void_p)
) {
    Detach(Slot(func));
}

i32_t
TimeTick::Attach(
    void (*func)(void_p),
    u32_t timeOutMiliSec,
    i8_t repeats,
    void_p param
) {
    Detach(Slot(func));
    return Attach(Slot(func), timeOutMiliSec, repeats, param);
}

void
TimeTick::Attach(
) {
    AutoLocker<Locker> locker(m_attachedLocker);
    for (auto & it : m_lstAttached) {
        it->state = BaseTimer::State::Running;
        m_lstRunning.emplace_back(it);
    }
    m_lstAttached.clear();
}

void
TimeTick::Detach(
) {
    AutoLocker<Locker> locker(m_detachedLocker);
    for (auto ait : m_lstDetached) {
        for (auto it = m_lstRunning.begin(); it != m_lstRunning.end(); ) {
            if ((*((*it)->func->m_pDelegate)) == *(ait->m_pDelegate)) {
                it = m_lstRunning.erase(it);
                break;
            } else {
                ++it;
            }
        }
    }
    m_lstDetached.clear();
}

void
TimeTick::Process(
) {
    for (auto it = m_lstRunning.begin(); it != m_lstRunning.end(); ) {
        if ((*it)->state == BaseTimer::State::Running && (*it)->IsExpired()) {
            auto pfn = (*it)->func->m_pDelegate;
            pfn->operator ()((*it)->param);
            if ((*it)->repeats != FOREVER && (*it)->repeats > 0) {
                (*it)->repeats--;
            }
            if ((*it)->repeats == 0) {
                it = m_lstRunning.erase(it);
            } else {
                (*it)->start.Update();
                ++it;
            }
        } else {
            ++it;
        }
    }
    Detach();
    Attach();
}

}
}
