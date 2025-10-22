#include "../include/Headers.h"
#include <stdio.h>
#include <string.h>

void add_common_headers(char* buf, size_t buf_len) {
    // caller should append this to the response headers before final CRLFCRLF
    // This function fills the buffer with additional header lines.
    snprintf(buf, buf_len,
        "Access-Control-Allow-Origin: *\r\n"
        "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n"
        "Access-Control-Allow-Headers: Content-Type\r\n"
        "X-Powered-By: Custom-C-HTTP-Server\r\n");
}
