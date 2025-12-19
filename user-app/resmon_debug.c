#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <time.h>
#include <string.h>
#include <errno.h>

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

int main() {
    struct resource_info info;
    
    printf("Testing syscall multiple times...\n\n");
    
    for (int i = 0; i < 5; i++) {
        memset(&info, 0, sizeof(info));
        
        long ret = syscall(SYS_RESMON, &info);
        
        if (ret < 0) {
            printf("Syscall failed: %s\n", strerror(errno));
            return 1;
        }
        
        printf("Call %d:\n", i+1);
        printf("  RAM: %lu KB used / %lu KB total\n", info.used_ram, info.total_ram);
        printf("  Procs: %lu\n", info.procs);
        printf("  Uptime: %lu seconds\n", info.uptime);
        printf("  Load: %.2f\n\n", info.load_1 / 65536.0);
        
        sleep(2);
    }
    
    return 0;
}
