#pragma once
#include "logc/log.h"

namespace Logger {

enum { LOG_TRACE,
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL };

template <class... Args>
void debug(Args... args)
{
    log_debug(args...);
};

template <class... Args>
void trace(Args... args)
{
    log_trace(args...);
};
template <class... Args>
void info(Args... args)
{
    log_info(args...);
};
template <class... Args>
void warn(Args... args)
{
    log_warn(args...);
};
template <class... Args>
void error(Args... args)
{
    log_error(args...);
};
template <class... Args>
void fatal(Args... args)
{
    log_log(LOG_FATAL, __FILE__, __LINE__, args...);
};

void setLevel(int level);

}
