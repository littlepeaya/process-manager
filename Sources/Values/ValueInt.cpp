/// Created by TrungTQ on 20 Jan 2017 10:40:07

#include "Libraries/Values/ValueInt.hpp"

namespace libX {
namespace val {

/// @fn
/// @brief  None
/// @param  None
/// @return None
ValueInt::ValueInt(
    int_t iValue
) : Value (Id)
  , m_iValue (iValue) {
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
ValueInt::ValueInt(
    const ValueInt& copied
) : Value    (copied),
    m_iValue (copied.m_iValue) {

}

/// @fn
/// @brief  None
/// @param  None
/// @return None
void
ValueInt::Swap(
    ValueInt& other
) {
    Value::Swap(other);
    std::swap(m_iValue, other.m_iValue);
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
ValueInt::~ValueInt() {
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
ValueInt&
ValueInt::operator= (
    ValueInt& copied
) {
    ValueInt temp(copied);
    Swap(temp);
    return *this;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
ValueInt&
ValueInt::operator= (
    int_t iValue
) {
    ValueInt temp(iValue);
    Swap(temp);
    return *this;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
ValueInt::operator== (
    const ValueInt& rhs
) const {
    return m_iValue == rhs.m_iValue;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
ValueInt::operator== (
    const int_t iValue
) const {
    return m_iValue == iValue;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
ValueInt::operator!= (
    const ValueInt& rhs
) const {
    return m_iValue != rhs.m_iValue;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
ValueInt::operator!= (
    const int_t iValue
) const {
    return m_iValue != iValue;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
ValueInt::operator>  (
    const ValueInt& rhs
) const {
    return m_iValue > rhs.m_iValue;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
ValueInt::operator>  (
    const int_t iValue
) const {
    return m_iValue > iValue;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
ValueInt::operator>= (
    const ValueInt& rhs
) const {
    return m_iValue >= rhs.m_iValue;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
ValueInt::operator>= (
    const int_t iValue
) const {
    return m_iValue >= iValue;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
ValueInt::operator< (
    const ValueInt& rhs
) const {
    return m_iValue < rhs.m_iValue;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
ValueInt::operator< (
    const int_t iValue
) const {
    return m_iValue < iValue;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
ValueInt::operator<= (
    const ValueInt& rhs
) const {
    return m_iValue <= rhs.m_iValue;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
ValueInt::operator<= (
    const int_t iValue
) const {
    return m_iValue < iValue;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
operator!= (
    const int_t lhs,
    const ValueInt& rhs
) {
    return lhs != rhs;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
operator== (
    const int_t lhs,
    const ValueInt& rhs
) {
    return lhs == rhs;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
operator>  (
    const int_t lhs,
    const ValueInt& rhs
) {
    return lhs > rhs;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
operator>= (
    const int_t lhs,
    const ValueInt& rhs
) {
    return lhs >= rhs;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
operator<  (
    const int_t lhs,
    const ValueInt& rhs
) {
    return lhs < rhs;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
operator<= (
    const int_t lhs,
    const ValueInt& rhs
) {
    return lhs <= rhs;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
ValueInt::operator int_t(
) const {
    return m_iValue;
}

}
}
