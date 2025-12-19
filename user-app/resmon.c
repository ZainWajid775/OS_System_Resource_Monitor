#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <time.h>
#include <string.h>
#include <errno.h>
#include <signal.h>

#define SYS_RESMON 451

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

volatile sig_atomic_t keep_running = 1;

void sig_handler(int sig) {
    keep_running = 0;
}

void log_to_file(struct resource_info *info) {
    FILE *fp = fopen("/var/log/resmon.log", "a");
    if (!fp) {
        fp = fopen("resmon.log", "a");
        if (!fp) {
            return;
        }
    }
    
    time_t now = time(NULL);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
    
    fprintf(fp, "[%s] RAM: %lu/%lu KB (%.1f%%) | Swap: %lu/%lu KB | Procs: %lu | Load: %.2f, %.2f, %.2f\n",
            timestamp,
            info->used_ram, info->total_ram,
            (info->used_ram * 100.0) / info->total_ram,
            (info->total_swap - info->free_swap), info->total_swap,
            info->procs,
            info->load_1 / 65536.0, 
            info->load_5 / 65536.0, 
            info->load_15 / 65536.0);
    
    fclose(fp);
}

void display_info(struct resource_info *info) {
    // Clear screen and move cursor to top
    printf("\033[2J\033[H");
    
    time_t now = time(NULL);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
    
    printf("╔════════════════════════════════════════════════════════╗\n");
    printf("║     SYSTEM RESOURCE MONITOR (resmon) - LIVE MODE       ║\n");
    printf("║     %s                               ║\n", timestamp);
    printf("╚════════════════════════════════════════════════════════╝\n\n");
    
    printf("📊 MEMORY (RAM)\n");
    printf("   Total:    %10lu KB  (%lu MB)\n", info->total_ram, info->total_ram / 1024);
    printf("   Used:     %10lu KB  (%lu MB) [%.1f%%]\n", 
           info->used_ram, info->used_ram / 1024,
           (info->used_ram * 100.0) / info->total_ram);
    printf("   Free:     %10lu KB  (%lu MB)\n", 
           info->free_ram, info->free_ram / 1024);
    
    // RAM usage bar
    int bar_width = 40;
    int filled = (int)((info->used_ram * bar_width) / info->total_ram);
    printf("   [");
    for (int i = 0; i < bar_width; i++) {
        if (i < filled) printf("█");
        else printf("░");
    }
    printf("]\n\n");
    
    printf("💾 SWAP\n");
    printf("   Total:    %10lu KB  (%lu MB)\n", info->total_swap, info->total_swap / 1024);
    printf("   Used:     %10lu KB  (%lu MB)\n", 
           info->total_swap - info->free_swap,
           (info->total_swap - info->free_swap) / 1024);
    printf("   Free:     %10lu KB  (%lu MB)\n\n", 
           info->free_swap, info->free_swap / 1024);
    
    printf("⚙️  SYSTEM\n");
    printf("   Processes: %lu\n", info->procs);
    printf("   Uptime:    %lu seconds (%.2f hours / %.2f days)\n", 
           info->uptime, info->uptime / 3600.0, info->uptime / 86400.0);
    printf("   Load Avg:  %.2f (1min) | %.2f (5min) | %.2f (15min)\n",
           info->load_1 / 65536.0,
           info->load_5 / 65536.0,
           info->load_15 / 65536.0);
    
    printf("\n📝 Logging to: /var/log/resmon.log or ./resmon.log\n");
    printf("⏸️  Press Ctrl+C to exit\n");
}

int main(int argc, char *argv[]) {
    struct resource_info info;
    long ret;
    int interval = 2;  // Default 2 seconds
    
    // Handle command line argument for interval
    if (argc > 1) {
        interval = atoi(argv[1]);
        if (interval < 1) interval = 2;
    }
    
    // Setup signal handler for clean exit
    signal(SIGINT, sig_handler);
    signal(SIGTERM, sig_handler);
    
    // First check if syscall works
    ret = syscall(SYS_RESMON, &info);
    if (ret < 0) {
        fprintf(stderr, "ERROR: System call failed!\n");
        fprintf(stderr, "Error code: %ld (%s)\n", ret, strerror(errno));
        fprintf(stderr, "\nTroubleshooting:\n");
        fprintf(stderr, "1. Check if custom kernel is running: uname -r\n");
        fprintf(stderr, "2. Verify syscall exists: cat /proc/kallsyms | grep sys_resmon\n");
        fprintf(stderr, "3. Check syscall number matches (should be 451)\n");
        return 1;
    }
    
    // Continuous monitoring loop
    while (keep_running) {
        ret = syscall(SYS_RESMON, &info);
        
        if (ret < 0) {
            fprintf(stderr, "\nERROR: System call failed during monitoring\n");
            break;
        }
        
        display_info(&info);
        log_to_file(&info);
        
        sleep(interval);
    }
    
    printf("\n\nMonitoring stopped. Log saved to resmon.log\n");
    return 0;
}
