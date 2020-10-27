#ifndef CHILD_H
#define CHILD_H

#include "diskmonitor.h"

// class represents daemon process
class Child {
    DiskMonitor *disk_monitor = nullptr;
    const char *lockf_name = "/var/run/lab1.pid";

    // signal handlers
    static void handleTerm(int);
    static void handleHangUp(int);

    Child();
    ~Child();
    Child(const Child &) = delete;
    Child& operator=(Child &) = delete;
public:
    static Child & instance() {
        static Child instance; // can throw exceptions
        return instance;
    }
    void run();
};

#endif /*CHILD_H*/
