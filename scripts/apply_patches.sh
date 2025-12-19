#!/bin/bash
# Apply kernel patches

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <kernel-source-directory>"
    echo "Example: $0 ~/linux-6.1.60"
    exit 1
fi

KERNEL_DIR=$1
PATCH_DIR="$(dirname "$0")/../kernel-patches"

echo "Applying patches to $KERNEL_DIR..."

# Create custom syscall directory
mkdir -p "$KERNEL_DIR/kernel/custom_syscalls"

# Copy system call file
cp "$PATCH_DIR/sys_resmon.c" "$KERNEL_DIR/kernel/custom_syscalls/"

# Apply patches
cd "$KERNEL_DIR"
patch -p1 < "$PATCH_DIR/Makefile.patch"
patch -p1 < "$PATCH_DIR/syscalls.h.patch"
patch -p1 < "$PATCH_DIR/unistd.h.patch"

echo "Patches applied successfully!"
