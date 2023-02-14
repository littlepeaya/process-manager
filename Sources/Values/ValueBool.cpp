/// Created by TrungTQ on 20 Jan 2017 10:40:07

#include "Libraries/Values/ValueBool.hpp"

namespace libX {
namespace val {

/// @fn
/// @brief  None
/// @param  None
/// @return None
void
ValueBool::Swap(
    ValueBool& other
) {
    Value::Swap(other);
    std::swap(m_boValue, other.m_boValue);
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
ValueBool::ValueBool(
    bool_t boValue
) : Value (Id)
  , m_boValue (boValue) {
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
ValueBool::ValueBool(
    const ValueBool& copied
) : Value (copied)
  , m_boValue (copied.m_boValue) {

}

/// @fn
/// @brief  None
/// @param  None
/// @return None
ValueBool::~ValueBool(
) {
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
ValueBool&
ValueBool::operator= (
    bool_t boValue
) {
    ValueBool temp(boValue);
    Swap(temp);
    return *this;
}

//// @fn
/// @brief  None
/// @param  None
/// @return None
ValueBool&
ValueBool::operator= (
    const ValueBool& copied
) {
    ValueBool temp(copied);
    Swap(temp);
    return *this;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
ValueBool::operator== (
    const ValueBool& rhs
) const {
    return m_boValue == rhs.m_boValue;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
ValueBool::operator!= (
    const ValueBool& rhs
) const {
    return m_boValue != rhs.m_boValue;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
ValueBool::operator== (
    const bool_t boValue
) const {
    return m_boValue == boValue;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
ValueBool::operator!= (
    const bool_t boValue
) const {
    return m_boValue != boValue;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
operator!= (
    const bool_t lhs,
    const ValueBool& rhs
) {
    return lhs != rhs;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
operator== (
    const bool_t lhs,
    const ValueBool& rhs
) {
    return lhs == rhs;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
ValueBool::operator bool_t(
) const {
    return m_boValue;
}

}
}
