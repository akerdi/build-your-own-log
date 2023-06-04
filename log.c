//
// Created by 0210 on 2023/6/3.
//

#include <stdarg.h>

#include "log.h"

#define MAX_CALLBACKS 12

typedef struct {
    log_LogFn fn;
    void* udata;
    int level;
} Callback;
static struct {
    void* lock_data;
    log_LockFn lock;
    uint32_t level;
    Callback callbacks[MAX_CALLBACKS];
} L;

static const char* level_strings[] = {"TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"};
static void out_callback(Log_Event* ev, bool is_file) {
    char buf[64];
    char* time_format = "%H:%M:%S";
    if (is_file) time_format = "%Y-%m-%d %H:%M:%S";

    buf[strftime(buf, sizeof(buf), time_format, ev->time)] = '\0';
    FILE* f = (FILE*)ev->udata;
    fprintf(f, "%s %-5s %s:%d: ",
            buf, level_strings[ev->level], ev->file, ev->line
            );
    vfprintf(f, ev->fmt, ev->ap);
    fprintf(f, "\n");
    fflush(f);
}
static void lock(bool is_lock) {
    if (L.lock) L.lock(is_lock, L.lock_data);
}
void log_set_lock(log_LockFn fn, void* lock_data) {
    L.lock = fn;
    L.lock_data = lock_data;
}
void log_set_level(uint32_t level) {
    L.level = level;
}
int log_add_callback(log_LogFn fn, void* udata, uint32_t level) {
    for (int i = 0; i < MAX_CALLBACKS; i++) {
        if (!L.callbacks[i].fn) {
            L.callbacks[i] = (Callback){out_callback, udata, level};
            return 0;
        }
    }
    return -1;
}
int log_add_fp(FILE* fp, uint32_t level) {
    return log_add_callback(out_callback, fp, level);
}
static void init_event(Log_Event* ev, void* udata) {
    if (!ev->time) {
        time_t t = time(NULL);
        ev->time = localtime(&t);
    }
    ev->udata = udata;
}
void log_log(uint32_t level, const char* file, uint32_t line, const char* fmt, ...) {
    Log_Event ev = {
            .fmt = fmt,
            .file = file,
            .line = line,
        .level = level,
    };
    lock(true);
    if (level >= L.level) {
        FILE* f = stdout;
        if (level >= LOG_WARN) f = stderr;
        init_event(&ev, f);
        va_start(ev.ap, fmt);
        out_callback(&ev, false);
        va_end(ev.ap);
    }
    for (int i = 0; i < MAX_CALLBACKS; i++) {
        Callback* cb = &L.callbacks[i];
        if (cb->udata && level >= cb->level) {
            init_event(&ev, cb->udata);
            va_start(ev.ap, fmt);
            cb->fn(&ev, true);
            va_end(ev.ap);
        }
    }
    lock(false);
}
