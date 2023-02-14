/// Created by TrungTQ on 29/09/17.

#ifndef LIBX_UTILS_AUTOPTR_HPP_
#define LIBX_UTILS_AUTOPTR_HPP_

#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Utils/Exception.hpp"

namespace libX {
namespace utils {

template<class T>
class AutoPtr {
public:
    AutoPtr();
    AutoPtr(T* ptr);
    AutoPtr(T* ptr, bool_t boShared);
    template<class T1>
    AutoPtr(const AutoPtr<T1> ptr);
    AutoPtr(const AutoPtr& ptr);
    ~AutoPtr();

    AutoPtr& operator= (T* ptr);
    AutoPtr& operator= (const AutoPtr& copied);
    template<class T1>
    AutoPtr& operator= (const AutoPtr<T1>& copied);

    void Swap(AutoPtr& other);

    T* operator-> ();
    const T* operator-> () const;

    T& operator* ();
    const T& operator* () const;

    T* get();
    const T* get() const;

    operator T* ();
    operator const T* () const;

    template<class T1>
    AutoPtr<T1> Cast() const;
    template<class T1>
    AutoPtr<T1> UnsafeCast() const;

    bool_t operator !() const;
    bool_t IsNull() const;

    T* Duplicate();

    bool_t operator== (const T* ptr) const;
    bool_t operator!= (const T* ptr) const;
    bool_t operator<  (const T* ptr) const;
    bool_t operator<= (const T* ptr) const;
    bool_t operator>  (const T* ptr) const;
    bool_t operator>= (const T* ptr) const;

    bool_t operator== (const AutoPtr& rhs) const;
    bool_t operator!= (const AutoPtr& rhs) const;
    bool_t operator<  (const AutoPtr& rhs) const;
    bool_t operator<= (const AutoPtr& rhs) const;
    bool_t operator>  (const AutoPtr& rhs) const;
    bool_t operator>= (const AutoPtr& rhs) const;

private:
    T* m_ptr;
};

template<class T>
inline
AutoPtr<T>::AutoPtr() : m_ptr (nullptr) {}

template<class T>
inline
AutoPtr<T>::AutoPtr(T* ptr) : m_ptr (ptr) {}

template<class T>
inline
AutoPtr<T>::AutoPtr(
    T* ptr,
    bool_t boShared
) : m_ptr (ptr) {
    if (m_ptr != nullptr && boShared)
        m_ptr->Duplicate();
}

template<class T>
template<class T1>
inline
AutoPtr<T>::AutoPtr(
    const AutoPtr<T1> ptr
) : m_ptr (const_cast<T1*>(ptr.get())) {
    if (m_ptr != nullptr) m_ptr->Duplicate();
}

template<class T>
inline
AutoPtr<T>::AutoPtr(
    const AutoPtr& ptr
) : m_ptr (ptr.m_ptr) {
    if (m_ptr != nullptr) m_ptr->Duplicate();
}

template<class T>
inline
AutoPtr<T>::~AutoPtr(
) {
    if (m_ptr != nullptr) m_ptr->Release();
}

template<class T>
inline AutoPtr<T>&
AutoPtr<T>::operator= (
    T* ptr
) {
    if (m_ptr != ptr) {
        AutoPtr<T> temp(ptr);
        Swap(temp);
    }
    return *this;
}

template<class T>
inline AutoPtr<T>&
AutoPtr<T>::operator= (
    const AutoPtr& copied
) {
    if (m_ptr != copied.get()) {
        AutoPtr<T> temp(copied);
        Swap(temp);
    }
    return *this;
}

template<class T>
template<class T1>
inline AutoPtr<T>&
AutoPtr<T>::operator= (
    const AutoPtr<T1>& copied
) {
    if (m_ptr != copied.get()) {
        AutoPtr<T> temp(copied);
        Swap(temp);
    }
    return *this;
}

template<class T>
inline void
AutoPtr<T>::Swap(
    AutoPtr& other
) {
    std::swap(m_ptr, other.m_ptr);
}

template<class T>
template<class T1>
inline AutoPtr<T1>
AutoPtr<T>::Cast(
) const {
    T1* pOther = dynamic_cast<T1*>(m_ptr);
    return AutoPtr<T1>(pOther, TRUE);
}

template<class T>
template<class T1>
inline AutoPtr<T1>
AutoPtr<T>::UnsafeCast(
) const {
    T1* pOther = static_cast<T1*>(m_ptr);
    return AutoPtr<T1>(pOther, TRUE);
}

template<class T>
inline bool_t
AutoPtr<T>::operator !(
) const {
    return m_ptr == nullptr;
}


template<class T>
inline bool_t
AutoPtr<T>::IsNull(
) const {
    return m_ptr == nullptr;
}

template<class T>
inline T*
AutoPtr<T>::Duplicate(
) {
    if (m_ptr != nullptr) m_ptr->Duplicate();
    return m_ptr;
}

template<class T>
inline T*
AutoPtr<T>::operator-> (
) {
    if (m_ptr != nullptr) {
        return m_ptr;
    }
    THROW_MESSAGE_NULLPTR("null dereference %s", typeid(*this).name());
}

template<class T>
inline const T*
AutoPtr<T>::operator-> (
) const {
    if (m_ptr != nullptr) {
        return m_ptr;
    }
    THROW_MESSAGE_NULLPTR("null dereference %s", typeid(*this).name());
}

template<class T>
inline T&
AutoPtr<T>::operator* (
) {
    if (m_ptr != nullptr) {
        return *m_ptr;
    }
    THROW_MESSAGE_NULLPTR("null dereference %s", typeid(*this).name());
}

template<class T>
inline const T&
AutoPtr<T>::operator* (
) const {
    if (m_ptr != nullptr) {
        return m_ptr;
    }
    THROW_MESSAGE_NULLPTR("null dereference %s", typeid(*this).name());
}

template<class T>
inline T*
AutoPtr<T>::get(
) {
    return m_ptr;
}

template<class T>
inline const T*
AutoPtr<T>::get(
) const {
    return m_ptr;
}

template<class T>
inline
AutoPtr<T>::operator T* (
) {
    return m_ptr;
}

template<class T>
inline
AutoPtr<T>::operator const T* (
) const {
    return m_ptr;
}

template<class T>
inline bool_t
AutoPtr<T>::operator== (
    const T* ptr
) const {
    return m_ptr == ptr;
}

template<class T>
inline bool_t
AutoPtr<T>::operator!= (
    const T* ptr
) const {
    return m_ptr != ptr;
}

template<class T>
inline bool_t
AutoPtr<T>::operator<  (
    const T* ptr
) const {
    return m_ptr < ptr;
}

template<class T>
inline bool_t
AutoPtr<T>::operator<= (
    const T* ptr
) const {
    return m_ptr <= ptr;
}

template<class T>
inline bool_t
AutoPtr<T>::operator>  (
    const T* ptr
) const {
    return m_ptr > ptr;
}

template<class T>
inline bool_t
AutoPtr<T>::operator>= (
    const T* ptr
) const {
    return m_ptr >= ptr;
}

template<class T>
inline bool_t
AutoPtr<T>::operator== (
    const AutoPtr& rhs
) const {
    return m_ptr == rhs.get();
}

template<class T>
inline bool_t
AutoPtr<T>::operator!= (
    const AutoPtr& rhs
) const {
    return m_ptr != rhs.get();
}

template<class T>
inline bool_t
AutoPtr<T>::operator<  (
    const AutoPtr& rhs
) const {
    return m_ptr < rhs.get();
}

template<class T>
inline bool_t
AutoPtr<T>::operator<= (
    const AutoPtr& rhs
) const {
    return m_ptr <= rhs.get();
}

template<class T>
inline bool_t
AutoPtr<T>::operator>  (
    const AutoPtr& rhs
) const {
    return m_ptr > rhs.get();
}

template<class T>
inline bool_t
AutoPtr<T>::operator>= (
    const AutoPtr& rhs
) const {
    return m_ptr >= rhs.get();
}

}
}

#endif //LIBX_UTILS_AUTOPTR_HPP_
