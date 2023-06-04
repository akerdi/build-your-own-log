//
// Created by 0210 on 2023/6/3.
//
#include "log.h"

int main() {
    FILE* fp = fopen("file.log", "ab+");
    log_add_fp(fp, LOG_WARN);
    log_debug("?????");
    log_debug("hello %s", "world");
    log_fatal("I meet error! Error: %s!", "fatal");
    fclose(fp);
}