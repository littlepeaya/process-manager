//
// Created by TrungTQ on 20 Jan 2017 10:40:07
//

#ifndef LIBX_VAL_VALUEARRAY_HPP_
#define LIBX_VAL_VALUEARRAY_HPP_

#include <cstring>
#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Utils/String.hpp"
#include "Libraries/Utils/Vector.hpp"
#include "Libraries/Utils/Exception.hpp"
#include "Libraries/Utils/SameType.hpp"
#include "Libraries/Values/Value.hpp"
#include "Libraries/Values/ValueDefault.hpp"
#include "Libraries/Values/Enums/TypeValues.hpp"

namespace libX {
namespace val {

template<typename T>
class ValueArray : public Value {
protected:
    /// @fn     Swap
    /// @brief  Swap data
    /// @param  other
    /// @return None
    void Swap(ValueArray& other);

public:
    typedef Vector<T> lstBuff_t;
    enum { Id = (u8_t) TypeValues::type_array };

    /// @fn     ValueArray
    /// @brief  Constructor
    /// @param  None
    /// @return Object
    ValueArray();

    /// @fn     ValueArray
    /// @brief  Constructor
    /// @param  DOWRD size
    /// @return Object
    explicit ValueArray(u32_t size);

    /// @fn     ValueArray
    /// @brief  Constructor
    /// @param  DOWRD size
    /// @param  T value
    /// @return Object
    explicit ValueArray(u32_t size, T value);

    /// @fn     ValueArray
    /// @brief  Constructor
    /// @param  T* pBuffer
    /// @param  DOWRD size
    /// @return Object
    explicit ValueArray(T* pBuffer, u32_t size);

    /// @fn     ValueArray
    /// @brief  Constructor
    /// @param  lstBuff
    /// @return Object
    explicit ValueArray(const lstBuff_t& lstBuff);

    /// @fn     ValueArray
    /// @brief  Constructor
    /// @param  T& rBuffer
    /// @return Object
    template<u32_t S>
    explicit ValueArray(const T (&rBuffer)[S]);

    /// @fn     ValueArray
    /// @brief  Copy Constructor
    /// @param  ARRAY copied
    /// @return Object
    ValueArray(const ValueArray& copied);

    /// @fn     operator=
    /// @brief  Assignment
    /// @param  ARRAY copied
    /// @return Object
    ValueArray& operator= (const ValueArray& copied);

    /// @fn     operator=
    /// @brief  Assignment
    /// @param  T value
    /// @return Object
    ValueArray& operator= (const T& value);

    /// @fn     ~ValueArray
    /// @brief  Destructor
    /// @param  None
    /// @return None
    virtual ~ValueArray();

    /// @fn     Size
    /// @brief  Download size of array
    /// @param  None
    /// @return DWORD size of array
    u32_t Size() const;

    /// @fn     Empty
    /// @brief  None
    /// @param  None
    /// @return BOOL
    bool_t Empty() const;

    /// @fn     GetValue
    /// @brief  Download value in specific index
    /// @param  DWORD index
    /// @return T value
    T GetValue(u32_t index) const;

    /// @fn     Operator []
    /// @brief  Download value in specific index
    /// @param  DWORD index
    /// @return T& value
    T& operator[] (u32_t index);

    /// @fn     Operator ()
    /// @brief  Download value in specific index
    /// @param  DWORD index
    /// @return T& value
    T& operator() (u32_t index);

    /// @fn     SetValue
    /// @brief  Set value in specific index
    /// @param  T value
    /// @param  DWORD index
    /// @return None
    void SetValue(T value, u32_t index);

    /// @fn     CopyTo
    /// @brief  Copy
    /// @param  ValueArray<T> rArray
    /// @param  DWORD begin
    /// @return None
    void CopyTo(ValueArray<T>& array, u32_t begin) const;

    /// @fn     CopyTo
    /// @brief  Copy
    /// @param  T* pArray
    /// @param  DWORD begin
    /// @return None
    void CopyTo(T* pArray, u32_t begin) const;

    /// @fn     operator<<
    /// @brief  operator<<
    /// @param  T value
    /// @return None
    ValueArray& operator<< (T value);

    /// @fn     operator,
    /// @brief  operator,
    /// @param  T value
    /// @return None
    ValueArray& operator, (T value);

    /// @fn     operator==
    /// @brief  None
    /// @param  VALUEARRAY<T> rhs
    /// @return BOOL
    bool_t operator== (const ValueArray<T>& rhs) const;

    /// @fn     Contain
    /// @brief  None
    /// @param  VALUEARRAY<T> value
    /// @return BOOL
    bool_t Contain(const ValueArray<T>& value) const;

    /// @fn      ToString
    /// @brief  None
    /// @param  None
    /// @return None
    String ToString() const;

    /// @fn     Clear
    /// @brief  None
    /// @param  None
    /// @return None
    void Clear();

    /// @fn     AddRangeTo
    /// @brief  None
    /// @param  data
    /// @return None
    void AddRangeTo(Vector<T>& data) const;

    /// @fn     Copy
    /// @brief  None
    /// @tparam T
    /// @param  src
    /// @param  srcIndex
    /// @param  dst
    /// @param  dstIndex
    /// @param  length
    /// @return None
    static void Copy(ValueArray<T>& src, u32_t srcIndex, ValueArray<T>& dst, u32_t dstIndex, u32_t length);

    /// @fn     Copy
    /// @brief  None
    /// @tparam T
    /// @param  src
    /// @param  srcIndex
    /// @param  dst
    /// @param  dstIndex
    /// @param  length
    /// @return None
    static void Copy(Vector<T>& src, u32_t srcIndex, ValueArray<T>& dst, u32_t dstIndex, u32_t length);

    /// @fn     Reverse
    /// @brief  None
    /// @tparam T
    /// @return None
    void Reverse();

private:
    lstBuff_t   m_lstBuff;
    u32_t       m_dwCount;
};

/// @fn     ValueArray
/// @brief  Constructor
/// @param  None
/// @return Object
template<typename T>
inline
ValueArray<T>::ValueArray(
) : Value (Id)
  , m_dwCount (0) {
}

/// @fn     ValueArray
/// @brief  Constructor
/// @param  DOWRD dwSize
/// @return Object
template<typename T>
inline
ValueArray<T>::ValueArray(
    u32_t size
) : Value (Id)
  , m_lstBuff (size, ValueDefault<T>::Default())
  , m_dwCount (0) {
}

/// @fn     ValueArray
/// @brief  Constructor
/// @param  DOWRD dwSize
/// @param  T value
/// @return Object
template<typename T>
inline
ValueArray<T>::ValueArray(
    u32_t size,
    T value
) : Value (Id)
  , m_lstBuff (size, value)
  , m_dwCount (size) {
}

/// @fn     ValueArray
/// @brief  Constructor
/// @param  T* pBuffer
/// @param  DOWRD dwSize
/// @return Object
template<typename T>
inline
ValueArray<T>::ValueArray(
    T* pBuffer,
    u32_t size
) : Value (Id)
  , m_lstBuff (pBuffer, pBuffer + size)
  , m_dwCount (size) {
}

/// @fn     ValueArray
/// @brief  Constructor
/// @param  lstBuff
/// @return Object
template<typename T>
inline
ValueArray<T>::ValueArray(
    const lstBuff_t& lstBuff
) : Value (Id)
  , m_lstBuff (lstBuff)
  , m_dwCount ((u32_t) lstBuff.size()) {
}

/// @fn     ValueArray
/// @brief  Constructor
/// @param  const T (&rBuffer)[S]
/// @param  DOWRD dwLength
/// @return Ojbect
template<typename T>
template<u32_t S>
inline
ValueArray<T>::ValueArray(
    const T (&rBuffer)[S]
) : Value (Id)
  , m_lstBuff (rBuffer, rBuffer + S)
  , m_dwCount (S) {
}

/// @fn     Swap
/// @brief  Swap data
/// @param  other
/// @return None
template<typename T>
inline void
ValueArray<T>::Swap(
    ValueArray& other
) {
    Value::Swap(other);
    std::swap(m_lstBuff, other.m_lstBuff);
    std::swap(m_dwCount, other.m_dwCount);
}

/// @fn     ValueArray
/// @brief  Copy Constructor
/// @param  ARRAY copied
/// @return Object
template<typename T>
inline
ValueArray<T>::ValueArray(
    const ValueArray& copied
) : Value (copied)
  , m_lstBuff (copied.m_lstBuff)
  , m_dwCount (copied.m_dwCount) {
}

/// @fn     ValueArray
/// @brief  Assignment
/// @param  ARRAY copied
/// @return Object
template<typename T>
inline ValueArray<T>&
ValueArray<T>::operator= (
    const ValueArray& copied
) {
    ValueArray<T> temp(copied);
    Swap(temp);
    return *this;
}

/// @fn     operator=
/// @brief  Assignment
/// @param  T value
/// @return Object
template<typename T>
inline ValueArray<T>&
ValueArray<T>::operator= (
    const T& value
) {
    for (size_t i = 0; i < m_lstBuff.size(); i++) {
        m_lstBuff[i] = value;
    }
    return *this;
}

/// @fn     ~ValueArray
/// @brief  Destructor
/// @param  None
/// @return None
template<typename T>
inline
ValueArray<T>::~ValueArray(
) {
}

/// @fn     Length
/// @brief  Download length of array
/// @param  None
/// @return BYTE length of array
template<typename T>
inline u32_t
ValueArray<T>::Size(
) const {
    return (u32_t) m_lstBuff.size();
}

/// @fn     GetValue
/// @brief  Download value in specific index
/// @param  DWORD dwIndex
/// @return T value
template<typename T>
inline T
ValueArray<T>::GetValue(
    u32_t index
) const {
    if (index < m_lstBuff.size())
        return m_lstBuff[index];
    THROW_MESSAGE("out of bound");
}

/// @fn     Operator []
/// @brief  Download value in specific index
/// @param  DWORD dwIndex
/// @return T value
template<typename T>
inline T&
ValueArray<T>::operator[] (
    u32_t index
) {
    if (index < m_lstBuff.size())
        return m_lstBuff[index];
    THROW_MESSAGE("out of bound");
}

/// @fn     Operator ()
/// @brief  Download value in specific index
/// @param  DWORD idwRow
/// @return T& value
template<typename T>
inline T&
ValueArray<T>::operator() (
    u32_t index
) {
    if (index < m_lstBuff.size())
        return m_lstBuff[index];
    THROW_MESSAGE("out of bound");
}

/// @fn     SetValue
/// @brief  Set value in specific index
/// @param  T value
/// @param  DWORD dwIndex
/// @return None
template<typename T>
inline void
ValueArray<T>::SetValue(
    T value,
    u32_t index
) {
    m_lstBuff[index] = value;
}

/// @fn     CopyTo
/// @brief  Copy
/// @param  ValueArray<T> array
/// @param  DWORD dwIdBegin
/// @return None
template<typename T>
inline void
ValueArray<T>::CopyTo(
    ValueArray<T>& array,
    u32_t begin
) const {
    if (begin + m_lstBuff.size() > array.Size())
        THROW_MESSAGE("out of range");
    for (u32_t i = 0; i < m_lstBuff.size(); i++) {
        array[begin + i] = m_lstBuff.at(i);
    }
}

/// @fn     CopyTo
/// @brief  Copy
/// @param  T* pArray
/// @param  DWORD dwIdBegin
/// @return None
template<typename T>
inline void
ValueArray<T>::CopyTo(
    T* pArray,
    u32_t begin
) const {
    for (u32_t i = 0; i < m_lstBuff.size(); i++) {
        pArray[begin + i] = m_lstBuff.at(i);
    }
}

/// @fn     operator<<
/// @brief  operator<<
/// @param  T value
/// @return None
template<typename T>
inline ValueArray<T>&
ValueArray<T>::operator<< (
    T value
) {
    VALUE_ASSERT(m_dwCount < m_lstBuff.size() && "error << ");
    m_lstBuff[m_dwCount] = value;
    m_dwCount++;
    return *this;
}

/// @fn     operator,
/// @brief  operator,
/// @param  T value
/// @return None
template<typename T>
inline ValueArray<T>&
ValueArray<T>::operator, (
    T value
) {
    VALUE_ASSERT(m_dwCount < m_lstBuff.size() && "error << ");
    m_lstBuff[m_dwCount] = value;
    m_dwCount++;
    return *this;
}

/// @fn     operator==
/// @brief  None
/// @param  VALUEARRAY<T> rhs
/// @return TRUE if equal, FALSE if not
template<typename T>
inline bool_t
ValueArray<T>::operator== (
    const ValueArray<T>& rhs
) const {
    return m_lstBuff == rhs.m_lstBuff;
}

template<typename T>
inline bool_t
ValueArray<T>::Contain(
    const ValueArray<T>& value
) const {
    if (Size() < value.Size())
        return FALSE;
    for (u32_t i = 0; i < value.Size(); i++) {
        if (m_lstBuff.at(i) != value.GetValue(i)) {
            return FALSE;
        }
    }
    return TRUE;
}

/// @fn     Clear
/// @brief  None
/// @param  None
/// @return None
template<typename T>
inline void
ValueArray<T>::Clear(
) {
    m_lstBuff.clear();
}

/// @fn     AddRange
/// @brief  None
/// @param  data
/// @return None
template<typename T>
inline void
ValueArray<T>::AddRangeTo(
    Vector<T>& data
) const {
    AddRange(data, m_lstBuff);
}

/// @fn     Copy
/// @brief  None
/// @tparam T
/// @param  src
/// @param  srcIndex
/// @param  dst
/// @param  dstIndex
/// @param  length
/// @return None
template<typename T>
inline void
ValueArray<T>::Copy(
    ValueArray<T>& src,
    u32_t srcIndex,
    ValueArray<T>& dst,
    u32_t dstIndex,
    u32_t length
) {
    for (u32_t i = 0; i < length; i++) {
        dst[i + dstIndex] = src[srcIndex + i];
    }
}

/// @fn     Copy
/// @brief  None
/// @tparam T
/// @param  src
/// @param  srcIndex
/// @param  dst
/// @param  dstIndex
/// @param  length
/// @return None
template<typename T>
inline void
ValueArray<T>::Copy(
    Vector<T>& src,
    u32_t srcIndex,
    ValueArray<T>& dst,
    u32_t dstIndex,
    u32_t length
) {
    for (u32_t i = 0; i < length; i++) {
        dst[i + dstIndex] = src[srcIndex + i];
    }
}

/// @fn     Reverse
/// @brief  None
/// @tparam T
/// @return None
template<typename T>
inline void
ValueArray<T>::Reverse(
) {
    std::reverse(m_lstBuff.begin(), m_lstBuff.end());
}

}
}

#define MAKE_TYPE_ARRAY(Type, TypeSuffix)       \
typedef libX::val::ValueArray<Type>   lst##TypeSuffix##_t;

MAKE_TYPE_ARRAY(u8_t,   u8)
MAKE_TYPE_ARRAY(i8_t,   i8)
MAKE_TYPE_ARRAY(u16_t, u16)
MAKE_TYPE_ARRAY(i16_t, i16)
MAKE_TYPE_ARRAY(u32_t, u32)
MAKE_TYPE_ARRAY(i32_t, i32)

#undef MAKE_TYPE_MATRIX

#endif /* !LIBX_VAL_VALUEARRAY_HPP_ */
