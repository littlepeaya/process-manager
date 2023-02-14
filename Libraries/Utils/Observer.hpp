//
// Created by TrungTQ on 17 Oct 2017 10:46:13
//

#ifndef LIBX_UTILS_OBSERVER_HPP_
#define LIBX_UTILS_OBSERVER_HPP_

#include "Libraries/Utils/IObserver.hpp"
#include "Locker.hpp"
#include "AutoLocker.hpp"

namespace libX {
namespace utils {

template<class C, class N>
class Observer : public IObserver {
public:
    typedef void (C::*Callback)(N*);

    Observer(C* pObj, Callback method);

    Observer(const Observer& copied);

    virtual ~Observer();

    Observer& operator = (const Observer& copied);

    virtual void Notify(Notification* pNf) const;
private:
    C* m_pObj;
    Callback m_method;
    utils::Locker_t m_locker;
};

template<class C, class N>
inline
Observer<C, N>::Observer(
    C* pObj,
    Callback method
) : m_pObj (pObj)
  , m_method (method) {
}

template<class C, class N>
inline Observer<C, N>::Observer(
    const Observer& copied
) : m_pObj (copied.m_pObj)
  , m_method (copied.m_method) {}

template<class C, class N>
inline Observer<C, N>::~Observer() {}

template<class C, class N>
inline Observer<C, N>&
Observer<C, N>::operator = (
    const Observer& copied
) {
    if (this != &copied) {
        m_pObj = copied.m_pObj;
        m_method = copied.m_method;
    }
    return *this;
}

template<class C, class N>
inline void
Observer<C, N>::Notify(
    Notification* pNf
) const {
    utils::AutoLocker<utils::Locker> locker(m_locker);
    if (m_pObj != NULL) {
        N* pCastNf = dynamic_cast<N*>(pNf);
        if (pCastNf != NULL) {
            pCastNf->Duplicate();
            (m_pObj->*m_method)(pCastNf);
        }
    }
}

}
}

#endif /* LIBX_UTILS_OBSERVER_HPP_ */
