#include "file_log.hpp"
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iostream>
#include <stdarg.h>
#include <string>

const char *gLevelMap[] = {
    "DEBUG",
    "NORMAL",
    "WARNING",
    "ERROR",
    "FATAL"};

void log_msg(int level, const char *format, ...)
{
#ifdef ENABLE_LOG
    char timebuffer[1024];
    // 日志时间
    time_t timestamp = time(nullptr);
    tm *ltm = localtime(&timestamp);
    int year = ltm->tm_year + 1900;
    int month = ltm->tm_mon + 1;
    int day = ltm->tm_mday;
    int hour = ltm->tm_hour;
    int min = ltm->tm_min;
    int second = ltm->tm_sec;

    snprintf(timebuffer, sizeof timebuffer, "[%s][%04d-%02d-%02d %02d:%02d:%02d]", gLevelMap[level], year, month, day, hour, min, second);

    // 日志内容
    char logBuffer[1024];
    va_list args;                                         // 初始化参数信息
    va_start(args, format);                               // 初始化参数列表
    vsnprintf(logBuffer, sizeof logBuffer, format, args); //日志内容存入logBuffe中
    va_end(args);

    // // 打印日志信息
    //printf("%s %s\n", timebuffer, logBuffer);

    // // 打开日志文件，以追加模式写入
    // std::ofstream logFile;
    // logFile.open(LOG_FILE_NAME, std::ios::app);

    // if (logFile.is_open()) {
    //     // 写入时间戳和日志信息
    //     logFile << timebuffer << logBuffer << std::endl;
    //     // 关闭文件
    //     logFile.close();
    // } else {
    //     // 如果无法打开日志文件，输出错误消息到标准错误流
    //     std::cerr << "Error opening log file: " << LOG_FILE_NAME << std::endl;
    // }

    //往文件打印
    FILE *fp = fopen(LOG_FILE_NAME, "a");
    fprintf(fp, "%s %s\n", timebuffer, logBuffer);
    fclose(fp);
#endif
}
