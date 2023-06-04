#ifndef UTILS_H
#define UTILS_H

#include <pthread.h>

#include "log.h"

static void log_lock(bool lock, void* lock_data);

pthread_mutex_t my_mutex;

void set_mutex_lock() {
    pthread_mutex_init(&my_mutex, NULL);
    log_set_lock(log_lock, &my_mutex);
}
FILE* set_log_file(const char* filename, uint32_t level) {
    FILE* fp = fopen(filename, "a+");
    log_add_fp(fp, level);
    return fp;
}
void destroy_mutex_lock() {
    pthread_mutex_destroy(&my_mutex);
}

#pragma mark - static

static void log_lock(bool lock, void* lock_data) {
    pthread_mutex_t* LOCK = (pthread_mutex_t*)lock_data;
    if (lock) {
        pthread_mutex_lock(LOCK);
    } else {
        pthread_mutex_unlock(LOCK);
    }
}


#endif