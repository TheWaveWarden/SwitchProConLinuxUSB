#!/usr/bin/env bash

# Build SwitchProCon
mkdir build
cd build
cmake ..
make
cd ..

#copy udev rules
sudo cp 71-nintendo-switch-procon-usb.rules /etc/udev/rules.d/71-nintendo-switch-procon-usb.rules

