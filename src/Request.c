#include "../include/Request.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

static void trim_right(char* s) {
    int n = strlen(s);
    while(n>0 && (s[n-1]=='\r' || s[n-1]=='\n')) { s[n-1]=0; --n; }
}

struct HttpRequest parse_request(const char* request_str) {
    struct HttpRequest req;
    memset(&req,0,sizeof(req));
    req.header_count = 0;
    req.host[0]=0;

    char buf[8192];
    strncpy(buf, request_str, sizeof(buf)-1);
    buf[sizeof(buf)-1]=0;

    char* line = strtok(buf, "\r\n");
    if (line) {
        // first line: METHOD PATH HTTP/1.x
        sscanf(line, "%s %s %s", req.method, req.path, req.http_version);
    }

    while ((line = strtok(NULL, "\r\n")) != NULL && strlen(line)>0) {
        char key[64], value[1024];
        if (sscanf(line, "%63[^:]: %1023[^\r\n]", key, value) == 2) {
            trim_right(value);
            strncpy(req.headers[req.header_count].key, key, sizeof(req.headers[req.header_count].key)-1);
            strncpy(req.headers[req.header_count].value, value, sizeof(req.headers[req.header_count].value)-1);
            if (strcasecmp(key, "Host") == 0) {
                // take host (strip port if present)
                char hostbuf[256];
                strncpy(hostbuf, value, sizeof(hostbuf)-1);
                char* p = strchr(hostbuf, ':');
                if (p) *p = '\0';
                strncpy(req.host, hostbuf, sizeof(req.host)-1);
            }
            req.header_count++;
        }
    }
    return req;
}
