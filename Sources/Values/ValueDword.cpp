/// Created by TrungTQ on 20 Jan 2017 10:40:07

#include "Libraries/Values/ValueDword.hpp"

namespace libX {
namespace val {

/// @fn
/// @brief  None
/// @param  None
/// @return None
ValueDword::ValueDword(
    u32_t dwValue
) : Value (Id)
  , m_dwValue (dwValue) {
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
ValueDword::ValueDword(
    const ValueDword& copied
) : Value (copied)
  , m_dwValue (copied.m_dwValue) {
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
void
ValueDword::Swap(
    ValueDword& other
) {
    Value::Swap(other);
    std::swap(m_dwValue, other.m_dwValue);
}

/**
 * @func   ~ValueDword
 * @brief  None
 * @param  None
 * @retval None
 */
ValueDword::~ValueDword(
) {
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueDword&
ValueDword::operator= (
    ValueDword& copied
) {
    ValueDword temp(copied);
    Swap(temp);
    return *this;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
ValueDword&
ValueDword::operator= (
    u32_t dwValue
) {
    ValueDword temp(dwValue);
    Swap(temp);
    return *this;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueDword::operator== (
    const ValueDword& rhs
) const {
    return m_dwValue == rhs.m_dwValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueDword::operator== (
    const u32_t dwValue
) const {
    return m_dwValue == dwValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueDword::operator!= (
    const ValueDword& rhs
) const {
    return m_dwValue != rhs.m_dwValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueDword::operator!= (
    const u32_t dwValue
) const {
    return m_dwValue != dwValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueDword::operator>  (
    const ValueDword& rhs
) const {
    return m_dwValue > rhs.m_dwValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueDword::operator>  (
    const u32_t dwValue
) const {
    return m_dwValue > dwValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueDword::operator>= (
    const ValueDword& rhs
) const {
    return m_dwValue >= rhs.m_dwValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueDword::operator>= (
    const u32_t dwValue
) const {
    return m_dwValue >= dwValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueDword::operator<  (
    const ValueDword& rhs
) const {
    return m_dwValue < rhs.m_dwValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueDword::operator<  (
    const u32_t dwValue
) const {
    return m_dwValue < dwValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueDword::operator<= (
    const ValueDword& rhs
) const {
    return m_dwValue <= rhs.m_dwValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
ValueDword::operator<= (
    const u32_t dwValue
) const {
    return m_dwValue <= dwValue;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
operator!= (
    const u32_t lhs,
    const ValueDword& rhs
) {
    return lhs != rhs;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
operator== (
    const u32_t lhs,
    const ValueDword& rhs
) {
    return lhs == rhs;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
operator>  (
    const u32_t lhs,
    const ValueDword& rhs
) {
    return lhs > rhs;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
operator>= (
    const u32_t lhs,
    const ValueDword& rhs
) {
    return lhs >= rhs;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
operator<  (
    const u32_t lhs,
    const ValueDword& rhs
) {
    return lhs < rhs;
}

/**
 * @func
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
operator<= (
    const u32_t lhs,
    const ValueDword& rhs
) {
    return lhs <= rhs;
}

/**
 * @func   operator u32_t
 * @brief  None
 * @param  None
 * @retval None
 */
ValueDword::operator u32_t(
) const {
    return m_dwValue;
}

}
}
