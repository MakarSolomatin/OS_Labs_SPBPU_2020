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

int main(int argc, char *argv[]) {
    static const char *help =
        "Usage: \n"
        "diskmonitor [-f config_file]\n"
        "-f            path to diskmonitor configuration file\n";

    string config_file = "/etc/diskmonitor/config";
    if (argc > 1) {
        if (argc != 3 || strcmp("-f", argv[1])) {
            puts(help);
            exit(1);
        }
        config_file = argv[2];
    }
    Child child(fork());
    return 0;
}
