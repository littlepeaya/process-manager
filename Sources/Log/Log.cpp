/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: Log.cpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 15 Nov 2016 10:17:07
 *
 ******************************************************************************/

#include <stdarg.h>
#include <stddef.h>

#include "Libraries/Log/LogImpl.hpp"

Log_p Log::m_pInstance = nullptr;
LogImpl_p  Log::m_pLogImpl = nullptr;

const_char_p LogLevelString[] = {
    "[CRIT]",
    "[EROR]",
    "[WARN]",
    "[INFO]",
    "[DBUG]",
    "[NOTE]"
};

/// @fn     Log
/// @brief  Constructor
/// @param  STRING fileName
/// @param  BOOL append
/// @param  BOOL console
/// @param  LEVEL logFile
/// @param  LEVEL logQueue
/// @return None
Log::Log(
    const String& fileName,
    const bool_t& append,
    const bool_t& console,
    const Level& logFile,
    const Level& logQueue
) {
    if (m_pLogImpl == nullptr) {
        m_pLogImpl = new LogImpl(fileName, append, console, logFile, logQueue);
    }
}

/// @func   Release
/// @brief  None
/// @param  None
/// @retval None
void
Log::Release(
) {
    if (m_pInstance != nullptr) {
        delete m_pInstance;
        m_pInstance = nullptr;
    }
}

/// @fn     Create
/// @brief  None
/// @param  STRING fileName
/// @param  BOOL append
/// @param  BOOL console
/// @param  LEVEL logFile
/// @param  LEVEL logQueue
/// @return LOG*
Log_p
Log::Create(
    const String& fileName,
    const bool_t& append,
    const bool_t& console,
    const Level& logFile,
    const Level& logQueue
) {
    if (m_pInstance == nullptr) {
        m_pInstance = new Log(fileName, append, console, logFile, logQueue);
    } else {
        Log::Release();
        m_pInstance = new Log(fileName, append, console, logFile, logQueue);
    }
    return m_pInstance;
}

/// @func   ~Log
/// @brief  None
/// @param  None
/// @retval None
Log::~Log(
) {
    if (m_pLogImpl != nullptr) {
        delete m_pLogImpl;
        m_pLogImpl = nullptr;
    }
}

/// @func   Write
/// @brief  None
/// @param  None
/// @retval None
void
Log::Write(
    Level level,
    const_char_p format, ...
) {
    try {
        va_list args;
        va_start(args, format);
        if (m_pInstance != nullptr)
            m_pInstance->m_pLogImpl->Write(level, format, args);
        va_end(args);
    } catch (std::exception &ex) {
    }
}

/// @func   Queue
/// @brief  None
/// @param  STRING out
/// @retval None
void
Log::Queue(
    const String& out
) {
    try {
        m_pLogImpl->Queue(out);
    } catch (std::exception &ex) {
    }
}

/// @func   PushQueue
/// @brief  None
/// @param  None
/// @retval None
void
Log::PushQueue(
) {
    try {
        m_pLogImpl->PushQueue();
    } catch (std::exception &ex) {
    }
}
