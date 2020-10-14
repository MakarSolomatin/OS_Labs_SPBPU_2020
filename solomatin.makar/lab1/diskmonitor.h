#ifndef DISKMONITOR_H
#define DISKMONITOR_H

#include <map>
#include <syslog.h>
#include "config_parser.h"

using namespace std;

// class contains logic of disk monitor
class DiskMonitor {
    ConfigParser config_parser;
    static map<string, string> defaults;
    static bool runnable;
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
