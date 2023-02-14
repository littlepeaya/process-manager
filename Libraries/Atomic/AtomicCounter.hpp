//
// Created by TrungTQ on 22 Mar 2017 10:46:38
//

#ifndef LIBX_ATOMIC_COUNTER_HPP_
#define LIBX_ATOMIC_COUNTER_HPP_

#include <atomic>
#include "Libraries/Utils/Typedefs.h"

class AtomicCounter {
public:
    typedef int_t valueType;
    AtomicCounter();
    ~AtomicCounter();

    AtomicCounter(const AtomicCounter& copied);
    AtomicCounter(valueType initvalue);

    AtomicCounter& operator= (const AtomicCounter& copied);
    AtomicCounter& operator= (valueType value);

    operator valueType() const;
    valueType value() const;

    valueType operator++ ();
    valueType operator++ (int_t);

    valueType operator-- ();
    valueType operator-- (int_t);

    bool_t operator! () const;
private:
    mutable std::atomic<int_t> m_counter;
};

typedef AtomicCounter  AtomicCounter_t;
typedef AtomicCounter* AtomicCounter_p;

#endif /* !LIBX_ATOMIC_COUNTER_HPP_ */
