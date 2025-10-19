#include "../include/Request.h"
#include <stdio.h>
#include <string.h>

struct HttpRequest parse_request(const char* request_str) {
    struct HttpRequest req;
    req.header_count = 0;
    req.host[0] = '\0';  // Initialize host to empty string

    // Temporary buffer
    char buffer[2048];
    strncpy(buffer, request_str, sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';

    // Parse method and path from the first line
    char *line = strtok(buffer, "\r\n");
    if (line != NULL) {
        sscanf(line, "%s %s %s", req.method, req.path, req.http_version);
    }

    // Parse headers
    while ((line = strtok(NULL, "\r\n")) && strlen(line) > 0) {
        char key[MAX_METHOD_LEN], value[MAX_VALUE_LEN];
        if (sscanf(line, "%[^:]: %[^\n]", key, value) == 2) {
            strncpy(req.headers[req.header_count].key, key, MAX_METHOD_LEN);
            strncpy(req.headers[req.header_count].value, value, MAX_VALUE_LEN);
            req.header_count++;

            // If this header is "Host", copy it to req.host
            if (strcasecmp(key, "Host") == 0) {
                strncpy(req.host, value, MAX_PATH_LEN);
            }
        }
    }

    return req;
}
