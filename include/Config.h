#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>

#define MAX_SITES 10
#define MAX_PATH_LEN 512

struct Site {
    char host[MAX_PATH_LEN];
    char root[MAX_PATH_LEN];
};

struct ServerConfig {
    int port;
    bool spa_mode;
    bool remote_access;
    int log_level;
    int site_count;
    struct Site sites[MAX_SITES];
};

// Load server config from JSON
struct ServerConfig load_config(const char* path);

const char* get_site_root(struct ServerConfig* config, const char* host);


#endif
