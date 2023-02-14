/*******************************************************************************
 * Copyright (c) 2016
 * Lumi, JSC.
 * All Rights Reserved
 *
 * File Name: LogFile.hpp
 *
 * Author: TrungTQ
 *
 * Last Changed By:  TrungTQ (trungkstn@gmail.com)
 * Revision:         1.0
 * Last Changed:     Date: 24 Feb 2017 06:47:38
 *
 ******************************************************************************/
#ifndef LIBX_LOG_FILE_HPP_
#define LIBX_LOG_FILE_HPP_

#include "Libraries/Log/Log.hpp"
#include "Libraries/Utils/Typedefs.h"
#include "Libraries/Log/LogMacro.hpp"

#define LLOG_CRIT(varlist)  MacroLog(Log::Level::CRITICAL, __FILENAME__, __LINE__, varlist)
#define LLOG_ERRO(varlist)  MacroLog(Log::Level::ERROR   , __FILENAME__, __LINE__, varlist)
#define LLOG_WARN(varlist)  MacroLog(Log::Level::WARN    , __FILENAME__, __LINE__, varlist)
#define LLOG_INFO(varlist)  MacroLog(Log::Level::INFO    , __FILENAME__, __LINE__, varlist)
#define LLOG_DBUG(varlist)  MacroLog(Log::Level::DEBUG   , __FILENAME__, __LINE__, varlist)

#endif /* LIBX_LOG_FILE_HPP_ */
