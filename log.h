//
// Created by 0210 on 2023/6/3.
//

#ifndef APP_LOG_H
#define APP_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef struct {
    const char* fmt;
    const char* file;
    struct tm* time;
    void* udata;
    uint32_t line;
    uint32_t level;
    va_list ap;
} Log_Event;
typedef void(*log_LogFn)(Log_Event* ev, bool is_file);
typedef void(*log_LockFn)(bool lock, void* lock_data);
enum { LOG_TRACE, LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_FATAL };

#define log_trace(...) log_log(LOGTRACE, __FILE__, __LINE__, __VA_ARGS__)
#define log_debug(...) log_log(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define log_info(...)  log_log(LOG_INFO,  __FILE__, __LINE__, __VA_ARGS__)
#define log_warn(...)  log_log(LOG_WARN,  __FILE__, __LINE__, __VA_ARGS__)
#define log_error(...) log_log(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define log_fatal(...) log_log(LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

void log_set_level(uint32_t level);
void log_set_lock(log_LockFn fn, void* lock_data);
int log_add_fp(FILE* fp, uint32_t level);
int log_add_callback(log_LogFn fn, void* udata, uint32_t level);

void log_log(uint32_t level, const char* file, uint32_t line, const char* fmt, ...);

#ifdef __cplusplus
}
#endif

#endif //APP_LOG_H
