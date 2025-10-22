#include "../include/FileHandler.h"
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

bool file_exists(const char* path) {
    struct stat st;
    return (stat(path, &st) == 0);
}

size_t read_file(const char* path, char** out_buffer) {
    FILE* f = fopen(path, "rb");
    if (!f) return 0;
    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    fseek(f, 0, SEEK_SET);
    *out_buffer = malloc(size);
    if (!*out_buffer) { fclose(f); return 0; }
    size_t r = fread(*out_buffer, 1, size, f);
    fclose(f);
    return r;
}

const char* get_mime_type(const char* path) {
    const char* ext = strrchr(path, '.');
    if (!ext) return "application/octet-stream";
    if (strcmp(ext, ".html")==0) return "text/html";
    if (strcmp(ext, ".css")==0) return "text/css";
    if (strcmp(ext, ".js")==0) return "application/javascript";
    if (strcmp(ext, ".json")==0) return "application/json";
    if (strcmp(ext, ".png")==0) return "image/png";
    if (strcmp(ext, ".jpg")==0 || strcmp(ext,".jpeg")==0) return "image/jpeg";
    if (strcmp(ext, ".svg")==0) return "image/svg+xml";
    if (strcmp(ext, ".ico")==0) return "image/x-icon";
    return "application/octet-stream";
}

const char* generate_etag(const char* path) {
    static char etag[64];
    struct stat st;
    if (stat(path, &st) == 0) {
        snprintf(etag, sizeof(etag), "\"%lx-%lx\"", (unsigned long)st.st_mtime, (unsigned long)st.st_size);
    } else {
        snprintf(etag, sizeof(etag), "\"0-0\"");
    }
    return etag;
}
