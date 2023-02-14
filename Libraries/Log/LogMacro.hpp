/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: MacroLog.hpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 23 Feb 2017 23:06:30
 *
 ******************************************************************************/
#ifndef MACRO_LOG_HPP_
#define MACRO_LOG_HPP_

#include <sys/time.h>
#include <pthread.h>
#include <string.h>
#include <stdarg.h>
#include <cstdio>
#include <string>

#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Utils/String.hpp"
#include "Libraries/Log/Log.hpp"

/// @func   PrintArgs
/// @brief  None
/// @param  None
/// @retval None
inline String
PrintArgs(
    const_char_p format, ...
) {
    char buffer[4096] = { 0 };
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    String strRet(buffer);
    return strRet;
}

/// @func   MacroLog
/// @brief  None
/// @param  None
/// @retval None
inline void
MacroLog(
    Log::Level log_level,
    const_char_p filename,
    int_t line,
    const String& message
) {
    char buffer[4096] = { 0 };
    memset(buffer, 0, 4096);
    int_t len = snprintf(buffer, sizeof(buffer), "[%18s:%4d] %s", filename, line, message.c_str());
    UNUSED(len);
    Log::Write(log_level, buffer);
}

/// @func   MacroLog
/// @brief  None
/// @param  None
/// @retval None
inline void
StdoLog(
    const_char_p format, ...
) {
    try {
        static pthread_mutex_t lock;
        char buffer[4096] = { 0 };
        memset(buffer, 0, 4096);
        va_list args;
        va_start(args, format);
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);
        pthread_mutex_lock(&lock);
        fputs(buffer, stdout);
        pthread_mutex_unlock(&lock);
    } catch (...) {

    }
}

#endif /* MACRO_LOG_HPP_ */
