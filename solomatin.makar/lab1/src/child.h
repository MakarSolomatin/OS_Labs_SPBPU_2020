#ifndef CHILD_H
#define CHILD_H

#include "diskmonitor.h"

// class represents daemon process
class Child {
    int pid;
    DiskMonitor *disk_monitor = nullptr;

    const char *lock_file_name = "/var/run/lab1.pid";

    // signal handlers
    static void handleTerm(int);
    static void handleHangUp(int);
public:
    Child(int pid);
    ~Child();
};

#endif /*CHILD_H*/
