/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: AtomicCounter.cpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 22 Mar 2017 10:52:35
 *
 ******************************************************************************/

#include "Libraries/Atomic/AtomicCounter.hpp"

/**
 * @func   AtomicCounter
 * @brief  None
 * @param  None
 * @retval None
 */
AtomicCounter::AtomicCounter(
) : m_counter (0) {
}

/**
 * @func   ~AtomicCounter
 * @brief  None
 * @param  None
 * @retval None
 */
AtomicCounter::~AtomicCounter(
) {
}

/**
 * @func   AtomicCounter
 * @brief  None
 * @param  None
 * @retval None
 */
AtomicCounter::AtomicCounter(
    const AtomicCounter& copied
) : m_counter (copied.value()) {
}

/**
 * @func   AtomicCounter
 * @brief  None
 * @param  None
 * @retval None
 */
AtomicCounter::AtomicCounter(
    valueType initvalue
) : m_counter (initvalue) {

}

/**
 * @func   operator=
 * @brief  None
 * @param  None
 * @retval None
 */
AtomicCounter&
AtomicCounter::operator= (
    const AtomicCounter& copied
) {
    m_counter.store(copied.m_counter);
    return *this;
}

/**
 * @func   operator=
 * @brief  None
 * @param  None
 * @retval None
 */
AtomicCounter&
AtomicCounter::operator= (
    AtomicCounter::valueType value
) {
    m_counter.store(value);
    return *this;
}

/**
 * @func   operator valueType
 * @brief  None
 * @param  None
 * @retval None
 */
AtomicCounter::operator valueType(
) const {
    return m_counter.load();
}

/**
 * @func   value
 * @brief  None
 * @param  None
 * @retval None
 */
AtomicCounter::valueType
AtomicCounter::value(
) const {
    return m_counter.load();
}

/**
 * @func   operator++
 * @brief  None
 * @param  None
 * @retval None
 */
AtomicCounter::valueType
AtomicCounter::operator++ (
) {
    return ++m_counter;
}

/**
 * @func   operator++
 * @brief  None
 * @param  None
 * @retval None
 */
AtomicCounter::valueType
AtomicCounter::operator++ (
    int_t
) {
    return m_counter++;
}

/**
 * @func   operator--
 * @brief  None
 * @param  None
 * @retval None
 */
AtomicCounter::valueType
AtomicCounter::operator-- (
) {
    return --m_counter;
}

/**
 * @func   operator--
 * @brief  None
 * @param  None
 * @retval None
 */
AtomicCounter::valueType
AtomicCounter::operator-- (
    int_t
) {
    return m_counter--;
}

/**
 * @func   operator!
 * @brief  None
 * @param  None
 * @retval None
 */
bool_t
AtomicCounter::operator! (
) const {
    return m_counter.load() == 0;
}
