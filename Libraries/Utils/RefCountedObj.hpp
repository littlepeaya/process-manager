//
// Created by TrungTQ on 29/09/17.
//

#ifndef LIBX_UTILS_REF_COUNTED_OBJ_HPP_
#define LIBX_UTILS_REF_COUNTED_OBJ_HPP_

#include "Libraries/Utils/Core.hpp"
#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Atomic/AtomicCounter.hpp"

namespace libX {
namespace utils {

class RefCountedObj {
private:
    DISABLE_COPY(RefCountedObj)
protected:
    mutable AtomicCounter m_counter;

    /// @fn     ~RefCountedObj
    /// @brief  None
    /// @param  None
    /// @retval None
    virtual ~RefCountedObj();
public:
    /// @fn     RefCountedObj
    /// @brief  None
    /// @param  None
    /// @retval None
    RefCountedObj();

    /// @fn     Duplicate
    /// @brief  None
    /// @param  None
    /// @retval None
    void Duplicate() const;

    /// @fn     Release
    /// @brief  None
    /// @param  None
    /// @retval None
    void Release() const;

    /// @fn     ReferenceCount
    /// @brief  None
    /// @param  None
    /// @retval None
    int_t ReferenceCount() const;
};

/// @fn     RefCountedObj
/// @brief  None
/// @param  None
/// @retval None
inline
RefCountedObj::RefCountedObj(
) : m_counter (1) {
}

/// @fn     ~RefCountedObj
/// @brief  None
/// @param  None
/// @retval None
inline
RefCountedObj::~RefCountedObj(
) {
}

/// @fn     Duplicate
/// @brief  None
/// @param  None
/// @retval None
inline void
RefCountedObj::Duplicate(
) const {
    m_counter++;
}

/// @fn     Release
/// @brief  None
/// @param  None
/// @retval None
inline void
RefCountedObj::Release(
) const {
    if (--m_counter == 0) {
        delete this;
    }
}

/// @fn     ReferenceCount
/// @brief  None
/// @param  None
/// @retval None
inline int_t
RefCountedObj::ReferenceCount(
) const {
    return m_counter.value();
}

}
}

#endif //LIBX_UTILS_REF_COUNTED_OBJ_HPP_
