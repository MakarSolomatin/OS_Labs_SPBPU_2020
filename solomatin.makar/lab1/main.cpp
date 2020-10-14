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

void close_descriptors() {
    for (int i = getdtablesize(); i >= 0; --i) close(i);
    int i = open("/dev/stdout", O_RDWR);
    dup(i);
    dup(i);
}

int main(int argc, char *argv[]) {
    close_descriptors();
    Child child(fork());
    return 0;
}
