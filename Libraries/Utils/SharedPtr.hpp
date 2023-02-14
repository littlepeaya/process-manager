//
// Created by TrungTQ on 3 Feb 2017 18:04:45
//

#ifndef LIBX_UTILS_SHAREPTR_HPP_
#define LIBX_UTILS_SHAREPTR_HPP_

#include <assert.h>
#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Utils/Exception.hpp"
#include "Libraries/Atomic/AtomicCounter.hpp"

namespace libX {
namespace utils {

class RefCounter {
private:
    mutable AtomicCounter m_iRef;
public:
    RefCounter();
    ~RefCounter();
    void IncRef() const;
    int_t DecRef() const;
    int_t Count() const;
};

inline RefCounter::RefCounter(
) : m_iRef (1) {
}

inline RefCounter::~RefCounter(
) {}

inline void
RefCounter::IncRef(
) const {
    ++m_iRef;
}

inline int_t
RefCounter::DecRef(
) const {
    return --m_iRef;
}

inline int_t
RefCounter::Count(
) const {
    return m_iRef.value();
}

template <typename T>
struct Release {
    static inline void Cleanup(T* ptr) {
        /// Enforce a complete type.
        typedef char IsIncompleteType[sizeof(T) ? 1 : -1];
        (void) sizeof(IsIncompleteType);
        delete ptr;
    }
};

template <typename T>
struct ReleaseArray {
    static inline void Cleanup(T* ptr) {
        /// Enforce a complete type.
        typedef char IsIncompleteType[sizeof(T) ? 1 : -1];
        (void) sizeof(IsIncompleteType);
        delete [] ptr;
    }
};

template<class T, class Counter = RefCounter, class Cleanup = Release<T>>
class SharedPtr {
private:
    T* m_ptr;
    Counter* m_pCounter;

public:
    SharedPtr();
    SharedPtr(T* ptr);
    SharedPtr(const SharedPtr& copied);
    SharedPtr(T* ptr, Counter* pCounter);

    template<class T1, class Cleanup1>
    SharedPtr(const SharedPtr<T1, Counter, Cleanup1>& copied);

    ~SharedPtr();

    SharedPtr& operator= (T* ptr);
    SharedPtr& operator= (const SharedPtr& copied);
    template<class T1, class Cleanup1>
    SharedPtr& operator= (const SharedPtr<T1, Counter, Cleanup1>& copied);

    void Swap(SharedPtr& other);

    T* operator-> ();
    const T* operator-> () const;

    T& operator*  ();
    const T& operator*  () const;

    T* get();
    const T* get() const;

    operator T*();
    operator const T*() const;

    bool_t operator !() const;
    bool_t IsNull() const;

    const Counter* counter() const;

    template<class T1>
    SharedPtr<T1, Counter, Cleanup> Cast() const;

    template<class T1>
    SharedPtr<T1, Counter, Cleanup> UnSafeCast() const;

    bool_t operator== (const T* ptr) const;
    bool_t operator!= (const T* ptr) const;
    bool_t operator<  (const T* ptr) const;
    bool_t operator<= (const T* ptr) const;
    bool_t operator>  (const T* ptr) const;
    bool_t operator>= (const T* ptr) const;

    bool_t operator== (const SharedPtr& rhs) const;
    bool_t operator!= (const SharedPtr& rhs) const;
    bool_t operator<  (const SharedPtr& rhs) const;
    bool_t operator<= (const SharedPtr& rhs) const;
    bool_t operator>  (const SharedPtr& rhs) const;
    bool_t operator>= (const SharedPtr& rhs) const;
};

template<class T, class Counter, class Cleanup>
inline
SharedPtr<T, Counter, Cleanup>::SharedPtr(
    T* ptr,
    Counter* pCounter
) : m_ptr (ptr)
  , m_pCounter (pCounter) {
    assert(m_pCounter);
    m_pCounter->IncRef();
}

template<class T, class Counter, class Cleanup>
inline
SharedPtr<T, Counter, Cleanup>::SharedPtr(
) : m_ptr (NULL)
  , m_pCounter (new Counter()) {
}

template<class T, class Counter, class Cleanup>
inline
SharedPtr<T, Counter, Cleanup>::SharedPtr(
    T* ptr
) : m_ptr (ptr)
  , m_pCounter (new Counter) {
}

template<class T, class Counter, class Cleanup>
inline
SharedPtr<T, Counter, Cleanup>::SharedPtr(
    const SharedPtr& copied
) : m_ptr (copied.m_ptr)
  , m_pCounter (copied.m_pCounter) {
    assert(m_pCounter);
    m_pCounter->IncRef();
}

template<class T, class Counter, class Cleanup>
template<class T1, class Cleanup1>
inline
SharedPtr<T, Counter, Cleanup>::SharedPtr(
    const SharedPtr<T1, Counter, Cleanup1>& copied
) : m_ptr (const_cast<T1*>(copied.get()))
  , m_pCounter (const_cast<Counter*>(copied.counter())) {
    assert(m_pCounter);
    m_pCounter->IncRef();
}

template<class T, class Counter, class Cleanup>
inline
SharedPtr<T, Counter, Cleanup>::~SharedPtr(
) {
    assert(m_pCounter);
    int_t i = m_pCounter->DecRef();
    if (i == 0) {
        if (m_ptr != NULL) {
            Cleanup::Cleanup(m_ptr);
        }
        m_ptr = NULL;
        delete m_pCounter;
        m_pCounter = NULL;
    }
}

template<class T, class Counter, class Cleanup>
inline SharedPtr<T, Counter, Cleanup>&
SharedPtr<T, Counter, Cleanup>::operator= (
    T* ptr
) {
    if (m_ptr != ptr) {
        SharedPtr temp(ptr);
        Swap(temp);
    }
    return *this;
}

template<class T, class Counter, class Cleanup>
inline SharedPtr<T, Counter, Cleanup>&
SharedPtr<T, Counter, Cleanup>::operator= (
    const SharedPtr& copied
) {
    if (m_ptr != copied.get()) {
        SharedPtr temp(copied);
        Swap(temp);
    }
    return *this;
}

template<class T, class Counter, class Cleanup>
template<class T1, class Cleanup1>
inline SharedPtr<T, Counter, Cleanup>&
SharedPtr<T, Counter, Cleanup>::operator= (
    const SharedPtr<T1, Counter, Cleanup1>& copied
) {
    if (m_ptr != copied.get()) {
        SharedPtr temp(copied);
        Swap(temp);
    }
    return *this;
}

template<class T, class Counter, class Cleanup>
inline void
SharedPtr<T, Counter, Cleanup>::Swap(
    SharedPtr& other
) {
    std::swap(m_ptr, other.m_ptr);
    std::swap(m_pCounter, other.m_pCounter);
}

template<class T, class Counter, class Cleanup>
inline T*
SharedPtr<T, Counter, Cleanup>::operator-> (
) {
    if (m_ptr != NULL) {
        return m_ptr;
    }
    THROW_MESSAGE_NULLPTR("null dereference %s", typeid(*this).name());
};


template<class T, class Counter, class Cleanup>
inline const T*
SharedPtr<T, Counter, Cleanup>::operator-> (
) const {
    if (m_ptr != NULL) {
        return m_ptr;
    }
    THROW_MESSAGE_NULLPTR("null dereference %s", typeid(*this).name());
}

template<class T, class Counter, class Cleanup>
T&
SharedPtr<T, Counter, Cleanup>::operator*  (
) {
    if (m_ptr != NULL) {
        return *m_ptr;
    }
    THROW_MESSAGE_NULLPTR("null dereference %s", typeid(*this).name());
};

template<class T, class Counter, class Cleanup>
inline const T&
SharedPtr<T, Counter, Cleanup>::operator*  (
) const {
    if (m_ptr != NULL) {
        return *m_ptr;
    }
    THROW_MESSAGE_NULLPTR("null dereference %s", typeid(*this).name());
}

template<class T, class Counter, class Cleanup>
inline T*
SharedPtr<T, Counter, Cleanup>::get(
) {
    return m_ptr;
};

template<class T, class Counter, class Cleanup>
inline const T*
SharedPtr<T, Counter, Cleanup>::get(
) const {
    return m_ptr;
}

template<class T, class Counter, class Cleanup>
inline
SharedPtr<T, Counter, Cleanup>::operator T*(
) {
    return m_ptr;
}

template<class T, class Counter, class Cleanup>
inline
SharedPtr<T, Counter, Cleanup>::operator const T*(
) const {
    return m_ptr;
}

template<class T, class Counter, class Cleanup>
inline bool_t
SharedPtr<T, Counter, Cleanup>::operator !(
) const {
    return m_ptr == NULL;
}

template<class T, class Counter, class Cleanup>
inline bool_t
SharedPtr<T, Counter, Cleanup>::IsNull(
) const {
    return m_ptr == NULL;
}

template<class T, class Counter, class Cleanup>
template<class T1>
inline SharedPtr<T1, Counter, Cleanup>
SharedPtr<T, Counter, Cleanup>::Cast(
) const {
    T1* pOther = dynamic_cast<T1*>(m_ptr);
    if (pOther != NULL) {
        return SharedPtr<T1, Counter, Cleanup>(pOther, m_pCounter);
    }
    return SharedPtr<T1, Counter, Cleanup>();
}

template<class T, class Counter, class Cleanup>
template<class T1>
inline SharedPtr<T1, Counter, Cleanup>
SharedPtr<T, Counter, Cleanup>::UnSafeCast(
) const {
    T1* pOther = static_cast<T1*>(m_ptr);
    return SharedPtr<T1, Counter, Cleanup>(pOther, m_pCounter);
}

template<class T, class Counter, class Cleanup>
inline const Counter*
SharedPtr<T, Counter, Cleanup>::counter(
) const {
    return m_pCounter;
}

template<class T, class Counter, class Cleanup>
inline bool_t
SharedPtr<T, Counter, Cleanup>::operator== (
    const T* ptr
) const {
    return m_ptr == ptr;
}

template<class T, class Counter, class Cleanup>
inline bool_t
SharedPtr<T, Counter, Cleanup>::operator!= (
    const T* ptr
) const {
    return m_ptr != ptr;
}

template<class T, class Counter, class Cleanup>
inline bool_t
SharedPtr<T, Counter, Cleanup>::operator<  (
    const T* ptr
) const {
    return m_ptr < ptr;
}

template<class T, class Counter, class Cleanup>
inline bool_t
SharedPtr<T, Counter, Cleanup>::operator<= (
    const T* ptr
) const {
    return m_ptr <= ptr;
}

template<class T, class Counter, class Cleanup>
inline bool_t
SharedPtr<T, Counter, Cleanup>::operator>  (
    const T* ptr
) const {
    return m_ptr > ptr;
}

template<class T, class Counter, class Cleanup>
inline bool_t
SharedPtr<T, Counter, Cleanup>::operator>= (
    const T* ptr
) const {
    return m_ptr >= ptr;
}

template<class T, class Counter, class Cleanup>
inline bool_t
SharedPtr<T, Counter, Cleanup>::operator== (
    const SharedPtr& rhs
) const {
    return m_ptr == rhs.get();
}

template<class T, class Counter, class Cleanup>
inline bool_t
SharedPtr<T, Counter, Cleanup>::operator!= (
    const SharedPtr& rhs
) const {
    return m_ptr != rhs.get();
}

template<class T, class Counter, class Cleanup>
inline bool_t
SharedPtr<T, Counter, Cleanup>::operator<  (
    const SharedPtr& rhs
) const {
    return m_ptr < rhs.get();
}

template<class T, class Counter, class Cleanup>
inline bool_t
SharedPtr<T, Counter, Cleanup>::operator<= (
    const SharedPtr& rhs
) const {
    return m_ptr <= rhs.get();
}

template<class T, class Counter, class Cleanup>
inline bool_t
SharedPtr<T, Counter, Cleanup>::operator>  (
    const SharedPtr& rhs
) const {
    return m_ptr > rhs.get();
}

template<class T, class Counter, class Cleanup>
inline bool_t
SharedPtr<T, Counter, Cleanup>::operator>= (
    const SharedPtr& rhs
) const {
    return m_ptr >= rhs.get();
}

template<class T, class Counter = RefCounter, class Cleanup = Release<T>>
inline SharedPtr<T, Counter, Cleanup>
MakeShare(
    T* ptr
) {
    return SharedPtr<T, Counter, Cleanup>(ptr);
}

#define MAKE_TYPE_SHAREPTR(Type, TypeSuffix)      \
typedef SharedPtr<Type> Share##TypeSuffix##_t;

}
}

#endif /* !LIBX_UTILS_SHAREPTR_HPP_ */
