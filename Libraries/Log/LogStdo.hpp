/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: LogStdout.hpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 24 Feb 2017 09:38:03
 *
 ******************************************************************************/
#ifndef LOG_STDOUT_HPP_
#define LOG_STDOUT_HPP_

#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Log/LogMacro.hpp"

#define STDO_CRIT(x, ...) \
    ST( \
        struct timeval tv; \
        gettimeofday(&tv, NULL); \
        struct tm* now; \
        now = localtime(&tv.tv_sec); \
        StdoLog("\x1b[95m[CRIT][%04d-%02d-%02d %02d:%02d:%02d.%03d][%010u][%18s:%4d] " x "\n\x1b[39m", \
        now->tm_year + 1990, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec, \
        (int_t) tv.tv_usec / 1000, (u32_t) pthread_self(), __FILENAME__, __LINE__, ##args); \
    )

#define STDO_ERRO(x, args...) \
    ST( \
        struct timeval tv; \
        gettimeofday(&tv, NULL); \
        struct tm* now; \
        now = localtime(&tv.tv_sec); \
        StdoLog("\x1b[31m[EROR][%04d-%02d-%02d %02d:%02d:%02d.%03d][%010u][%18s:%4d] " x "\n\x1b[39m", \
        now->tm_year + 1990, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec, \
        (int_t) tv.tv_usec / 1000, (u32_t) pthread_self(), __FILENAME__, __LINE__, ##args); \
    )

#define STDO_WARN(x, args...) \
    ST( \
        struct timeval tv; \
        gettimeofday(&tv, NULL); \
        struct tm* now; \
        now = localtime(&tv.tv_sec); \
        StdoLog("\x1b[33m[WARN][%04d-%02d-%02d %02d:%02d:%02d.%03d][%010u][%18s:%4d] " x "\n\x1b[39m", \
        now->tm_year + 1990, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec, \
        (int_t) tv.tv_usec / 1000, (u32_t) pthread_self(), __FILENAME__, __LINE__, ##args); \
    )

#define STDO_INFO(x, args...) \
    ST( \
        struct timeval tv; \
        gettimeofday(&tv, NULL); \
        struct tm* now; \
        now = localtime(&tv.tv_sec); \
        StdoLog("\x1b[39m[INFO][%04d-%02d-%02d %02d:%02d:%02d.%03d][%010u][%18s:%4d] " x "\n\x1b[39m", \
        now->tm_year + 1990, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec, \
        (int_t) tv.tv_usec / 1000, (u32_t) pthread_self(), __FILENAME__, __LINE__, ##args); \
    )

#define STDO_DBUG(x, args...) \
    ST ( \
        struct timeval tv; \
        gettimeofday(&tv, NULL); \
        struct tm* now; \
        now = localtime(&tv.tv_sec); \
        StdoLog("\x1b[36m[DBUG][%04d-%02d-%02d %02d:%02d:%02d.%03d][%010u][%18s:%4d] " x "\n\x1b[39m", \
        now->tm_year + 1990, now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min, now->tm_sec, \
        (int_t) tv.tv_usec / 1000, (u32_t) pthread_self(), __FILENAME__, __LINE__, ##args); \
    )

#endif /* !LOG_STDOUT_HPP_ */
