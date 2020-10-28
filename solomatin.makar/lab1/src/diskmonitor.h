#ifndef DISKMONITOR_H
#define DISKMONITOR_H

#include <map>
#include <syslog.h>
#include "config_parser.h"

using namespace std;

// disk monitoring logic encapsulated here, SINGLETON
class DiskMonitor {
    ConfigParser configParser;
    static bool runnable;

    // watch descriptors
    map<int, string> wds;

    // inotify instance descriptor
    int inotifyFd = -1;

    // read config file and recreate all watches accordingly, throws exception on fail

    // recursively add watches starting with directory
    void addWatch(const string &dir);

    // remove all active watches
    void removeWatches();

    DiskMonitor();
public:
    void applyConfig(const string &configFile);
    ~DiskMonitor();

    void run();
    void finish() {
        runnable = false;
        removeWatches();
        syslog(LOG_INFO, "Removed all watches");
        close(inotifyFd);
        syslog(LOG_INFO, "Inotify file closed");
    }

    static DiskMonitor & instance() {
        static DiskMonitor instance;
        return instance;
    }
};

#endif /*DISKMONITOR_H*/
