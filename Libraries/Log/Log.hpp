/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: Log.hpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 15 Nov 2016 10:01:37
 *
 ******************************************************************************/
#ifndef LIBX_LOG_HPP_
#define LIBX_LOG_HPP_

#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Utils/Locker.hpp"
#include "Libraries/Utils/SharedPtr.hpp"

class LogImpl;

extern const_char_p LogLevelString[];

class Log {
public:
    enum Level {
        CRITICAL = 0,
        ERROR,
        WARN,
        INFO,
        DEBUG,
        ALL
    };

    /// @fn     Create
    /// @brief  None
    /// @param  STRING fileName
    /// @param  BOOL append
    /// @param  BOOL console
    /// @param  LEVEL logFile
    /// @param  LEVEL logQueue
    /// @return LOG*
    static Log* Create(
            const String& fileName,
            const bool_t& append,
            const bool_t& console,
            const Level& logFile,
            const Level& logQueue);

    static void Release();
    static void Write(Level level, const_char_p format, ...);
    static void Queue(const String& out);
    static void PushQueue();
private:
    static Log* m_pInstance;
    static LogImpl* m_pLogImpl;

    /// @fn     Log
    /// @brief  Constructor
    /// @param  STRING fileName
    /// @param  BOOL append
    /// @param  BOOL console
    /// @param  LEVEL logFile
    /// @param  LEVEL logQueue
    /// @return None
    Log(const String& fileName,
        const bool_t& append,
        const bool_t& console,
        const Level& logFile,
        const Level& logQueue);

    /// @func   ~Log
    /// @brief  None
    /// @param  None
    /// @retval None
    virtual ~Log();
};

typedef Log  Log_t;
typedef Log* Log_p;

#endif /* LIBX_LOG_HPP_ */
