#include <stdlib.h>
#include <syslog.h>
#include <stdarg.h>
#include <signal.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <fstream>
#include <string.h>
#include "child.h"

Child::Child() {
    setsid(); // create session
    umask(027); // drop group and other users permissions (rw-r-----)

    openlog("DiskMonitor", LOG_PID, LOG_LOCAL0);

    // create lock file for daemon
    int lock_file = open(lockf_name, O_RDWR | O_CREAT, 0644);
    if (lock_file < 0) {
        syslog(LOG_ERR, "Error while opening lock file %s", lockf_name);
        throw "Error on child creation";
    }

    if (lockf(lock_file, F_TLOCK, 0) < 0) {
        syslog(LOG_ERR, "Error while locking file %s", lockf_name);
        throw "Error on child creation";
    }

    {
        string pid_str = std::to_string(getpid());
        write(lock_file, &pid_str[0], pid_str.size());
    }
    close(lock_file);

    // close all owned file desrciptors
    for (int i = getdtablesize(); i >= 0; --i) close(i);
    int i = open("/dev/null", O_RDWR);
    dup(i);
    dup(i);

    // set callbacks for signal handling
    signal(SIGHUP, handleHangUp);
    signal(SIGTERM, handleTerm);

    syslog(LOG_INFO, "Successfully initialized child process");
    disk_monitor = new DiskMonitor("/home/makar");
}

Child::~Child() {
    delete disk_monitor;

    syslog(LOG_INFO, "Exiting gracefully. Good bye!");
    closelog();
}

void Child::handleTerm(int sig) {
    syslog(LOG_INFO, "Received SIGTERM");
    DiskMonitor::finish();
}

void Child::handleHangUp(int sig) {
    syslog(LOG_INFO, "Received SIGHUP");
}

void Child::run() {
    disk_monitor->run();
}