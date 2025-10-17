#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <time.h>

// Log request info
void log_request(const char* client_ip, const char* method, const char* path, int status_code);

#endif

