/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: LogPlus.hpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trunstkn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 5 Dec 2016 20:34:23
 *
 ******************************************************************************/
#ifndef LOGP_LUS_HPP_
#define LOGP_LUS_HPP_

#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Log/LogStdo.hpp"
#include "Libraries/Log/LogFile.hpp"

#if defined (USE_LOG_FILE)
#define LOG_CRIT(...) \
    ST(\
        String varlist = PrintArgs(__VA_ARGS__); \
        LLOG_CRIT(varlist); \
    )

#define LOG_ERRO(...) \
    ST(\
        String varlist = PrintArgs(__VA_ARGS__); \
        LLOG_ERRO(varlist); \
    )

#define LOG_WARN(...) \
    ST(\
        String varlist = PrintArgs(__VA_ARGS__); \
        LLOG_WARN(varlist); \
    )

#define LOG_INFO(...) \
    ST(\
        String varlist = PrintArgs(__VA_ARGS__); \
        LLOG_INFO(varlist); \
    )

#define LOG_DBUG(...) \
    ST(\
        String varlist = PrintArgs(__VA_ARGS__); \
        LLOG_DBUG(varlist); \
    )
#elif defined (USE_LOG_CONSOLE)

#define LOG_CRIT(...) \
    ST( \
        String varlist = PrintArgs(__VA_ARGS__); \
        STDO_CRIT("%s", varlist.c_str()); \
    )

#define LOG_ERRO(...) \
    ST( \
        String varlist = PrintArgs(__VA_ARGS__); \
        STDO_ERRO("%s", varlist.c_str()); \
    )

#define LOG_WARN(...) \
    ST( \
        String varlist = PrintArgs(__VA_ARGS__); \
        STDO_WARN("%s", varlist.c_str()); \
    )

#define LOG_INFO(...) \
    ST( \
        String varlist = PrintArgs(__VA_ARGS__); \
        STDO_INFO("%s", varlist.c_str()); \
    )

#define LOG_DBUG(...) \
    ST( \
        String varlist = PrintArgs(__VA_ARGS__); \
        STDO_DBUG("%s", varlist.c_str()); \
    )
#else
#define LOG_CRIT(...)
#define LOG_ERRO(...)
#define LOG_WARN(...)
#define LOG_INFO(...)
#define LOG_DBUG(...)
#endif /* USE_LOG_FILE */

#endif /* !LOGP_LUS_HPP_ */
