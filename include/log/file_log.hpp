#pragma once

#include <string>

#define ENABLE_LOG 
#define LOG_FILE_NAME "/var/log/kwl_watchdog.log"

//日志级别
#define DEBUG 0
#define NORMAL 1
#define WARNING 2
#define ERROR 3
#define FATAL 4

void log_msg(int level, const char *format, ...);
