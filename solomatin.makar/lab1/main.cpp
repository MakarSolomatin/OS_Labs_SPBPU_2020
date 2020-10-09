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

void prepare_parent() {
    for (int i = getdtablesize(); i >= 0; --i) close(i);
    int i = open("/dev/null",O_RDWR);
    dup(i);
    dup(i);

    umask(027);
}

int main() {
    prepare_parent();
    Child child(fork());
    return 0;
}
