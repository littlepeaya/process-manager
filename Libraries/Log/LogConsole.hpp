/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: LogConsole.hpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 24 Feb 2017 06:51:27
 *
 ******************************************************************************/
#ifndef LOG_CONSOLE_HPP_
#define LOG_CONSOLE_HPP_

#include <sys/time.h>
#include <pthread.h>
#include <string.h>
#include <stdarg.h>
#include <cstdio>
#include <string>

#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Log/LogMacro.hpp"

#define CONSOLE(...) \
    ST (\
        struct timeval tv; \
        gettimeofday(&tv, NULL); \
        struct tm* now; \
        now = localtime(&tv.tv_sec); \
        String varlist = PrintArgs(__VA_ARGS__); \
        StdoLog("\x1b[36m[DBUG][%04d-%02d-%02d %02d:%02d:%02d.%03d][%010u][%18s:%4d] %s\n\x1b[39m", \
        now->tm_year + 1990, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec, \
        (int_t) tv.tv_usec / 1000, (u32_t) pthread_self(), __FILENAME__, __LINE__, varlist.c_str()); \
    )

#endif /* LOG_CONSOLE_HPP_ */
