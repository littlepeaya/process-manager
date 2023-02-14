//
// Created by TrungTQ on 6 Jan 2017 20:48:25
//

#include <stdarg.h>
#include <stdio.h>
#include "Libraries/Utils/Exception.hpp"

/// @fn     SetParams
/// @brief  None
/// @param  None
/// @retval None
void
Exception::SetParams(
    const_char_p message,
    va_list args
) {
    char linebuffer [1024] = { 0 };
    vsnprintf(linebuffer, sizeof(linebuffer), message, args);
    m_strWhat.append(linebuffer);
}

/// @fn     Exception
/// @brief  None
/// @param  None
/// @retval None
Exception::Exception(
) noexcept {
}

/// @fn     ~Exception
/// @brief  None
/// @param  None
/// @retval None
Exception::~Exception(
) noexcept {
    m_strWhat.clear();
}

/// @fn     ExceptionBase
/// @brief  None
/// @param  None
/// @retval None
Exception::Exception (
    const_char_p message,
    ...
) noexcept {
    m_strWhat.assign("[exception] ");
    va_list args;
    va_start(args, message);
    SetParams(message, args);
    va_end(args);
}

/// @fn     Exception
/// @brief  None
/// @param  None
/// @retval None
Exception::Exception(
    const Exception& copied
) noexcept : m_strWhat (copied.m_strWhat) {

}

/// @fn     operator=
/// @brief  None
/// @param  None
/// @retval None
Exception&
Exception::operator= (
    const Exception& copied
) noexcept {
    m_strWhat = copied.m_strWhat;
    return *this;
}

/// @fn     what
/// @brief  None
/// @param  None
/// @retval None
const_char_p
Exception::what(
) const noexcept {
    return m_strWhat.c_str();
}

/// @fn     ExceptionInvalidCmd
/// @brief  None
/// @param  None
/// @retval None
ExceptionInvalidCmd::ExceptionInvalidCmd(
) noexcept {
    m_strWhat.assign("[invalid cmd]");
}

/// @fn     ~ExceptionInvalidCmd
/// @brief  None
/// @param  None
/// @retval None
ExceptionInvalidCmd::~ExceptionInvalidCmd(
) noexcept {

}

/// @fn     ExceptionInvalidCmd
/// @brief  None
/// @param  None
/// @retval None
ExceptionInvalidCmd::ExceptionInvalidCmd (
    const_char_p message,
    ...
) noexcept {
    m_strWhat.assign("[invalid cmd] ");
    va_list args;
    va_start(args, message);
    SetParams(message, args);
    va_end(args);
}

/// @fn     ExceptionInvalidCmd
/// @brief  None
/// @param  None
/// @retval None
ExceptionInvalidCmd::ExceptionInvalidCmd(
    const ExceptionInvalidCmd& copied
) noexcept : Exception (copied) {

}

/// @fn     operator=
/// @brief  None
/// @param  None
/// @retval None
ExceptionInvalidCmd&
ExceptionInvalidCmd::operator= (
    const ExceptionInvalidCmd& copied
) noexcept {
    Exception::operator =(copied);
    return *this;
}

/// @fn     what
/// @brief  None
/// @param  None
/// @retval None
const_char_p
ExceptionInvalidCmd::what(
) const noexcept {
    return Exception::what();
}

/// @fn     ExceptionNullPtr
/// @brief  None
/// @param  None
/// @retval None
ExceptionNullPtr::ExceptionNullPtr(
) noexcept {
    m_strWhat.assign("[nullptr] ");
}

/// @fn     ~ExceptionNullPtr
/// @brief  None
/// @param  None
/// @retval None
ExceptionNullPtr::~ExceptionNullPtr(
) noexcept {

}

/// @fn     ExceptionNullPtr
/// @brief  None
/// @param  None
/// @retval None
ExceptionNullPtr::ExceptionNullPtr (
    const_char_p message,
    ...
) noexcept {
    m_strWhat.assign("[nullptr] ");
    va_list args;
    va_start(args, message);
    SetParams(message, args);
    va_end(args);
}

/// @fn     ExceptionNullPtr
/// @brief  None
/// @param  None
/// @retval None
ExceptionNullPtr::ExceptionNullPtr(
    const ExceptionNullPtr& copied
) noexcept : Exception (copied) {
}

/// @fn     operator=
/// @brief  None
/// @param  None
/// @retval None
ExceptionNullPtr&
ExceptionNullPtr::operator= (
    const ExceptionNullPtr& copied
) noexcept {
    Exception::operator= (copied);
    return *this;
}

/// @fn     what
/// @brief  None
/// @param  None
/// @retval None
const_char_p
ExceptionNullPtr::what(
) const noexcept {
    return Exception::what();
}

/// @fn     ExceptionDatabase
/// @brief  None
/// @param  None
/// @retval None
ExceptionErrDatabase::ExceptionErrDatabase(
) noexcept {
    m_strWhat.assign("[error db] ");
}

/// @fn     ~ExceptionDatabase
/// @brief  None
/// @param  None
/// @retval None
ExceptionErrDatabase::~ExceptionErrDatabase(
) noexcept {

}

/// @fn     ExceptionDatabase
/// @brief  None
/// @param  None
/// @retval None
ExceptionErrDatabase::ExceptionErrDatabase (
    const_char_p message,
    ...
) noexcept {
    m_strWhat.assign("[error db] ");
    va_list args;
    va_start(args, message);
    SetParams(message, args);
    va_end(args);
}

/// @fn     ExceptionDatabase
/// @brief  None
/// @param  None
/// @retval None
ExceptionErrDatabase::ExceptionErrDatabase(
    const ExceptionErrDatabase& copied
) noexcept : Exception (copied) {

}

/// @fn     operator=
/// @brief  None
/// @param  None
/// @retval None
ExceptionErrDatabase&
ExceptionErrDatabase::operator= (
    const ExceptionErrDatabase& copied
) noexcept {
    Exception::operator =(copied);
    return *this;
}

/// @fn     what
/// @brief  None
/// @param  None
/// @retval None
const_char_p
ExceptionErrDatabase::what(
) const noexcept {
    return Exception::what();
}

/// @fn     ExceptionInvaLidArgs
/// @brief  None
/// @param  None
/// @retval None
ExceptionInvalidArg::ExceptionInvalidArg(
) noexcept {

}

/// @fn     ~ExceptionInvaLidArgs
/// @brief  None
/// @param  None
/// @retval None
ExceptionInvalidArg::~ExceptionInvalidArg(
) noexcept {
    m_strWhat.assign("[invalid arg] ");
}

/// @fn     ExceptionInvaLidArgs
/// @brief  None
/// @param  None
/// @retval None
ExceptionInvalidArg::ExceptionInvalidArg(
    const ExceptionInvalidArg& copied
) noexcept : Exception (copied) {

}

/// @fn     operator=
/// @brief  None
/// @param  None
/// @retval None
ExceptionInvalidArg&
ExceptionInvalidArg::operator= (
    const ExceptionInvalidArg& copied
) noexcept {
    Exception::operator =(copied);
    return *this;
}

/// @fn     ExceptionInvaLidArgs
/// @brief  None
/// @param  None
/// @retval None
ExceptionInvalidArg::ExceptionInvalidArg(
    const_char_p message,
    ...
) noexcept {
    m_strWhat.assign("[invalid arg] ");
    va_list args;
    va_start(args, message);
    SetParams(message, args);
    va_end(args);
}

/// @fn     what
/// @brief  None
/// @param  None
/// @retval None
const_char_p
ExceptionInvalidArg::what(
) const noexcept {
    return Exception::what();
}
