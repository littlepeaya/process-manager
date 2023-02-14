//
// Created by TrungTQ on 23 Feb 2017 16:30:15
// Base on Emad Barsoum
//

#ifndef LIBX_UTILS_PROPERTY_HPP_
#define LIBX_UTILS_PROPERTY_HPP_

#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Utils/Exception.hpp"

typedef enum {
    READ_ONLY   = 0,
    WRITE_ONLY,
    READ_WRITE
} PropType;

template<typename Container, typename ValueType, PropType Type = PropType::READ_WRITE>
class Property {
private:
    Container* m_pObj;

public:
    Property(Container* pObj = NULL);
    ~Property();

    void SetContainer(Container* pContainer);

    ValueType operator= (const ValueType& value);
    operator ValueType();
};

/**
 * @func   Property
 * @brief  None
 * @param  None
 * @retval None
 */
template<typename Container, typename ValueType, PropType Type>
inline
Property<Container, ValueType, Type>::Property(
    Container* pObj
) : m_pObj (pObj) {
}

/**
 * @func   ~Property
 * @brief  None
 * @param  None
 * @retval None
 */
template<typename Container, typename ValueType, PropType Type>
inline
Property<Container, ValueType, Type>::~Property(
) {

}

/**
 * @func   ~Property
 * @brief  None
 * @param  None
 * @retval None
 */
template<typename Container, typename ValueType, PropType Type>
inline void
Property<Container, ValueType, Type>::SetContainer(
    Container* pContainer
) {
    m_pObj = pContainer;
}

/**
 * @func   operator =
 * @brief  None
 * @param  None
 * @retval None
 */
template<typename Container, typename ValueType, PropType Type>
inline ValueType
Property<Container, ValueType, Type>::operator= (
    const ValueType& value
) {
    if ((m_pObj != NULL) && ((Type == WRITE_ONLY) || (Type == READ_WRITE))) {
        *m_pObj = value;
        return value;
    }
    THROW_MESSAGE("not allow set");
}

/**
 * @func   operator V
 * @brief  None
 * @param  None
 * @retval None
 */
template<typename Container, typename ValueType, PropType Type>
inline
Property<Container, ValueType, Type>::operator ValueType(
) {
    if ((m_pObj != NULL) && ((Type == READ_ONLY) || (Type == READ_WRITE))) {
        return (ValueType)(*m_pObj);
    }
    THROW_MESSAGE("not allow get");
}

#endif /* !PROPERTY_HPP_ */
