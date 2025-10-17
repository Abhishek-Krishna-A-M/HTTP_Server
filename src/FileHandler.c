#include "../include/FileHandler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

const char* get_mime_type(const char* path) {
    const char* ext = strrchr(path, '.');
    if (!ext) return "text/plain";
    if (strcmp(ext, ".html") == 0) return "text/html";
    if (strcmp(ext, ".css") == 0) return "text/css";
    if (strcmp(ext, ".js") == 0) return "application/javascript";
    if (strcmp(ext, ".json") == 0) return "application/json";
    if (strcmp(ext, ".png") == 0) return "image/png";
    if (strcmp(ext, ".jpg") == 0) return "image/jpeg";
    if (strcmp(ext, ".gif") == 0) return "image/gif";
    if (strcmp(ext, ".ico") == 0) return "image/x-icon";
    return "application/octet-stream";
}

bool file_exists(const char* path) {
    struct stat st;
    return stat(path, &st) == 0;
}

size_t read_file(const char* path, char** buffer) {
    FILE* f = fopen(path, "rb");
    if (!f) return 0;

    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    rewind(f);

    *buffer = malloc(size);
    if (!*buffer) {
        fclose(f);
        return 0;
    }

    fread(*buffer, 1, size, f);
    fclose(f);
    return size;
}

