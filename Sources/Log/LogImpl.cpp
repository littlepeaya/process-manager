/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: LogImpl.cpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 15 Nov 2016 10:25:25
 *
 ******************************************************************************/
#include <stdarg.h>
#include <sys/time.h>
#include <errno.h>
#include <Libraries/Utils/AutoLocker.hpp>
#include <json/json.h>

#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Utils/String.hpp"
#include "Libraries/Log/LogPlus.hpp"
#include "Libraries/Log/LogConsole.hpp"
#include "Libraries/Utils/Exception.hpp"
#include "Libraries/Log/LogImpl.hpp"

LogImpl::LogImpl(
    const String& fileName,
    const bool_t& append,
    const bool_t& console,
    const Log::Level& logFile,
    const Log::Level& logQueue
) : m_fileName (fileName)
  , m_append (append)
  , m_console (console)
  , m_logFile (logFile)
  , m_logQueue (logQueue)
  , m_pFile (nullptr) {
    if (!m_fileName.empty()) {
        if (m_append) {
            m_pFile = fopen(m_fileName.c_str(), "a");
        } else {
            m_pFile = fopen(m_fileName.c_str(), "w");
        }
        if (m_pFile != nullptr) {
            setlinebuf(m_pFile);
        } else {
            THROW_MESSAGE("create log file error [%03d]:<<%s>>", errno, strerror(errno));
        }
    }
    setlinebuf(stdout);
}

/// @func   ~LogImpl
/// @brief  None
/// @param  None
/// @return None
LogImpl::~LogImpl(
) {
    if (m_pFile != nullptr) {
        fclose(m_pFile);
        m_pFile = nullptr;
    }
}

/// @func   ToEscapeCode
/// @brief  None
/// @param  LEVEL
/// @return None
u32_t
LogImpl::ToEscapeCode(
    Log::Level level
) {
    u32_t dwCode;
    switch (level) {
    case Log::Level::CRITICAL:
        dwCode = 95;
        break;

    case Log::Level::DEBUG:
        dwCode = 36;
        break;

    case Log::Level::ERROR:
        dwCode = 31;
        break;

    case Log::Level::WARN:
        dwCode = 33;
        break;

    case Log::Level::INFO:
        dwCode = 39;
        break;

    case Log::Level::ALL:
        dwCode = 32;
        break;

    default:
        dwCode = 39;
    }
    return dwCode;
}

/// @func   Write
/// @brief  None
/// @param  LEVEL loglevel
/// @param  CONSTCHAR format
/// @param  LIST args
/// @return None
void
LogImpl::Write(
    Log::Level loglevel,
    const_char_p format,
    va_list args
) {
    String strTime(GetTime());
    String strThreadId(GetTId());
    String strLogLevel(GetLogLevel(loglevel));
    String info;

    if ((format != nullptr) && (format[0] != '\0')) {
        char lineBuffer[4096] = { 0 };
        memset(lineBuffer, 0, 4096);
        va_list saveargs;
        va_copy(saveargs, args);
        int_t len = vsnprintf(lineBuffer, sizeof(lineBuffer), format, args);
        va_end(saveargs);
        info.append(String(lineBuffer, len));
    }
    Json::Value outbuffjson;
    if ((m_pFile != nullptr) || m_console) {
        String outBuff;
        outbuffjson["id_process"] = "[IO]";
        outbuffjson["log_level"] = strLogLevel;
        outbuffjson["local_time"] = strTime;
        outbuffjson["message"] = info;
        outBuff.append("[IO]");
        outBuff.append(strThreadId);
        outBuff.append(strLogLevel);
        outBuff.append(strTime);
        outBuff.append(info);
        outBuff.append("\n");
        Json::FastWriter writer;
        AutoLocker<Locker> locker(m_locker);
        if ((m_pFile != nullptr) && (m_logFile >= loglevel)) {
            fputs(writer.write(outbuffjson).c_str(), m_pFile);
        }
        if (m_console) {
            fprintf(stdout, "\x1B[%02um", ToEscapeCode(loglevel));
            fputs(outBuff.c_str(), stdout);
            fprintf(stdout, "\x1b[39m");
        }
        // Queue(outBuff);
    }
}

/// @func   GetTime
/// @brief  None
/// @param  None
/// @return None
String
LogImpl::GetTime(
) {
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    struct tm* now;
    now = localtime(&tv.tv_sec);
    char buffer[100] = { 0 };
    memset(buffer, 0, 100);
    int_t len = snprintf(buffer, sizeof(buffer), "[%04d-%02d-%02d %02d:%02d:%02d.%03d]",
                         now->tm_year + 1900, now->tm_mon + 1, now->tm_mday,
                         now->tm_hour, now->tm_min, now->tm_sec, (int_t)tv.tv_usec / 1000); //Firm 7.2.0.48: Fix exact time on Debug
    String strTime(buffer, (unsigned) len);
    return strTime;
}

/// @func   GetTId
/// @brief  None
/// @param  None
/// @retval None
String
LogImpl::GetTId(
) {
    char buffer[20] = { 0 };
    memset(buffer, 0, 20);
    int_t len = snprintf(buffer, sizeof(buffer), "[%010u]", (u32_t) pthread_self());
    String strTId(buffer, len);
    return strTId;
}

/// @func   GetLogLevel
/// @brief  None
/// @param  LEVEL loglevel
/// @retval None
String
LogImpl::GetLogLevel(
    Log::Level loglevel
) {
    if (loglevel <= Log::Level::ALL) {
        String strLogLevel(LogLevelString[loglevel]);
        return strLogLevel;
    }
    return String("[Unknown]");
}

/// @func   Queue
/// @brief  None
/// @param  STRING out
/// @retval None
void
LogImpl::Queue(
    const String& out
) {
    m_lstLog.push_back(out);
    if (m_lstLog.size() > queueMAX) {
        m_lstLog.pop_back();
    }
}

/// @func   PushQueue
/// @brief  None
/// @param  None
/// @retval None
void
LogImpl::PushQueue(
) {
    auto it = m_lstLog.begin();
    while (it != m_lstLog.end()) {
        Log::Write(Log::Level::ALL, "%s", (*it).c_str());
        it++;
    }
    m_lstLog.clear();
}
