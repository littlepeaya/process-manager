/// Created by TrungTQ on 20 Jan 2017 10:40:07

#include "Libraries/Values/ValueWord.hpp"

namespace libX {
namespace val {

/// @func   ValueWord
/// @brief  None
/// @param  None
/// @retval None
ValueWord::ValueWord(
    u16_t wValue
) : Value (Id)
  , m_wValue (wValue) {
}

/// @func   ValueWord
/// @brief  None
/// @param  None
/// @retval None
ValueWord::ValueWord(
    const ValueWord& copied
) : Value (copied)
  , m_wValue (copied.m_wValue) {
}

/// @func   Swap
/// @brief  None
/// @param  None
/// @retval None
void
ValueWord::Swap(
    ValueWord& other
) {
    Value::Swap(other);
    std::swap(m_wValue, other.m_wValue);
}

/// @func   ~ValueWord
/// @brief  None
/// @param  None
/// @retval None
ValueWord::~ValueWord(
) {
}

/// @func
/// @brief  None
/// @param  None
/// @retval None
ValueWord&
ValueWord::operator= (
    ValueWord& copied
) {
    ValueWord temp(copied);
    Swap(temp);
    return *this;
}

/// @func
/// @brief  None
/// @param  None
/// @retval None
ValueWord&
ValueWord::operator= (
    u16_t wValue
) {
    m_wValue = wValue;
    return *this;
}

/// @func
/// @brief  None
/// @param  None
/// @retval None
bool_t
ValueWord::operator== (
    const ValueWord& rhs
) const {
    return m_wValue == rhs.m_wValue;
}

/// @func
/// @brief  None
/// @param  None
/// @retval None
bool_t
ValueWord::operator== (
    const u16_t wValue
) const {
    return m_wValue == wValue;
}

/// @func
/// @brief  None
/// @param  None
/// @retval None
bool_t
ValueWord::operator!= (
    const ValueWord& rhs
) const {
    return m_wValue != rhs.m_wValue;
}

/// @func
/// @brief  None
/// @param  None
/// @retval None
bool_t
ValueWord::operator!= (
    const u16_t wValue
) const {
    return m_wValue != wValue;
}

/// @func
/// @brief  None
/// @param  None
/// @retval None
bool_t
ValueWord::operator> (
    const ValueWord& rhs
) const {
    return m_wValue > rhs.m_wValue;
}

/// @func
/// @brief  None
/// @param  None
/// @retval None
bool_t
ValueWord::operator> (
    const u16_t wValue
) const {
    return m_wValue > wValue;
}

/// @func
/// @brief  None
/// @param  None
/// @retval None
bool_t
ValueWord::operator>= (
    const ValueWord& rhs
) const {
    return m_wValue >= rhs.m_wValue;
}

/// @func
/// @brief  None
/// @param  None
/// @retval None
bool_t
ValueWord::operator>= (
    const u16_t wValue
) const {
    return m_wValue >= wValue;
}


/// @func
/// @brief  None
/// @param  None
/// @retval None
bool_t
ValueWord::operator< (
    const ValueWord& rhs
) const {
    return m_wValue < rhs.m_wValue;
}

/// @func
/// @brief  None
/// @param  None
/// @retval None
bool_t
ValueWord::operator< (
    const u16_t wValue
) const {
    return m_wValue < wValue;
}

/// @func
/// @brief  None
/// @param  None
/// @retval None
bool_t
ValueWord::operator<= (
    const ValueWord& rhs
) const {
    return m_wValue < rhs.m_wValue;
}

/// @func
/// @brief  None
/// @param  None
/// @retval None
bool_t
ValueWord::operator<= (
    const u16_t wValue
) const {
    return m_wValue <= wValue;
}

/// @func
/// @brief  None
/// @param  None
/// @retval None
bool_t
operator!= (
    const u16_t lhs,
    const ValueWord& rhs
) {
    return lhs != rhs;
}

/// @func
/// @brief  None
/// @param  None
/// @retval None
bool_t
operator== (
    const u16_t lhs,
    const ValueWord& rhs
) {
    return lhs == rhs;
}

/// @func
/// @brief  None
/// @param  None
/// @retval None
bool_t
operator>  (
    const u16_t lhs,
    const ValueWord& rhs
) {
    return lhs > rhs;
}

/// @func
/// @brief  None
/// @param  None
/// @retval None
bool_t
operator>= (
    const u16_t lhs,
    const ValueWord& rhs
) {
    return lhs >= rhs;
}

/// @func
/// @brief  None
/// @param  None
/// @retval None
bool_t
operator<  (
    const u16_t lhs,
    const ValueWord& rhs
) {
    return lhs < rhs;
}

/// @func
/// @brief  None
/// @param  None
/// @retval None
bool_t
operator<= (
    const u16_t lhs,
    const ValueWord& rhs
) {
    return lhs <= rhs;
}

/// @func   u16_t
/// @brief  None
/// @param  None
/// @retval None
ValueWord::operator u16_t(
) const {
    return m_wValue;
}

}
}
