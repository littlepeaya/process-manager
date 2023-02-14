/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: LogImpl.hpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 15 Nov 2016 10:24:52
 *
 ******************************************************************************/
#ifndef LIBX_LOGIMPL_HPP_
#define LIBX_LOGIMPL_HPP_

#include "Libraries/Utils/List.hpp"
#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Utils/Locker.hpp"
#include "Libraries/Log/Log.hpp"

class LogImpl {
private:
    List<String> m_lstLog;
    String m_fileName;
    bool_t m_append;
    bool_t m_console;
    Log::Level m_logFile;
    Log::Level m_logQueue;
    FILE* m_pFile;
    Locker_t m_locker;

    String GetTime();
    String GetTId();
    String GetLogLevel(Log::Level level);
    u32_t ToEscapeCode(Log::Level level);
public:
    static const u32_t queueMAX = 500;

    LogImpl(const String& fileName,
            const bool_t& append,
            const bool_t& console,
            const Log::Level& logFile,
            const Log::Level& logQueue);

    ~LogImpl();

    void Write(Log::Level level, const_char_p format, va_list args);
    void Queue(const String& out);
    void PushQueue();
};

typedef LogImpl  LogImpl_t;
typedef LogImpl* LogImpl_p;

#endif /* LOGIMPL_HPP_ */
