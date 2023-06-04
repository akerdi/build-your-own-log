# Log

本章目标: 实现log 的正常打印输出，并且支持设置日志等级。

## 实现

提供一个结构存储需要输出的数据:

```c++
typedef struct {
    const char* fmt;    // 输出格式
    const char* file;   // 记录文件名
    struct tm* time;    // 记录时间
    void* udata;        // 输出目标.e.g. FILE*
    uint32_t line;      // 记录文件行号
    uint32_t level;     // 当次输出日志等级
} Log_Event;
```

设置一个全局变量，存储日志打印等级，并且支持修改:

```c++
static struct {
    uint32_t level;     // 日志打印等级
} L;
void log_set_level(uint32_t level) {
    L.level = level;
}
```

提供输出log方法，该方法接受日志等级、文件名、文件行号、输出格式以及不定参数:

```c++
// 设置time 和udata
void init_event(Log_Event* ev, void* udata) {
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
        FILE* file = stdout;
        if (level > LOG_WARN) f = stderr;
        init_event(&ev, f);
        va_start(ev.ap, fmt);
        out_callback(&ev);
        va_end(ev.app);
    }
}
```

上面实现了打印日志入口，`out_callback` 最终写入输出:

```c++
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
```

在调用`void(*log_log)(uint32_t level, const char* file, uint32_t line, const char* fmt, ...)`后，即可正确打印日志。

为了方便打印，增加宏定义:

```c++
#define log_trace(...) log_log(LOGTRACE, __FILE__, __LINE__, __VA_ARGS__)
#define log_debug(...) log_log(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define log_info(...)  log_log(LOG_INFO,  __FILE__, __LINE__, __VA_ARGS__)
#define log_warn(...)  log_log(LOG_WARN,  __FILE__, __LINE__, __VA_ARGS__)
#define log_error(...) log_log(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define log_fatal(...) log_log(LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)
```

至此，实现基础日志打印功能。
