#!/bin/bash
# Install prerequisites for kernel compilation

echo "Installing prerequisites..."
sudo apt update
sudo apt install -y build-essential libncurses-dev bison flex \
                    libssl-dev libelf-dev bc dwarves git fakeroot

echo "Prerequisites installed successfully!"
echo "Available disk space:"
df -h /
