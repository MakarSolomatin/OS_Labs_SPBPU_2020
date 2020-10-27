#ifndef DISKMONITOR_H
#define DISKMONITOR_H

#include <map>
#include <syslog.h>
#include "config_parser.h"

using namespace std;

// disk monitoring logic encapsulated here
class DiskMonitor {
    ConfigParser config_parser;
    static map<string, string> defaults; // move to config_parser
    static bool runnable;

    // inotify instance descriptor
    int inotify_fd = -1;

    // recursively add watches starting with directory
    bool addWatch(const string &dir);
public:
    DiskMonitor(string config_file = "/etc/diskmonitor/config");
    ~DiskMonitor();

    void run();
    static void finish() {
        syslog(LOG_NOTICE/*LOG_INFO*/, "Finishing DiskMonitor gracefully...");
        runnable = false;
    }
};

#endif /*DISKMONITOR_H*/
