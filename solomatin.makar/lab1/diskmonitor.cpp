#include <syslog.h>
#include <stdio.h>
#include <unistd.h>
#include "diskmonitor.h"

DiskMonitor::DiskMonitor(const char *dir) {
    chdir(dir);

    openlog("DiskMonitor", 0/*LOG_PID*/, LOG_USER/*LOG_DAEMON*/);
    syslog(LOG_NOTICE/*LOG_INFO*/, "DiskMonitor started!");
}

DiskMonitor::~DiskMonitor() {
    closelog();
}

void DiskMonitor::run() {
    for (int i = 0;; i++) {
        sleep(1);
        syslog(LOG_INFO, "DiskMonitor up for %i seconds", i);
    }
}
