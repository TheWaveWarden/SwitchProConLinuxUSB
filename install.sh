#!/usr/bin/env bash

# Build SwitchProCon
mkdir build
cd build
cmake ..
make

#copy udev rules
sudo cp 99-hid-procon.rules /etc/udev/rules.d/99-hid-procon.rules
sudo cp 50-uinput.rules /etc/udev/rules.d/50-uinput.rules