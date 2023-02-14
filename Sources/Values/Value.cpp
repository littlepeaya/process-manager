/// Created by TrungTQ on 20 Jan 2017 10:40:07

#include "Libraries/Values/Value.hpp"

namespace libX {
namespace val {

/// @fn
/// @brief  None
/// @param  None
/// @return None
void
Value::Swap(
    Value& other
) {
    std::swap(m_byValueType, other.m_byValueType);
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
Value::Value(
    u8_t byValueType
) : m_byValueType (byValueType) {
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
Value::Value(
    const Value& copied
) : m_byValueType (copied.m_byValueType) {
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
Value&
Value::operator= (
    const Value& copied
) {
    Value temp(copied);
    Swap(temp);
    return *this;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
Value::~Value(
) {
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
void
Value::SetType(
    u8_t byValueType
) {
    m_byValueType = byValueType;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
u8_t
Value::GetType(
) const {
    return m_byValueType;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
Value::operator== (
    const Value& rhs
) const {
    return m_byValueType == rhs.m_byValueType;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
Value::operator!= (
    const Value& rhs
) const {
    return m_byValueType != rhs.m_byValueType;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
Value::operator>= (
    const Value& rhs
) const {
    return m_byValueType >= rhs.m_byValueType;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
Value::operator<= (
    const Value& rhs
) const {
    return m_byValueType <= rhs.m_byValueType;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
Value::operator> (
    const Value& rhs
) const {
    return m_byValueType > rhs.m_byValueType;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
Value::operator< (
    const Value& rhs
) const {
    return m_byValueType < rhs.m_byValueType;
}

}
}
