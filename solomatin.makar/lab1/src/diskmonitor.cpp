#include <syslog.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <map>
#include <dirent.h>
#include <sys/inotify.h>
#include "diskmonitor.h"

#define MAX_LEN 1024
#define MAX_EVENTS 1024
#define LEN_NAME 16
#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define BUF_LEN     ( MAX_EVENTS * ( EVENT_SIZE + LEN_NAME ))

bool DiskMonitor::runnable = false;

DiskMonitor::DiskMonitor(string config_file) {
    config_parser.parse(config_file);
    vector<string> dirs = config_parser.get(ConfigParser::Token::DIRECTORY);

    if ((inotify_fd = inotify_init()) < 0) {
        syslog(LOG_ERR, "Could not initialize inotify");
        return;
    }

    for (auto &dir : dirs) addWatch(dir);
}

DiskMonitor::~DiskMonitor() {
    close(inotify_fd);
    syslog(LOG_INFO, "Inotify file closed");
}

void DiskMonitor::run() {
    const int MAX_BUF = 100;
    char *event_buf = new char[MAX_BUF];
    while (runnable) {
        int length = read(inotify_fd, event_buf, MAX_BUF);
        if (length < 0) {
            syslog(LOG_ERR, "Read from inotify error");
        }

        for (int i = 0; i < length; i++) {
            inotify_event *event = (inotify_event *)(event_buf + i);
            if (event->len) {
                if (event->mask & IN_CREATE) {
                    if (event->mask & IN_ISDIR)
                        syslog(LOG_ERR, "%d DIR::%s CREATED\n", event->wd,event->name);
                    else
                        syslog(LOG_ERR, "%d FILE::%s CREATED\n", event->wd, event->name);
                }
                if (event->mask & IN_MODIFY) {
                    if (event->mask & IN_ISDIR)
                        syslog(LOG_ERR, "%d DIR::%s MODIFIED\n", event->wd,event->name);
                    else
                        syslog(LOG_ERR, "%d FILE::%s MODIFIED\n", event->wd,event->name);
                }
                if (event->mask & IN_DELETE) {
                    if (event->mask & IN_ISDIR)
                        syslog(LOG_ERR,"%d DIR::%s DELETED\n", event->wd,event->name );
                    else
                        syslog(LOG_ERR,"%d FILE::%s DELETED\n", event->wd,event->name );
                }
            }
            i += EVENT_SIZE + event->len;
        }
    }

    delete[] event_buf;
}

bool DiskMonitor::addWatch(const string &dir) {
    // open directory
    const char *dir_c = dir.c_str();
    DIR *dp = opendir(dir_c);
    if (dp == NULL) {
        syslog(LOG_ERR, "Error opening the starting directory");
        return false;
    }

    // add watch for root dir
    int wd = inotify_add_watch(inotify_fd, dir_c, IN_CREATE | IN_MODIFY | IN_DELETE);
    if (wd == -1) {
        syslog(LOG_ERR, "Could not add watch to %s\n", dir_c);
    } else {
        syslog(LOG_INFO, "Watching %s\n", dir_c);
    }

    // add watch for all subdirectories (1 level depth)
    string abs_dir;
    dirent *entry;
    while ((entry = readdir(dp))) {
        if (entry->d_type == DT_DIR) {
            abs_dir = dir + entry->d_name;
            const char * abs_dir_c = abs_dir.c_str();

            wd = inotify_add_watch(inotify_fd, abs_dir_c, IN_CREATE | IN_MODIFY | IN_DELETE);
            if (wd == -1)
                printf("Couldn't add watch to the directory %s\n", abs_dir_c);
            else
                printf("Watching:: %s\n", abs_dir_c);
        }
    }

    closedir(dp);

    return true;
}
