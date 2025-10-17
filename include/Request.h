#ifndef REQUEST_H
#define REQUEST_H

#define MAX_HEADERS 100
#define MAX_METHOD_LEN 8
#define MAX_PATH_LEN 512
#define MAX_VALUE_LEN 1024

struct Header {
    char key[MAX_METHOD_LEN];
    char value[MAX_VALUE_LEN];
};


struct HttpRequest {
    char method[MAX_METHOD_LEN];   // GET, POST, etc.
    char path[MAX_PATH_LEN];       // URL path requested
    int header_count;              // Number of headers
    struct Header headers[MAX_HEADERS];
};

struct HttpRequest parse_request(const char* request_str);


#endif
