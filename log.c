//
// Created by 0210 on 2023/6/3.
//

#include <stdarg.h>

#include "log.h"

static struct {
    uint32_t level;
} L;

static const char* level_strings[] = {"TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"};
static void out_callback(Log_Event* ev) {
    char buf[16];
    char* time_format = "%H:%M:%S";
    buf[strftime(buf, sizeof(buf), time_format, ev->time)] = '\0';
    FILE* f = (FILE*)ev->udata;
    fprintf(f, "%s %-5s %s:%d: ",
            buf, level_strings[ev->level], ev->file, ev->line
            );
    vfprintf(f, ev->fmt, ev->ap);
    fprintf(f, "\n");
    fflush(f);
}
void log_set_level(uint32_t level) {
    L.level = level;
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
    if (level >= L.level) {
        FILE* f = stdout;
        if (level >= LOG_WARN) f = stderr;
        init_event(&ev, f);
        va_start(ev.ap, fmt);
        out_callback(&ev);
        va_end(ev.ap);
    }
}
