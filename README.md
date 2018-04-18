# SwitchProConLinuxUSB
This repository aims to provide a uinput driver for the Nintendo Switch Pro Controller when connected via USB.
Currently only one controller is supported!

# Installation

1. Open a terminal (ctrl + alt + T)
2. Create install folder and enter it, e.g.
```
mkdir ~/procon_driver
cd ~/procon_driver
```
3. Clone the repository here
```
git clone https://github.com/FrotBot/SwitchProConLinuxUSB.git
```
4. install and build the driver
```
bash install
```
(You'll be prompted to type your password and press y a few times, it installs some libraries.)
5. reboot your PC once to make the udev rules work
6. Open terminal once more and navigate to the build directory in the install folder
```
cd ~/procon_driver/build
```
7. Follow instructions on screen and enjoy your games.
(Repeat 6. and 7. every time you want to open the driver)


# Thanks
This project took heavy inspiration and some constants from this project:
https://github.com/MTCKC/ProconXInput/tree/v0.1.0-alpha2

This project uses the hidapi library!


