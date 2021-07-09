# HSIAP Proof of Concept

Hobbits is an open source bitwise analyzer and manipulation tool and can be found here: [Hobbits](https://github.com/Mahlet-Inc/hobbits)

The end goal for me is to build multiple pluggins for importing serial and USB data from IO and analyzing the data.

This code is proof of concept code, it is not final production code for the hobbits pluggins. 

## Usage:

The Serial Reading program can read the data comming off of a specific, active serial port for a user selected timeout.

The USB reading code can read the data comming off a user selected endpoint for a user selected timeout durration. 

### **Disclaimer: This code is only for proof of concept for reading from Serial and USB ports. The code is not very well documented, and probably wont be documented in its current state. The documnentation will be written with the final repository for the plugins.**

## Dependencies

The Serial Reader python program `serialRead.py` requires the dependency _pySerial_ to function

PySerial can be installed by using pip:

    pip install pyserial

The USB reader python code `usbRead.py` and the C++ code `usbRead(old).cpp` are depreciated, for USB reading please use `usbRead.cpp`.

For `usbRead.py` you must have libusb-1.0 installed, it comes pre-installed on most linux distributions. If you don't already have libusb installed, you can install your distributions package manager. For apt systems you can use:

    sudo apt-get install libusb-1.0.0-dev

For other Linux Distributions and Windows Systems please refer to the [libusb website.](libusb.info)

## Building and Running C/C++ Programs with Libusb From Source

If writing a new program you must import libusb using:

    #include <libusb-1.0/libusb.h>

And to compile a program containing libusb you must use the libusb compiler flag:

For usbRead.cpp the compile command is as follows:

     g++ usbRead.cpp -o build/main -lusb-1.0

And on linux it can be run by using:

    ./build/main

