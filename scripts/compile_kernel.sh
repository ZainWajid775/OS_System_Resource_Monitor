#!/bin/bash
# Compile and install kernel

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <kernel-source-directory>"
    exit 1
fi

KERNEL_DIR=$1
cd "$KERNEL_DIR"

# Copy current config
cp /boot/config-$(uname -r) .config

# Update config
yes "" | make oldconfig

# Fix certificate issue
sed -i 's/CONFIG_SYSTEM_TRUSTED_KEYS=".*"/CONFIG_SYSTEM_TRUSTED_KEYS=""/' .config
sed -i 's/CONFIG_SYSTEM_REVOCATION_KEYS=".*"/CONFIG_SYSTEM_REVOCATION_KEYS=""/' .config

# Compile
echo "Starting kernel compilation (this will take 1-3 hours)..."
make -j$(nproc)

# Install
echo "Installing kernel modules..."
sudo make modules_install

echo "Installing kernel..."
sudo make install

echo "Updating GRUB..."
sudo update-grub

echo "Kernel compilation complete!"
echo "Please reboot and select kernel 6.1.60 from GRUB menu."
