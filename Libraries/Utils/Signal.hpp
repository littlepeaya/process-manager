/// Created by TrungTQ on 1 Mar 2017 11:47:27

#ifndef LIBX_SIGNAL_SIGNAL_HPP_
#define LIBX_SIGNAL_SIGNAL_HPP_

#include <functional>
#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Utils/Vector.hpp"
#include "Libraries/Utils/Function.hpp"
#include "Libraries/Utils/SharedPtr.hpp"
#include "Libraries/Utils/Locker.hpp"
#include "Libraries/Utils/AutoLocker.hpp"
#include "InternalSlot.hpp"

namespace libX {
namespace utils {

template<typename RT> class Signal;

template<typename RT, typename... Args>
class Signal<RT(Args...)> {
public:
    typedef Function<RT(Args...)> Function_t;
    typedef utils::SharedPtr<InternalSlot<RT, Args...>> SlotPtr_t;

    Signal();

    virtual ~Signal();

    Signal(const Signal& copied);

    Signal& operator= (const Signal& copied);

    i32_t Attach(const Function_t& func,
                 void_p pInstance = NULL,
                 i32_t id = -1,
                 i32_t priority = -1) const;

    i32_t Attach(SlotPtr_t slot) const;

    bool_t Detach(i32_t id) const;

    bool_t Detach(SlotPtr_t slot) const;

    bool_t Detach(const_void_p pInstance) const;

    void DetachAll() const;

    virtual void Emit(Args... args);

    size_t NbrOfSlots() const;

    Vector<SlotPtr_t> Slots() const;

    i32_t operator+= (const Function_t& func);

    i32_t operator+= (SlotPtr_t slot);

    bool_t operator-= (i32_t id);

    bool_t operator-= (SlotPtr_t slot);

    bool_t operator-= (const_void_p pInstance);
private:
    typedef Vector<SlotPtr_t> lstSlots_t;
    mutable utils::Locker_t m_locker;
    mutable lstSlots_t m_lstSlots;
    mutable i32_t m_iLastId;
};

typedef Signal<void()> NullSignal;

template<typename RT, typename... Args>
inline
Signal<RT(Args...)>::Signal(
) : m_iLastId (0) {
}

template<typename RT, typename... Args>
inline
Signal<RT(Args...)>::~Signal(
) {
}

template<typename RT, typename... Args>
inline
Signal<RT(Args...)>::Signal(
    const Signal& copied
) : m_lstSlots (copied.m_lstSlots)
  , m_iLastId (copied.m_iLastId) {
}

template<typename RT, typename... Args>
inline Signal<RT(Args...)>&
Signal<RT(Args...)>::operator= (
    const Signal& copied
) {
    if (&copied != this) {
        m_lstSlots = copied.m_lstSlots;
        m_iLastId = copied.m_iLastId;
    }
    return *this;
};

/// @fn     Attach
/// @brief  None
/// @param  None
/// @retval None
template<typename RT, typename... Args>
inline i32_t
Signal<RT(Args...)>::Attach(
    const Function_t& func,
    void_p pInstance,
    i32_t id,
    i32_t priority
) const {
    return Attach(SharedPtr<InternalSlot<RT, Args...>>(
            new InternalSlot<RT, Args...>(
            new FunctionDelegate<RT, Args...>(func), pInstance, id, priority)));
}

/// @fn     Attach
/// @brief  None
/// @param  None
/// @retval None
template<typename RT, typename... Args>
inline i32_t
Signal<RT(Args...)>::Attach(
    SlotPtr_t slot
) const {
    Detach(slot);
    utils::AutoLocker<utils::Locker> locker(m_locker);
    if (slot->m_id == -1) {
        slot->m_id = ++m_iLastId;
    }
    m_lstSlots.push_back(slot);
    std::sort(m_lstSlots.begin(), m_lstSlots.end(),
              [](SlotPtr_t const& l, SlotPtr_t const& r) {
                  return l->m_iPriority > r->m_iPriority; });
    return slot->m_id;
}

/// @fn     Detach
/// @brief  None
/// @param  None
/// @retval None
template<typename RT, typename... Args>
inline bool_t
Signal<RT(Args...)>::Detach(
    i32_t id
) const {
    utils::AutoLocker<utils::Locker> locker(m_locker);
    for (auto it = m_lstSlots.begin(); it != m_lstSlots.end(); ) {
        auto& slot = *it;
        if (slot->Alive() && slot->m_id == id) {
            slot->Kill();
            m_lstSlots.erase(it);
            return TRUE;
        }
        it++;
    }
    return FALSE;
}

/// @fn     Detach
/// @brief  None
/// @param  None
/// @retval None
template<typename RT, typename... Args>
inline bool_t
Signal<RT(Args...)>::Detach(
    SlotPtr_t other
) const {
    utils::AutoLocker<utils::Locker> locker(m_locker);
    for (auto it = m_lstSlots.begin(); it != m_lstSlots.end(); ) {
        auto& slot = *it;
        if (slot->Alive() && (*(slot->m_pDelegate)) == (*(other->m_pDelegate))) {
            slot->Kill();
            m_lstSlots.erase(it);
            return TRUE;
        }
        it++;
    }
    return FALSE;
}

template<typename RT, typename... Args>
inline bool_t
Signal<RT(Args...)>::Detach(
    const_void_p pInstance
) const {
    utils::AutoLocker<utils::Locker> locker(m_locker);
    bool_t boRemoved = FALSE;
    for (auto it = m_lstSlots.begin(); it != m_lstSlots.end();) {
        auto& slot = *it;
        if (slot->Alive() && slot->m_pInstance == pInstance) {
            slot->Kill();
            it = m_lstSlots.erase(it);
            boRemoved = TRUE;
        } else {
            it++;
        }
    }
    return boRemoved;
}

template<typename RT, typename... Args>
inline void
Signal<RT(Args...)>::DetachAll(
) const {
    utils::AutoLocker<utils::Locker> locker(m_locker);
    while (!m_lstSlots.empty()) {
        m_lstSlots.back()->Kill();
        m_lstSlots.pop_back();
    }
}

template<typename RT, typename... Args>
inline void
Signal<RT(Args...)>::Emit(
    Args... args
) {
    try {
        utils::AutoLocker<utils::Locker> locker(m_locker);
        for (auto & slot : m_lstSlots) {
            if (slot->Alive()) {
                auto func = slot->m_pDelegate;
                func->operator()(std::forward<Args>(args)...);
            }
        }
    } catch (std::exception& ex) {
//        LOG_DBUG(ex.what());
    }
}

template<typename RT, typename... Args>
inline size_t
Signal<RT(Args...)>::NbrOfSlots(
) const {
    utils::AutoLocker<utils::Locker> locker(m_locker);
    return m_lstSlots.size();
}

template<typename RT, typename... Args>
inline Vector<typename Signal<RT(Args...)>::SlotPtr_t>
Signal<RT(Args...)>::Slots(
) const {
    utils::AutoLocker<utils::Locker> locker(m_locker);
    return m_lstSlots;
}

template<typename RT, typename... Args>
inline i32_t
Signal<RT(Args...)>::operator+= (
    const Function_t& func
) {
    return Attach(func);
}

template<typename RT, typename... Args>
inline i32_t
Signal<RT(Args...)>::operator+= (
    SlotPtr_t slot
) {
    return Attach(slot);
}

template<typename RT, typename... Args>
inline bool_t
Signal<RT(Args...)>::operator-= (
    i32_t id
) {
    return Detach(id);
}

template<typename RT, typename... Args>
inline bool_t
Signal<RT(Args...)>::operator-= (
    SlotPtr_t slot
) {
    return Detach(slot);
}

template<typename RT, typename... Args>
inline bool_t
Signal<RT(Args...)>::operator-= (
    const_void_p pInstance
) {
    return Detach(pInstance);
}

typedef Signal<void()> NullSignal;

template<class Class, class RT, class... Args>
utils::SharedPtr<InternalSlot<RT, Args...>>
Slot(
    Class* pInstance,
    RT (Class::*method)(Args...),
    i32_t id = -1,
    i32_t priority = -1
) {
    return SharedPtr<InternalSlot<RT, Args...>>(
            new InternalSlot<RT, Args...>(
                    new ClassDelegate<Class, RT, Args...>(pInstance, method),
                    pInstance, id, priority));
}

template<class Class, class RT, class... Args>
utils::SharedPtr<InternalSlot<RT, Args...>>
Slot(
    Class* pInstance,
    RT (Class::*method)(Args...) const,
    i32_t id = -1,
    i32_t priority = -1
) {
    return SharedPtr<InternalSlot<RT, Args...>>(
            new InternalSlot<RT, Args...>(
                    new ConstClassDelegate<Class, RT, Args...>(pInstance, method),
                    pInstance, id, priority));
}

template<class RT, class... Args>
utils::SharedPtr<InternalSlot<RT, Args...>>
Slot(
    RT (*method)(Args...),
    i32_t id = -1,
    i32_t priority = -1
) {
    return SharedPtr<InternalSlot<RT, Args...>>(
            new InternalSlot<RT, Args...>(
                    new FunctionDelegate<RT, Args...>([method](Args... args){
                        return (*method)(std::forward<Args>(args)...);}),
                    NULL, id, priority));
}

}
}

#endif /* !LIBX_SIGNAL_HPP_ */
