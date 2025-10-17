#include "../include/Logger.h"

void log_request(const char* client_ip, const char* method, const char* path, int status_code) {
    time_t now = time(NULL);
    char time_str[64];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", localtime(&now));

    printf("[%s] %s %s %s %d\n", time_str, client_ip, method, path, status_code);

    FILE* f = fopen("server.log", "a");
    if (f) {
        fprintf(f, "[%s] %s %s %s %d\n", time_str, client_ip, method, path, status_code);
        fclose(f);
    }
}

