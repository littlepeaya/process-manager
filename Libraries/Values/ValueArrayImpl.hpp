//
// Created by TrungTQ on 07/11/17.
//

#ifndef LIBX_VAL_VALUEARRAYIMPL_HPP_
#define LIBX_VAL_VALUEARRAYIMPL_HPP_

#include "Libraries/Utils/Converter.hpp"
#include "Libraries/Values/ValueArray.hpp"

namespace libX {
namespace val {

/// @fn     Empty
/// @brief  None
/// @param  None
/// @return BOOL
template<typename T>
inline bool_t
ValueArray<T>::Empty(
) const {
    return Converter::ToByte(m_lstBuff.empty());
}

template<typename T>
inline String
ValueArray<T>::ToString(
) const {
    String ret;
    if (SameType<T, u8_t>::value) {
        bool_t boFirst = TRUE;
        for (u32_t i = 0; i < m_lstBuff.size(); i++) {
            if (!boFirst) { ret.append(" "); }
            ret.append(Converter::GetHex(m_lstBuff.at(i)));
            boFirst = FALSE;
        }
    }
    return ret;
}

}
}

#endif //LIBX_VAL_VALUEARRAYIMPL_HPP_
