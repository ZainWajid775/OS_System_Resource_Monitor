# Linux Kernel Custom System Call - Resource Monitor

A custom Linux kernel system call (`sys_resmon`) that provides real-time system resource monitoring including RAM, swap, process count, uptime, and load averages.

## Features

- ✅ Custom kernel system call implementation
- ✅ Real-time resource monitoring
- ✅ Live updating display with progress bars
- ✅ Automatic logging functionality
- ✅ Configurable update intervals
- ✅ Low overhead (< 1ms per call)

## System Requirements

- **OS:** Linux (Ubuntu/Debian recommended)
- **Architecture:** ARM64 (aarch64) or x86_64
- **Kernel Version:** 6.1.60
- **Disk Space:** 20GB+ free
- **RAM:** 4GB+ recommended

## Project Structure.
├── kernel-patches/       # Kernel modifications
│   ├── sys_resmon.c     # System call implementation
│   └── *.patch          # Patch files for kernel
├── user-app/            # User-space applications
│   ├── resmon.c         # Main monitoring application
│   └── resmon_debug.c   # Debug/test version
├── scripts/             # Installation scripts
├── docs/                # Documentation
└── README.md

## Quick Start

### 1. Install Prerequisites
```bash
cd scripts
./install_prerequisites.sh
```

### 2. Download Kernel Source
```bash
cd ~
wget https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.1.60.tar.xz
tar -xf linux-6.1.60.tar.xz
```

### 3. Apply Patches
```bash
cd linux-syscall-resmon/scripts
./apply_patches.sh ~/linux-6.1.60
```

### 4. Compile Kernel
```bash
./compile_kernel.sh ~/linux-6.1.60
```

### 5. Reboot and Select New Kernel
```bash
sudo reboot
# At GRUB menu: Advanced Options → Linux 6.1.60
```

### 6. Compile User Application
```bash
cd user-app
make
make install
```

### 7. Run Monitor
```bash
resmon          # Default: updates every 2 seconds
resmon 5        # Updates every 5 seconds
```

## Usage
```bash
# Basic usage
resmon

# Custom update interval (seconds)
resmon <interval>

# Examples
resmon 1        # Update every 1 second
resmon 10       # Update every 10 second

# Stop monitoring
Press Ctrl+C
```

## Output Example

╔════════════════════════════════════════════════════════╗
║     SYSTEM RESOURCE MONITOR (resmon) - LIVE MODE       ║
║     2024-12-14 10:30:15                               ║
╚════════════════════════════════════════════════════════╝
📊 MEMORY (RAM)
Total:       4000740 KB  (3907 MB)
Used:        1585404 KB  (1548 MB) [39.6%]
Free:        2415336 KB  (2359 MB)
[████████████████░░░░░░░░░░░░░░░░░░░░░░░░]
💾 SWAP
Total:       2097148 KB  (2048 MB)
Used:              0 KB  (0 MB)
Free:        2097148 KB  (2048 MB)
⚙️  SYSTEM
Processes: 597
Uptime:    55 seconds (0.02 hours / 0.00 days)
Load Avg:  1.04 (1min) | 1.05 (5min) | 0.98 (15min)
📝 Logging to: /var/log/resmon.log or ./resmon.log
⏸️  Press Ctrl+C to exit

## System Call Details

**System Call Number:** 451  
**Function Name:** `sys_resmon`  
**Architecture:** ARM64 (aarch64)

### Structure Definition
```c
struct resource_info {
    unsigned long total_ram;      // Total RAM in KB
    unsigned long free_ram;       // Free RAM in KB
    unsigned long used_ram;       // Used RAM in KB
    unsigned long total_swap;     // Total swap in KB
    unsigned long free_swap;      // Free swap in KB
    unsigned long procs;          // Process count
    unsigned long uptime;         // Uptime in seconds
    unsigned long load_1;         // 1-min load * 65536
    unsigned long load_5;         // 5-min load * 65536
    unsigned long load_15;        // 15-min load * 65536
};
```

## Documentation

- [Project Report](docs/PROJECT_REPORT.md)
- [Installation Guide](docs/INSTALLATION.md)

## Testing
```bash
# Compile debug version
cd user-app
make resmon_debug

# Run test
./resmon_debug
```

## Troubleshooting

### Kernel not booting
- Check GRUB menu and manually select 6.1.60
- Review compilation errors in build log

### System call not found
```bash
# Verify kernel version
uname -r

# Check syscall registration
cat /proc/kallsyms | grep sys_resmon
```

### Permission denied on log
```bash
sudo touch /var/log/resmon.log
sudo chmod 666 /var/log/resmon.log
```
