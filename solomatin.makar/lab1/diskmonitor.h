#ifndef DISKMONITOR_H
#define DISKMONITOR_H

// class contains logic of disk monitor
class DiskMonitor {
    const char *log_file = "/var/log/";
public:
    DiskMonitor(const char *log_file);
    ~DiskMonitor();

    void run();
};

#endif /*DISKMONITOR_H*/
