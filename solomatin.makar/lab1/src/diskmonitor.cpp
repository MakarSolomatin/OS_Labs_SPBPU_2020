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

bool DiskMonitor::runnable = true;
void DiskMonitor::applyConfig(const string &configFile) {
    removeWatches();

    configParser.parse(configFile); // possible exception
    vector<string> dirs = configParser.get(ConfigParser::Token::DIRECTORY);

    for (auto &dir : dirs) addWatch(dir);
}

void DiskMonitor::removeWatches() {
    for (auto it = wds.begin(); it != wds.end(); it++) inotify_rm_watch(inotifyFd, it->first);
    wds.clear();
}

DiskMonitor::DiskMonitor() {
    if ((inotifyFd = inotify_init()) < 0) {
        syslog(LOG_ERR, "Could not initialize inotify");
        return;
    }
}

DiskMonitor::~DiskMonitor() {
    if (runnable) finish();
}

void DiskMonitor::run() {
    char *event_buf = new char[BUF_LEN];
    while (runnable) {
        int length = read(inotifyFd, event_buf, BUF_LEN);
        if (length < 0) {
            syslog(LOG_ERR, "Read from inotify returned -1");
        }

        for (int i = 0; i < length; i++) {
            inotify_event *event = (inotify_event *)(event_buf + i);
            if (event->len) {
                if (event->mask & IN_CREATE) {
                    if (event->mask & IN_ISDIR)
                        syslog(LOG_ERR, "%s DIR::%s CREATED\n", wds[event->wd].c_str(),event->name);
                    else
                        syslog(LOG_ERR, "%s FILE::%s CREATED\n", wds[event->wd].c_str(), event->name);
                }
                if (event->mask & IN_MODIFY) {
                    if (event->mask & IN_ISDIR)
                        syslog(LOG_ERR, "%s DIR::%s MODIFIED\n", wds[event->wd].c_str(),event->name);
                    else
                        syslog(LOG_ERR, "%s FILE::%s MODIFIED\n", wds[event->wd].c_str(),event->name);
                }
                if (event->mask & IN_DELETE) {
                    if (event->mask & IN_ISDIR)
                        syslog(LOG_ERR,"%s DIR::%s DELETED\n", wds[event->wd].c_str(),event->name );
                    else
                        syslog(LOG_ERR,"%s FILE::%s DELETED\n", wds[event->wd].c_str(),event->name );
                }
            }
            i += EVENT_SIZE + event->len;
        }
    }

    syslog(LOG_INFO, "Exited from endless while loop");
    delete[] event_buf;
}

void DiskMonitor::addWatch(const string &dir) {
    const char *dirPointer = dir.c_str();
    int wd = inotify_add_watch(inotifyFd, dirPointer, IN_CREATE | IN_MODIFY | IN_DELETE);
    if (wd == -1) {
        syslog(LOG_ERR, "Could not add watch to %s\n", dirPointer);
    } else {
        wds[wd] = dir;
        syslog(LOG_INFO, "Watching %s\n", dirPointer);
    }
}
