# SwitchProConLinuxUSB
This repository aims to provide a uinput driver for the Nintendo Switch Pro Controller when connected via USB.
Currently only one controller is supported!

# Dependencies

This repo needs 

- libudev
- autotools, autoconf and libtool
- cmake
- hidapi

On Ubuntu you can install these in a terminal with:
```
sudo apt-get install libudev-dev libusb-1.0-0-dev libfox-1.6-dev
sudo apt-get install autotools-dev autoconf automake libtool
sudo apt-get install cmake
sudo apt-get install libhidapi-dev
```
# Installation

Create install folder for Pro Controller driver and enter it, e.g.
```
mkdir ~/procon_driver
cd ~/procon_driver
```
You can download the ZIP file through your browser and extract it, or you can use git. If you don't already have it:
```
sudo apt install git
```
Clone the repository here:
```
git clone https://github.com/FrotBot/SwitchProConLinuxUSB.git .
```
install and build the driver:
```
bash install.sh
```

Reboot your PC once to make the udev rules work.

Open the terminal once more and navigate to the build directory in the install folder:
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

# Inverted axes and swap buttons 

If you're having trouble with inverted axes, try ./procon_driver --help and see options to run with inverted axes there.
There's also an option to run with A and B as well as X and Y buttons switched, if you prefer the button output as they're written on the pad as opposed to XBox layout.


# Thanks

This project took heavy inspiration and some constants from this project:
https://github.com/MTCKC/ProconXInput/tree/v0.1.0-alpha2





