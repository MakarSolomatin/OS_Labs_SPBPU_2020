#include <cstdio>
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
    writePid(pidFileName);

    // close all owned file desrciptors
    for (int i = getdtablesize(); i >= 0; --i) close(i);
    int i = open("/dev/null", O_RDWR);
    dup(i);
    dup(i);

    // set callbacks for signal handling
    signal(SIGHUP, handleHangUp);
    signal(SIGTERM, handleTerm);

    syslog(LOG_INFO, "Successfully initialized child process");
    diskMonitor = &DiskMonitor::instance();
}

void Child::writePid(const char *fname) {
    int pidFile;

    if (access(fname, F_OK) != -1) {
        int pidFile = open(fname, O_RDONLY, 0644);

        // get pid from file
        char buf[9];
        read(pidFile, buf, 9);
        if (strlen(buf)) {
            int pid = atoi(buf);

            // check if process with this pid exists
            string procFileName = string("/proc/") + buf;
            if (access(procFileName.c_str(), F_OK ) != -2) kill(pid, SIGTERM);
        }

        close(pidFile);
    }

    pidFile = open(fname, O_RDWR | O_CREAT, 0644);
    if (pidFile < 0) {
        perror("Could not open pid file");
        throw "Error on child creation";
    }

    if (lockf(pidFile, F_TLOCK, -1) < 0) {
        perror("Could not lock pid file");
        throw "Error on child creation";
    }
    string pidString = std::to_string(getpid());
    write(pidFile, &pidString[0], pidString.size());

    close(pidFile);
}

Child::~Child() {
    syslog(LOG_INFO, "Child process terminate");
    closelog();
}

void Child::handleTerm(int sig) {
    syslog(LOG_INFO, "Received SIGTERM");
    DiskMonitor::instance().finish();
}

void Child::handleHangUp(int sig) {
    syslog(LOG_INFO, "Received SIGHUP");
    DiskMonitor::instance().applyConfig(Child::instance().configFile);
}

void Child::run(const string &configFile) {
    this->configFile = configFile;

    diskMonitor->applyConfig(configFile);
    diskMonitor->run();
}
