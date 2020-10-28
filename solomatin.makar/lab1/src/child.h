#ifndef CHILD_H
#define CHILD_H

#include "diskmonitor.h"

// class represents daemon process, SINGLETON
class Child {
    DiskMonitor *diskMonitor = nullptr;
    string configFile;

    const char *pidFileName = "/var/run/lab1.pid";

    // signal handlers
    static void handleTerm(int);
    static void handleHangUp(int);

    Child();
    ~Child();
    Child(const Child &) = delete;
    Child& operator=(Child &) = delete;

    void writePid(const char *fileName);
public:
    static Child & instance() {
        static Child instance; // can throw exceptions
        return instance;
    }
    void run(const string &configFile);
};

#endif /*CHILD_H*/
