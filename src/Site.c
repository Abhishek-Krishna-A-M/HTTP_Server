#include "../include/Site.h"
#include <string.h>

const char* get_site_root(struct ServerConfig* config, const char* host) {
    for (int i = 0; i < config->site_count; i++) {
        if (strcmp(config->sites[i].host, host) == 0) {
            return config->sites[i].root;
        }
    }
    // fallback to default site
    if (config->site_count > 0) return config->sites[0].root;
    return "www";
}
