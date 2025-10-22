#include "../include/CLI.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void parse_cli_args(int argc, char* argv[], char* config_path, int* out_port, int* hotreload_flag) {
    // defaults already set by caller
    for (int i=1;i<argc;i++) {
        if (strcmp(argv[i], "--config")==0 && i+1<argc) {
            strncpy(config_path, argv[i+1], 511);
            i++;
        } else if ((strcmp(argv[i], "-p")==0 || strcmp(argv[i],"--port")==0) && i+1<argc) {
            *out_port = atoi(argv[i+1]); i++;
        } else if (strcmp(argv[i], "--hotreload")==0) {
            *hotreload_flag = 1;
        } else if (strcmp(argv[i], "status")==0) {
            printf("Status command will be implemented in future (use logs/status endpoints).\n");
        } else if (strcmp(argv[i], "reload")==0) {
            // noop here; hotreload thread handles reload, or use admin endpoint
            printf("Use admin endpoint or modify server.json (hot reload) to apply changes.\n");
        } else if (strcmp(argv[i], "logs")==0) {
            system("tail -n 50 access.log");
        } else {
            printf("Unknown arg: %s\n", argv[i]);
        }
    }
}

