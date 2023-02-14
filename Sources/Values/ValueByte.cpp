/// Created by TrungTQ on 20 Jan 2017 10:40:07

#include "Libraries/Values/ValueByte.hpp"

namespace libX {
namespace val {

/// @fn
/// @brief  None
/// @param  None
/// @return None
ValueByte::ValueByte(
    u8_t byValue
) : Value (Id),
    m_byValue (byValue) {
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
ValueByte::ValueByte(
    const ValueByte& copied
) : Value (copied)
  , m_byValue (copied.m_byValue) {

}

/// @fn
/// @brief  None
/// @param  None
/// @return None
void
ValueByte::Swap(
    ValueByte& other
) {
    Value::Swap(other);
    std::swap(m_byValue, other.m_byValue);
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
ValueByte::~ValueByte(
) {
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
ValueByte&
ValueByte::operator= (
    const ValueByte& copied
) {
    ValueByte temp(copied);
    Swap(temp);
    return *this;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
ValueByte&
ValueByte::operator= (
    const u8_t byValue
) {
    ValueByte temp(byValue);
    Swap(temp);
    return *this;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
ValueByte::operator== (
    const ValueByte& rhs
) const {
    return m_byValue == rhs.m_byValue;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
ValueByte::operator== (
    const u8_t byValue
) const {
    return m_byValue == byValue;
}

//// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
ValueByte::operator!= (
    const ValueByte& rhs
) const {
    return m_byValue != rhs.m_byValue;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
ValueByte::operator!= (
    const u8_t byValue
) const {
    return m_byValue != byValue;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
ValueByte::operator>  (
    const ValueByte& rhs
) const {
    return m_byValue > rhs.m_byValue;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
ValueByte::operator>  (
    const u8_t byValue
) const {
    return m_byValue > byValue;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
ValueByte::operator>= (
    const ValueByte& rhs
) const {
    return m_byValue >= rhs.m_byValue;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
ValueByte::operator>= (
    const u8_t byValue
) const {
    return m_byValue >= byValue;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
ValueByte::operator<  (
    const ValueByte& rhs
) const {
    return m_byValue < rhs.m_byValue;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
ValueByte::operator<  (
    const u8_t byValue
) const {
    return m_byValue < byValue;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
ValueByte::operator<= (
    const ValueByte& rhs
) const {
    return m_byValue <= rhs.m_byValue;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
ValueByte::operator<= (
    const u8_t byValue
) const {
    return m_byValue <= byValue;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
operator!= (
    const u8_t lhs,
    const ValueByte& rhs
) {
    return lhs != rhs;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
operator== (
    const u8_t lhs,
    const ValueByte& rhs
) {
    return lhs == rhs;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
operator>  (
    const u8_t lhs,
    const ValueByte& rhs
) {
    return lhs > rhs;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
operator>= (
    const u8_t lhs,
    const ValueByte& rhs
) {
    return lhs >= rhs;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
operator<  (
    const u8_t lhs,
    const ValueByte& rhs
) {
    return lhs < rhs;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
operator<= (
    const u8_t lhs,
    const ValueByte& rhs
) {
    return lhs <= rhs;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
ValueByte::operator u8_t(
) const {
    return m_byValue;
}

}
}
