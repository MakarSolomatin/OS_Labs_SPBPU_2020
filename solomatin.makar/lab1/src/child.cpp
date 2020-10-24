#include <stdlib.h>
#include <syslog.h>
#include <stdarg.h>
#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "child.h"

Child::Child(int pid) : pid(pid) {
    if (pid < 0) {
        perror("Error while forking");
        exit(1);
    } else if (pid > 0) {
        exit(0); // parent process exits here
    } // child process continues

    setsid(); // create session
    umask(027);

    // lock file
    int lock_file = open(lock_file_name, O_RDWR | O_CREAT, 0640);
    if (lock_file < 0) {
        perror("Error while opening lock file");
        exit(1);
    }

    if (lockf(lock_file, F_TLOCK, 0) < 0) {
        perror("Could not lock file");
        exit(1);
    }

    char str[10];
    sprintf(str, "%d\n", getpid());
    write(lock_file, str, strlen(str));
    close(lock_file);

    // close all owned file desrciptors
    for (int i = getdtablesize(); i >= 0; --i) close(i);
    int i = open("/dev/null", O_RDWR);
    dup(i);
    dup(i);

    signal(SIGHUP, handleHangUp);
    signal(SIGTERM, handleTerm);

    disk_monitor = new DiskMonitor("/home/makar");
    disk_monitor->run();
}

Child::~Child() {
    delete disk_monitor;
}

void Child::handleTerm(int sig) {
    DiskMonitor::finish();
}

void Child::handleHangUp(int sig) {
}
