/// Created by TrungTQ on 20 Jan 2017 10:40:07

#include "Libraries/Values/ValueString.hpp"

namespace libX {
namespace val {

/// @fn
/// @brief  None
/// @param  None
/// @return None
ValueString::ValueString(
    const String& strValue
) : Value (Id)
  , m_strValue (strValue) {
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
ValueString::ValueString(
    const ValueString& copied
) : Value (copied)
  , m_strValue (copied.m_strValue) {

}

/// @fn
/// @brief  None
/// @param  None
/// @return None
void
ValueString::Swap(
    ValueString& other
) {
    Value::Swap(other);
    std::swap(m_strValue, other.m_strValue);
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
ValueString::~ValueString() {
    m_strValue.clear();
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
ValueString&
ValueString::operator= (
    String strValue
) {
    ValueString temp(strValue);
    Swap(temp);
    return *this;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
ValueString&
ValueString::operator= (
    const ValueString& copied
) {
    ValueString temp(copied);
    Swap(temp);
    return *this;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
void
ValueString::Push_Back(
    char ch
) {
    m_strValue.push_back(ch);
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
void
ValueString::Pop_Back(
) {
    m_strValue.pop_back();
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
void
ValueString::Clear(
) {
    m_strValue.clear();
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
size_t
ValueString::Size(
) const {
    return m_strValue.size();
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
ValueString::Empty(
) const {
    return m_strValue.empty();
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
ValueString::operator== (
    const ValueString& rhs
) const {
    return m_strValue == rhs.m_strValue;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
ValueString::operator!= (
    const ValueString& rhs
) const {
    return m_strValue != rhs.m_strValue;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
ValueString::operator> (
    const ValueString& rhs
) const {
    return m_strValue > rhs.m_strValue;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
ValueString::operator>= (
    const ValueString& rhs
) const {
    return m_strValue >= rhs.m_strValue;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
ValueString::operator< (
    const ValueString& rhs
) const {
    return m_strValue < rhs.m_strValue;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
ValueString::operator<= (
    const ValueString& rhs
) const {
    return m_strValue <= rhs.m_strValue;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
ValueString::operator== (
    const String& strValue
) const {
    return m_strValue == strValue;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
ValueString::operator!= (
    const String& strValue
) const {
    return m_strValue != strValue;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
ValueString::operator> (
    const String& strValue
) const {
    return m_strValue > strValue;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
ValueString::operator>= (
    const String& strValue
) const {
    return m_strValue >= strValue;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
ValueString::operator< (
    const String& strValue
) const {
    return m_strValue < strValue;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
bool_t
ValueString::operator<= (
    const String& strValue
) const {
    return m_strValue <= strValue;
}

/// @fn
/// @brief  None
/// @param  None
/// @return None
ValueString::operator String(
) const {
    return m_strValue;
}

}
}
