#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>
#include <linux/mm.h>
#include <linux/sched/signal.h>
#include <linux/sched/stat.h>
#include <linux/sched/loadavg.h>
#include <linux/swap.h>
#include <linux/timekeeping.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>
#include <linux/mm.h>
#include <linux/sched/signal.h>
#include <linux/sched/stat.h>
#include <linux/swap.h>
#include <linux/timekeeping.h>

struct resource_info {
    unsigned long total_ram;
    unsigned long free_ram;
    unsigned long used_ram;
    unsigned long total_swap;
    unsigned long free_swap;
    unsigned long procs;
    unsigned long uptime;
    unsigned long load_1;
    unsigned long load_5;
    unsigned long load_15;
};

SYSCALL_DEFINE1(resmon, struct resource_info __user *, info)
{
    struct resource_info kinfo;
    struct sysinfo si;
    struct timespec64 tp;
    unsigned long loads[3];
    
    if (!info)
        return -EINVAL;
    
    memset(&kinfo, 0, sizeof(kinfo));
    memset(&si, 0, sizeof(si));
    
    // Get uptime
    ktime_get_boottime_ts64(&tp);
    kinfo.uptime = tp.tv_sec;
    
    // Get load average
    get_avenrun(loads, 0, SI_LOAD_SHIFT - FSHIFT);
    kinfo.load_1 = loads[0];
    kinfo.load_5 = loads[1];
    kinfo.load_15 = loads[2];
    
    // Get process count
    kinfo.procs = nr_threads;
    
    // Get memory info
    si_meminfo(&si);
    kinfo.total_ram = si.totalram * (si.mem_unit / 1024);
    kinfo.free_ram = si.freeram * (si.mem_unit / 1024);
    kinfo.used_ram = kinfo.total_ram - kinfo.free_ram;
    
    // Get swap info
    si_swapinfo(&si);
    kinfo.total_swap = si.totalswap * (si.mem_unit / 1024);
    kinfo.free_swap = si.freeswap * (si.mem_unit / 1024);
    
    if (copy_to_user(info, &kinfo, sizeof(kinfo)))
        return -EFAULT;
    
    return 0;
}
