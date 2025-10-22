#include "../include/Logger.h"
#include <stdio.h>
#include <time.h>

void log_request(const char* ip, const char* method, const char* path, int status) {
    FILE* f = fopen("access.log", "a");
    if (!f) return;
    time_t t = time(NULL);
    char ts[64];
    struct tm tm;
    localtime_r(&t, &tm);
    strftime(ts, sizeof(ts), "%Y-%m-%d %H:%M:%S", &tm);
    fprintf(f, "[%s] %s %s %s %d\n", ts, ip ? ip : "-", method ? method : "-", path ? path : "-", status);
    fclose(f);
}
