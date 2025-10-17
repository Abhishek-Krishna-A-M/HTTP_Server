#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <stdbool.h>
#include <stddef.h>

#define MAX_PATH_LEN 512

// Detect MIME type based on file extension
const char* get_mime_type(const char* path);

// Check if file exists
bool file_exists(const char* path);

// Read file content into buffer (returns size)
size_t read_file(const char* path, char** buffer);

#endif

