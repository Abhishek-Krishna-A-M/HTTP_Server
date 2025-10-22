#include "include/Server.h"
#include "include/Config.h"
#include "include/Launch.h"
#include "include/HotReload.h"
#include "include/CLI.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

int main(int argc, char* argv[]) {
    char config_path[512] = "server.json";
    int port = 0;
    int hotreload = 0;

    parse_cli_args(argc, argv, config_path, &port, &hotreload);

    struct ServerConfig config = load_config(config_path);
    if (port==0) port = config.default_port;
    config.default_port = port;

    struct Server server = server_constructor(port, 128, &config);

    if (hotreload) start_hot_reload(config_path, &config);

    // launch server loop
    launch_server_with(&server);

    return 0;
}
