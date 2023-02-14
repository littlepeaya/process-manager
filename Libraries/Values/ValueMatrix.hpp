/// Created by TrungTQ on 8 Jul 2017 09:23:47

#ifndef LIBX_VAL_VALUEMATRIX_HPP_
#define LIBX_VAL_VALUEMATRIX_HPP_

#include <cstring>
#include <cassert>
#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Utils/Exception.hpp"
#include "Libraries/Values/Value.hpp"
#include "Libraries/Values/ValueArray.hpp"
#include "Libraries/Values/Enums/TypeValues.hpp"

namespace libX {
namespace val {

template<typename T>
class ValueMatrix : public Value {
private:
    enum { Id = (u8_t) TypeValues::type_matrix };
    typedef ValueArray<T> Row;
    Vector<Row> m_Rows;
    u32_t       m_dwRowCount;
    u32_t       m_dwColCount;
protected:
    /// @fn     Swap
    /// @brief  Swap data
    /// @param  other
    /// @return None
    void Swap(ValueMatrix& other);
public:
    /// @fn     ValueMatrix
    /// @brief  Constructor
    /// @param  None
    /// @return Object
    ValueMatrix();

    /// @fn     ValueMatrix
    /// @brief  Constructor
    /// @param  DOWRD dwRow
    /// @param  DOWRD dwCol
    /// @return Object
    ValueMatrix(u32_t dwRow, u32_t dwCol);

    /// @fn     ValueMatrix
    /// @brief  Constructor
    /// @param  DOWRD   dwRow
    /// @param  DOWRD   dwCol
    /// @param  T       value
    /// @return Object
    ValueMatrix(u32_t dwRow, u32_t dwCol, T value);

    /// @fn     ValueMatrix
    /// @brief  Constructor
    /// @param  T* pBuffer
    /// @param  DOWRD dwRow
    /// @param  DOWRD dwCol
    /// @return Object
    ValueMatrix(T* pBuffer, u32_t dwRow, u32_t dwCol);

    /// @fn     ValueMatrix
    /// @brief  Constructor
    /// @param  CONST T& rBuffer
    /// @return Object
    template<u32_t R, u32_t C>
    explicit ValueMatrix(const T (&rBuffer)[R][C]);

    /// @fn     ValueMatrix
    /// @brief  Copy Constructor
    /// @param  ARRAY copied
    /// @return Object
    ValueMatrix(const ValueMatrix& copied);

    /// @fn     ValueMatrix
    /// @brief  Assignment
    /// @param  ARRAY copied
    /// @return Object
    ValueMatrix& operator= (const ValueMatrix& copied);

    /// @fn     ValueMatrix
    /// @brief  Assignment
    /// @param  T value
    /// @return Object
    ValueMatrix& operator= (const T value);

    /// @fn     ~ValueMatrix
    /// @brief  Destructor
    /// @param  None
    /// @return None
    virtual ~ValueMatrix();

    /// @fn     RowSize
    /// @brief  Download row size of matrix
    /// @param  None
    /// @return DWORD row size of matrix
    u32_t RowSize() const;

    /// @fn     ColSize
    /// @brief  Download column size of matrix
    /// @param  None
    /// @return DWORD column size of matrix
    u32_t ColSize() const;

    /// @fn     Size
    /// @brief  None
    /// @pram   None
    /// @return case overflow
    u32_t Size() const;

    /// @fn     GetValue
    /// @brief  Download value in specific row, column
    /// @param  IDWORD idwRow
    /// @param  IDWORD idwCol
    /// @return T value
    T GetValue(u32_t dwRow, u32_t dwCol) const;

    /// @fn     Operator []
    /// @brief  None
    /// @param  DWORD dwRow
    /// @return Row&
    Row& operator[] (u32_t dwRow);

    /// @fn     Operator ()
    /// @brief  Download value in specific row, column
    /// @param  DWORD dwRow
    /// @param  DWORD dwCol
    /// @return T& value
    T& operator() (u32_t dwRow, u32_t dwCol);

    /// @fn     SetValue
    /// @brief  Set value in specific index
    /// @param  T value
    /// @param  DWORD dwRow
    /// @param  DWORD dwCol
    /// @return None
    void SetValue(u32_t dwRow, u32_t dwCol, T value);

    /// @fn     CopyTo
    /// @brief  Copy
    /// @param  T* pMatrix
    /// @param  DWORD dwIndex
    /// @return None
    void CopyTo(T* pMatrix, u32_t dwIndex);

    /// @fn     operator<<
    /// @brief  operator<<
    /// @param  T value
    /// @return None
    ValueMatrix& operator<< (T value);

    /// @fn     operator,
    /// @brief  operator,
    /// @param  T value
    /// @return None
    ValueMatrix& operator, (T value);
};

/// @fn     ValueMatrix
/// @brief  Constructor
/// @param  None
/// @return Object
template<typename T>
inline
ValueMatrix<T>::ValueMatrix(
) : Value (Id)
  , m_dwRowCount (0)
  , m_dwColCount (0) {

}

/// @fn     ValueMatrix
/// @brief  Constructor
/// @param  DOWRD dwLength
/// @return Object
template<typename T>
inline
ValueMatrix<T>::ValueMatrix(
    u32_t dwRow,
    u32_t dwCol
) : Value (Id)
  , m_Rows (dwRow, Row(dwCol))
  , m_dwRowCount (0)
  , m_dwColCount (0) {
}

/// @fn     ValueMatrix
/// @brief  Constructor
/// @param  DOWRD dwRow
/// @param  DOWRD dwCol
/// @param  T value
/// @return Object
template<typename T>
inline
ValueMatrix<T>::ValueMatrix(
    u32_t dwRow,
    u32_t dwCol,
    T value
) : Value (Id)
  , m_Rows (dwRow, Row(dwCol, value))
  , m_dwRowCount (dwRow)
  , m_dwColCount (dwCol) {

}

/// @fn     ValueMatrix
/// @brief  Constructor
/// @param  T* pBuffer
/// @param  DOWRD dwRow
/// @param  DOWRD dwCol
/// @return Object
template<typename T>
inline
ValueMatrix<T>::ValueMatrix(
    T* pBuffer,
    u32_t dwRow,
    u32_t dwCol
) : Value (Id)
  , m_Rows (dwRow, Row(dwCol))
  , m_dwRowCount (dwRow)
  , m_dwColCount (dwCol) {
    for (i32_t iRow = 0; iRow < dwRow; iRow++) {
        for (i32_t iCol = 0; iCol < dwCol; iCol++) {
            m_Rows[iRow][iCol] = pBuffer[dwCol * iRow + iCol];
        }
    }
}

/// @fn     ValueMatrix
/// @brief  Constructor
/// @brief  R: Row, C: Column
/// @param  CONST T& rBuffer[R][C]
/// @return Object
template<typename T>
template<u32_t R, u32_t C>
inline
ValueMatrix<T>::ValueMatrix(
    const T (&rBuffer)[R][C]
) : Value (Id)
  , m_Rows (R, Row(C))
  , m_dwRowCount (R)
  , m_dwColCount (C) {
    for (u32_t dwRow = 0; dwRow < R; dwRow++) {
        for (u32_t dwCol = 0; dwCol < C; dwCol++) {
            m_Rows[dwRow][dwCol] = rBuffer[dwRow][dwCol];
        }
    }
}

/// @fn     ValueMatrix
/// @brief  Copy Constructor
/// @param  MATRIX copied
/// @return Object
template<typename T>
inline
ValueMatrix<T>::ValueMatrix(
    const ValueMatrix& copied
) : Value (copied)
  , m_Rows (copied.m_Rows)
  , m_dwRowCount (copied.m_dwRowCount)
  , m_dwColCount (copied.m_dwColCount) {
}

/// @fn     ValueMatrix
/// @brief  Assignment
/// @param  MATRIX copied
/// @return Object
template<typename T>
inline
ValueMatrix<T>&
ValueMatrix<T>::operator= (
    const ValueMatrix& copied
) {
    ValueMatrix<T> temp(copied);
    Swap(temp);
    return *this;
}

/// @fn     ValueMatrix
/// @brief  Assignment
/// @param  T value
/// @return Object
template<typename T>
inline
ValueMatrix<T>&
ValueMatrix<T>::operator= (
    const T value
) {
    for (u32_t i = 0; i < m_Rows.size(); i++) {
        m_Rows[i] = value;
    }
    return *this;
}

/// @fn     ~ValueMatrix
/// @brief  Destructor
/// @param  None
/// @return None
template<typename T>
inline
ValueMatrix<T>::~ValueMatrix(
) {
}

/// @fn     Swap
/// @brief  Swap data
/// @param  other
/// @return None
template<typename T>
inline void
ValueMatrix<T>::Swap(
    ValueMatrix& other
) {
    Value::Swap(other);
    std::swap(m_Rows, other.m_Rows);
}

/// @fn     RowSize
/// @brief  Download row size of matrix
/// @param  None
/// @return DWORD row size of matrix
template<typename T>
inline u32_t
ValueMatrix<T>::RowSize(
) const {
    return m_Rows.size();
}

/// @fn     ColSize
/// @brief  Download column size of matrix
/// @param  None
/// @return DWORD column size of matrix
template<typename T>
inline u32_t
ValueMatrix<T>::ColSize(
) const {
    return (m_Rows.size() != 0) ? m_Rows[0].Size() : 0;
}

/// @fn     Size
/// @brief  None
/// @pram   None
/// @return None
template<typename T>
inline u32_t
ValueMatrix<T>::Size(
) const {
    /// Note: may be overflow
    return RowSize() * ColSize();
}

/// @fn     GetValue
/// @brief  Download value in specific row, column
/// @param  DWORD dwRow
/// @param  DWORD dwCol
/// @return T value
template<typename T>
inline T
ValueMatrix<T>::GetValue(
    u32_t dwRow,
    u32_t dwCol
) const {
    if (dwRow < RowSize()) {
        if (dwCol < ColSize()) {
            return m_Rows[dwRow][dwCol];
        }
        THROW_MESSAGE("column index out of bound");
    }
    THROW_MESSAGE("row index out of bound");
}

/// @fn     Operator []
/// @brief  None
/// @param  IDWORD idwRow
/// @return Row&
template<typename T>
inline typename ValueMatrix<T>::Row&
ValueMatrix<T>::operator[] (
    u32_t dwRow
) {
    if (dwRow < m_Rows.size()) {
        return m_Rows[dwRow];
    }
    THROW_MESSAGE("index out of bound");
}

/// @fn     operator ()
/// @brief  Download value in specific row, column
/// @param  DWORD dwRow
/// @param  DWORD dwCol
/// @return T value
template<typename T>
inline T&
ValueMatrix<T>::operator(
) (
    u32_t dwRow,
    u32_t dwCol
) {
    if (dwRow < RowSize()) {
        if (dwCol < ColSize()) {
            return m_Rows[dwRow][dwCol];
        }
        THROW_MESSAGE("column index out of bound");
    }
    THROW_MESSAGE("row index out of bound");
}

/// @fn     SetValue
/// @brief  Set value in specific index
/// @param  T value
/// @param  DWORD dwRow
/// @param  DWORD dwCol
/// @return None
template<typename T>
inline void
ValueMatrix<T>::SetValue(
    u32_t   dwRow,
    u32_t   dwCol,
    T       value
) {
    if (dwRow < RowSize()) {
        if (dwCol < ColSize()) {
            m_Rows[dwRow][dwCol] = value;
        }
        THROW_MESSAGE("column index out of bound");
    }
    THROW_MESSAGE("row index out of bound");
}

/// @fn     CopyTo
/// @brief  Copy
/// @param  T* pMatrix
/// @param  DWORD dwIndex
/// @return None
template<typename T>
inline void
ValueMatrix<T>::CopyTo(
    T* pMatrix,
    u32_t dwIndex
) {
    for (u32_t dwRow = 0; dwRow < RowSize(); dwRow++) {
        for (u32_t dwCol = 0; dwCol < ColSize(); dwCol++) {
            pMatrix[dwIndex + dwRow * ColSize() + dwCol] = m_Rows[dwRow][dwCol];
        }
    }
}

/// @fn     operator<<
/// @brief  operator<<
/// @param  T value
/// @return None
template<typename T>
inline ValueMatrix<T>&
ValueMatrix<T>::operator<< (
    T value
) {
    VALUE_ASSERT(m_dwRowCount < RowSize() && m_dwColCount < ColSize() && "error <<");
    m_Rows[m_dwRowCount][m_dwColCount] = value;
    m_dwColCount++;
    if (m_dwColCount >= ColSize()) {
        m_dwRowCount++;
        m_dwColCount = 0;
    }
    return *this;
}

/// @fn     operator,
/// @brief  operator,
/// @param  T value
/// @return None
template<typename T>
inline ValueMatrix<T>&
ValueMatrix<T>::operator, (
    T value
) {
    VALUE_ASSERT(m_dwRowCount < RowSize() && m_dwColCount < ColSize() && "error <<");
    m_Rows[m_dwRowCount][m_dwColCount] = value;
    m_dwColCount++;
    if (m_dwColCount >= ColSize()) {
        m_dwRowCount++;
        m_dwColCount = 0;
    }
    return *this;
}

#define MAKE_TYPE_MATRIX(Type, TypeSuffix)       \
typedef ValueMatrix<Type>   Mat##TypeSuffix##_t;

MAKE_TYPE_MATRIX(u8_t,   u8)
MAKE_TYPE_MATRIX(i8_t,   i8)
MAKE_TYPE_MATRIX(u16_t, u16)
MAKE_TYPE_MATRIX(i16_t, i16)
MAKE_TYPE_MATRIX(u32_t, u32)
MAKE_TYPE_MATRIX(i32_t, i32)

#undef MAKE_TYPE_MATRIX

}
}

#endif /* !LIBX_VAL_VALUEMATRIX_HPP_ */
