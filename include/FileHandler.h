#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <stdbool.h>
#include <stdlib.h>

bool file_exists(const char* path);
size_t read_file(const char* path, char** out_buffer);
const char* get_mime_type(const char* path);
const char* generate_etag(const char* path);

#endif
