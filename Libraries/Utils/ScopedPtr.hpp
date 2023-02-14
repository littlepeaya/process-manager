//
// Created by TrungTQ on 07/08/17.
//

#ifndef LIBX_UTILS_SCOPEDPTR_HPP_
#define LIBX_UTILS_SCOPEDPTR_HPP_

#include <assert.h>
#include <utility>
#include "Libraries/Utils/Typedefs.h"

namespace libX {
namespace utils {

#define SCOPED_PTR_ASSERT(x)    assert(x != NULL)

template <typename T>
struct PtrDeleter {
    static inline void Cleanup(T* pTr) {
        /// Enforce a complete type.
        typedef char IsIncompleteType[sizeof(T) ? 1 : -1];
        (void) sizeof(IsIncompleteType);
        delete pTr;
    }
};

template <typename T>
struct PtrArrayDeleter {
    static inline void Cleanup(T* pTr) {
        /// Enforce a complete type.
        typedef char IsIncompleteType[sizeof(T) ? 1 : -1];
        (void) sizeof(IsIncompleteType);
        delete [] pTr;
    }
};

template<typename T, typename Cleanup = PtrDeleter<T>>
class ScopedPtr {
public:
    explicit ScopedPtr(T* p = NULL) noexcept;

    virtual ~ScopedPtr();

    T& operator*() const;

    T* operator->() const noexcept;

    bool_t operator!() const noexcept;

    inline operator bool() const;

    T* Data() const noexcept;

    bool_t IsNull() const noexcept;

    void Reset(T* other = NULL) noexcept(noexcept(Cleanup::Cleanup(std::declval<T*>())));

    T* Take() noexcept;

    void Swap(ScopedPtr<T, Cleanup>& other) noexcept;

    bool_t operator== (const ScopedPtr<T, Cleanup>& rhs) noexcept;

    bool_t operator!= (const ScopedPtr<T, Cleanup>& rhs) noexcept;

    typedef T* pointer;

protected:
    T* m_pTr;
private:
    DISABLE_COPY(ScopedPtr)
};

template <class T, class Cleanup>
inline
ScopedPtr<T, Cleanup>::ScopedPtr(
    T* pTr
) noexcept : m_pTr(pTr) {
}

template <class T, class Cleanup>
inline
ScopedPtr<T, Cleanup>::~ScopedPtr(
) {
    T* pOld = this->m_pTr;
    Cleanup::Cleanup(pOld);
}

template <class T, class Cleanup>
inline T&
ScopedPtr<T, Cleanup>::operator*(
) const {
    SCOPED_PTR_ASSERT(m_pTr);
    return *m_pTr;
}

template <class T, class Cleanup>
inline T*
ScopedPtr<T, Cleanup>::operator->(
) const noexcept {
    return m_pTr;
}

template <class T, class Cleanup>
inline bool_t
ScopedPtr<T, Cleanup>::operator!(
) const noexcept {
    return !m_pTr;
}

template <class T, class Cleanup>
inline
ScopedPtr<T, Cleanup>::operator bool(
) const {
    return IsNull() ? NULL : &ScopedPtr::m_pTr;
}

template <class T, class Cleanup>
inline T*
ScopedPtr<T, Cleanup>::Data(
) const noexcept {
    return m_pTr;
}

template <class T, class Cleanup>
bool_t
ScopedPtr<T, Cleanup>::IsNull(
) const noexcept {
    return m_pTr == NULL;
}

template <class T, class Cleanup>
inline void
ScopedPtr<T, Cleanup>::Reset(
    T* other
) noexcept(noexcept(Cleanup::Cleanup(std::declval<T*>()))) {
    if (m_pTr == other)
        return;
    T* pOld = m_pTr;
    m_pTr = other;
    Cleanup::Cleanup(pOld);
}

template <class T, class Cleanup>
inline T*
ScopedPtr<T, Cleanup>::Take(
) noexcept {
    T* pOld = m_pTr;
    m_pTr = NULL;
    return pOld;
}

template <class T, class Cleanup>
inline void
ScopedPtr<T, Cleanup>::Swap(
    ScopedPtr<T, Cleanup>& other
) noexcept {
    std::swap(m_pTr, other.m_pTr);
}

template <class T, class Cleanup>
inline bool_t
ScopedPtr<T, Cleanup>::operator==(
    const ScopedPtr<T, Cleanup> &rhs
) noexcept {
    return m_pTr == rhs.m_pTr;
}

template <class T, class Cleanup>
inline bool_t
ScopedPtr<T, Cleanup>::operator!=(
    const ScopedPtr<T, Cleanup> &rhs
) noexcept {
    return m_pTr != rhs.m_pTr;
}

}
}

#endif //LIBX_UTILS_SCOPEDPTR_HPP_
