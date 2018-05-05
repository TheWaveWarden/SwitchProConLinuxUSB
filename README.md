# SwitchProConLinuxUSB
This repository aims to provide a uinput driver for the Nintendo Switch Pro Controller when connected via USB.
Currently only one controller is supported!

# Dependencies

This repo needs 

- libudev

- autotools, autoconf and libtool

- cmake

On Ubuntu you can install these in a terminal with

```
sudo apt-get install libudev-dev libusb-1.0-0-dev libfox-1.6-dev
sudo apt-get install autotools-dev autoconf automake libtool
sudo apt-get install cmake
```
Further we need

- hidapi

Compile and install it from source in its own folder by using:

```
mkdir hidapi
cd hidapi
git clone git://github.com/signal11/hidapi.git .
./bootstrap
./configure
make
sudo make install
```

You can delete the hidapi folder now:

```
cd ..
rm -rf hidapi
```

# Installation


Create install folder for Pro Controller driver and enter it, e.g.
```
mkdir ~/procon_driver
cd ~/procon_driver
```
Clone the repository here
```
git clone https://github.com/FrotBot/SwitchProConLinuxUSB.git .
```
install and build the driver
```
bash install.sh
```

reboot your PC once to make the udev rules work

Open terminal once more and navigate to the build directory in the install folder
```
cd ~/procon_driver/build
```

Start the driver!
```
./procon_driver
```

Follow instructions on screen and enjoy your games.

(You'll need to reopen the executable from the last step everytime you use the driver.)

On newer kernel versions, uinput devices need root privileges, so if you get error messages try to run
```
sudo ./procon_driver
```


# Thanks
This project took heavy inspiration and some constants from this project:
https://github.com/MTCKC/ProconXInput/tree/v0.1.0-alpha2



