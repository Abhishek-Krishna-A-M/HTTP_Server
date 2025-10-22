#include "../include/HotReload.h"
#include "../include/Config.h"
#include <pthread.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// global pointer to config is managed by Launch.c; we receive a pointer here.
static struct ServerConfig* g_config_ptr = NULL;
static char g_path[512];

static void* watcher(void* arg) {
    (void)arg;
    time_t last = 0;
    while (1) {
        struct stat st;
        if (stat(g_path, &st) == 0) {
            if ((time_t)st.st_mtime != last) {
                last = st.st_mtime;
                printf("[hotreload] detected change, reloading config '%s'\n", g_path);
                struct ServerConfig newcfg = load_config(g_path);
                if (g_config_ptr) {
                    memcpy(g_config_ptr, &newcfg, sizeof(struct ServerConfig));
                    printf("[hotreload] reload complete\n");
                }
            }
        }
        sleep(2);
    }
    return NULL;
}

void start_hot_reload(const char* path, struct ServerConfig* config_ptr) {
    strncpy(g_path, path, sizeof(g_path)-1);
    g_path[sizeof(g_path)-1]=0;
    g_config_ptr = config_ptr;
    pthread_t tid;
    pthread_create(&tid, NULL, watcher, NULL);
    pthread_detach(tid);
}

