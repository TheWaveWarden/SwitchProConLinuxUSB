#!/usr/bin/env bash

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