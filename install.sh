#!/usr/bin/env bash

# Install first part of SwitchProCon
sudo cp 99-hid-procon.rules /etc/udev/rules.d/99-hid-procon.rules

# Build SwitchProCon
mkdir build
cd build
cmake ..
make