#ifndef PROCESSLIST_H
#define PROCESSLIST_H
//#include <unistd.h>
#include <string>
#include <vector>
using namespace std;

struct Process
{
    unsigned long cpuTime,cpuTotalTime;
    //pid_t pid;
    int pid;
    double cpuUsage;
    double memoryUsage;
    string comm,
        task_state;
    int ppid,
        pgid,
        sid,
        tty_nr,
        tty_pgrp;
    unsigned int task_flags;
    unsigned long min_flt,
        cmin_flt,
        maj_flt,
        cmaj_flt,
        utime,
        stime;
    long cutime,
        cstime,
        priority,
        nice,
        num_threads,
        it_real_value;
    unsigned long long start_time;
    unsigned long vsize;
    long rss;
    unsigned long rsslim;
};
#endif // PROCESSLIST_H
