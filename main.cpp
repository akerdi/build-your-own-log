//
// Created by 0210 on 2023/6/3.
//
#include "utils.hpp"

int main() {
    FILE* fp = set_log_file("aa/file.log", LOG_WARN);
    set_mutex_lock();
    log_debug("?????");
    log_debug("hello %s", "world");
    log_fatal("I meet error! Error: %s!", "fatal");
    fclose(fp);
    destroy_mutex_lock();
}