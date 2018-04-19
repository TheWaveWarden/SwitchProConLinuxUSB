#!/usr/bin/env bash

# Install dependencies
sudo apt-get install libudev-dev libusb-1.0-0-dev libfox-1.6-dev
sudo apt-get install autotools-dev autoconf automake libtool

# Install first part of SwitchProCon
sudo cp 99-hid.rules /etc/udev/rules.d/99-hid-procon.rules

# Compile hidapi
mkdir hidapi
cd hidapi
git clone git://github.com/signal11/hidapi.git .
./bootstrap
./configure
make
sudo make install
cd ..
rm -rf hidapi

# Build SwitchProCon
mkdir build
cd build
cmake ..
make