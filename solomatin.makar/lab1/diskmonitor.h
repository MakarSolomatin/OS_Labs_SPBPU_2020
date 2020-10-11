#ifndef DISKMONITOR_H
#define DISKMONITOR_H

#include <map>
#include "config_parser.h"

using namespace std;

// class contains logic of disk monitor
class DiskMonitor {
    ConfigParser config_parser;
    static map<string, string> defaults;
    bool runnable;
public:
    DiskMonitor(string config_file = "/etc/diskmonitor/config");
    ~DiskMonitor();

    void run();
};

#endif /*DISKMONITOR_H*/
