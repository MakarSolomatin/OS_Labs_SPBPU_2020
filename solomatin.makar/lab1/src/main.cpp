#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "child.h"
#include "config_parser.h"

auto main(int argc, char *argv[]) -> int {
    static const char *help =
        "\x1b[4mUsage\x1b[0m: \n"
        "\x1b[1;32mdiskmonitor\x1b[0m [-f config_file]\n"
        "\x1b[33m-f\x1b[0m     path to diskmonitor configuration file\n";

    string configFile = "/etc/diskmonitor/config";
    if (argc > 1) {
        if (argc != 3 || strcmp("-f", argv[1])) {
            puts(help);
            return 1;
        }
        configFile = argv[2];
    }

    int pid = fork();
    if (pid < 0) {
        perror("Error while forking");
        return 1;
    }
    else if (pid == 0) { //child process
        try {
            Child &child = Child::instance();
            child.run(configFile);
        } catch (const char *str) {
            return 1;
        }
    }
    return 0;
}
