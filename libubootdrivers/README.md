# libubootdrivers

The libubootdrivers library provides a framework for utilising U-Boot device drivers under seL4. The library provides the neccessary stubs, configuration data and supporting code to allow the use of these drivers with minimal or no modification, as well as an API for interacting with the drivers and essential library functions. The library includes example usage for an extensive set of drivers for the Avnet MaaXBoard, inlcuding USB, Ethernet, MMC, I2C, SPI, GPIO and Clock drivers.

## Library directory strucuture

The root of the library contains 3 folders and one as follows:

- include - this folder contains platform specific configuration data (examples provided for the Avnet MaaXBoard and Odroid C2) for the drivers, the head file for public API provided by the library, aswell as a number of header files for wrapper around the U-Boot source code.

- src - this folder contains the source code for the libraries wrapper around U-Boot as well as its API, as well as supporting code for timer drivers and additional platform speciifc configuration data.

- uboot_stub - this folder provides stubs for various U-Boot source code files which now have functions provided by the seL4 kernel, including the console, logging, random number generation and enviroment varibales.

