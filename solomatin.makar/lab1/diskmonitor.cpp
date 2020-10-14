#include <syslog.h>
#include <stdio.h>
#include <unistd.h>
#include <map>
#include "diskmonitor.h"

map<string, string> DiskMonitor::defaults = {
    {"directory", getenv("HOME")}
};

bool DiskMonitor::runnable = false;

DiskMonitor::DiskMonitor(string config) {
    openlog("DiskMonitor", 0/*LOG_PID*/, LOG_USER/*LOG_DAEMON*/);
    runnable = config_parser.parse(config);

    if (!runnable) {
        syslog(LOG_NOTICE/*LOG_INFO*/, "Error while parsing config, aborting...");
        return;
    }

    const char *dir = config_parser.get(ConfigParser::Token::DIRECTORY);
    if (dir == nullptr) {
        dir = defaults["directory"].c_str();
        syslog(LOG_NOTICE/*LOG_INFO*/, "Could not find directory in config, using %s instead", dir);
    }
    chdir(dir);
}

DiskMonitor::~DiskMonitor() {
    closelog();
}

void DiskMonitor::run() {
    if (!runnable) return;

    syslog(LOG_NOTICE/*LOG_INFO*/, "DiskMonitor started!");
    for (int i = 0;; i++) {
        sleep(1);
        syslog(LOG_INFO, "DiskMonitor up for %i seconds", i);

        if (!runnable) break;
    }
}
