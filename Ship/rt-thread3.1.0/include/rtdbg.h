/*
 * File      : rtdbg.h
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2016, RT-Thread Development Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2016-11-12     Bernard      The first version
 * 2018-05-25     armink       Add simple API, such as LOG_D, LOG_E
 */

/*
 * The macro definitions for debug
 *
 * These macros are defined in static. If you want to use debug macro, you can
 * use as following code:
 *
 * In your C/C++ file, enable/disable DEBUG_ENABLE macro, and then include this
 * header file.
 *
 * #define DBG_SECTION_NAME    "MOD"
 * #define DBG_ENABLE          // enable debug macro
 * #define DBG_LEVEL           DBG_INFO
 * #include <rtdbg.h>          // must after of DEBUG_ENABLE or some other options
 *
 * Then in your C/C++ file, you can use dbg_log macro to print out logs:
 * dbg_log(DBG_INFO, "this is a log!\n");
 *
 * Or if you want to using the simple API, you can
 * LOG_D("this is a debug log!");
 * LOG_E("this is a error log!");
 *
 * If you want to use different color for different kinds log, you can
 * #define DBG_COLOR
 */

#ifndef RT_DBG_H__
#define RT_DBG_H__

#include <rtconfig.h>

/* DEBUG level */
#define DBG_LOG             0
#define DBG_INFO            1
#define DBG_WARNING         2
#define DBG_ERROR           3

#ifndef DBG_SECTION_NAME
#define DBG_SECTION_NAME    "DBG"
#endif

#ifdef DBG_ENABLE

#ifndef DBG_LEVEL
#define DBG_LEVEL         DBG_WARNING
#endif

/*
 * The color for terminal (foreground)
 * BLACK    30
 * RED      31
 * GREEN    32
 * YELLOW   33
 * BLUE     34
 * PURPLE   35
 * CYAN     36
 * WHITE    37
 */
#ifdef DBG_COLOR
#define _DBG_COLOR(n)        rt_kprintf("\033["#n"m")
#define _DBG_LOG_HDR(lvl_name, color_n)                    \
    rt_kprintf("\033["#color_n"m["lvl_name"/"DBG_SECTION_NAME"] ")
#else
#define _DBG_COLOR(n)
#define _DBG_LOG_HDR(lvl_name, color_n)                    \
    rt_kprintf("["lvl_name"/"DBG_SECTION_NAME"] ")
#endif /* DBG_COLOR */

/*
 * static debug routine
 */
#define dbg_log(level, fmt, ...)                            \
    if ((level) >= DBG_LEVEL)                               \
    {                                                       \
        switch(level)                                       \
        {                                                   \
            case DBG_ERROR:   _DBG_LOG_HDR("E", 31); break; \
            case DBG_WARNING: _DBG_LOG_HDR("W", 33); break; \
            case DBG_INFO:    _DBG_LOG_HDR("I", 32); break; \
            case DBG_LOG:     _DBG_LOG_HDR("D", 0); break;  \
            default: break;                                 \
        }                                                   \
        rt_kprintf(fmt, ##__VA_ARGS__);                     \
        _DBG_COLOR(0);                                      \
    }

#define dbg_here                                            \
    if ((DBG_LEVEL) >= DBG_LOG){                            \
        rt_kprintf(DBG_SECTION_NAME " Here %s:%d\n",        \
            __FUNCTION__, __LINE__);                        \
    }

#define dbg_enter                                           \
    if ((DBG_LEVEL) >= DBG_LOG){                            \
        _DBG_COLOR(32);                                     \
        rt_kprintf(DBG_SECTION_NAME " Enter %s\n",          \
            __FUNCTION__);                                  \
        _DBG_COLOR(0);                                      \
    }

#define dbg_exit                                            \
    if ((DBG_LEVEL) >= DBG_LOG){                            \
        _DBG_COLOR(32);                                     \
        rt_kprintf(DBG_SECTION_NAME " Exit  %s:%d\n",       \
            __FUNCTION__);                                  \
        _DBG_COLOR(0);                                      \
    }

#define dbg_log_line(level, ...)                            \
    dbg_log(level, __VA_ARGS__);                            \
    if ((level) >= DBG_LEVEL) {                             \
        rt_kprintf("\n");                                   \
    }

#define dbg_raw(...)         rt_kprintf(__VA_ARGS__);

#else
#define dbg_log(level, fmt, ...)
#define dbg_here
#define dbg_enter
#define dbg_exit
#define dbg_log_line(level, ...)
#define dbg_raw(...)
#endif

#define LOG_D(...)           dbg_log_line(DBG_LOG    , __VA_ARGS__)
#define LOG_I(...)           dbg_log_line(DBG_INFO   , __VA_ARGS__)
#define LOG_W(...)           dbg_log_line(DBG_WARNING, __VA_ARGS__)
#define LOG_E(...)           dbg_log_line(DBG_ERROR  , __VA_ARGS__)
#define LOG_RAW(...)         dbg_raw(__VA_ARGS__)

#endif /* RT_DBG_H__ */
