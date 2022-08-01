# libubootdrivers

The libubootdrivers library provides a framework for utilising U-Boot device drivers under seL4. The library provides the neccessary stubs, configuration data and supporting code to allow the use of these drivers with minimal or no modification, as well as an API for interacting with the drivers and essential library functions. The library includes example usage for an extensive set of drivers for the Avnet MaaXBoard, inlcuding USB, Ethernet, MMC, I2C, SPI, GPIO and Clock drivers.

## Library directory strucuture

The root of the library contains 4 folders and one file as follows:

- include - this folder contains platform specific configuration data (examples provided for the Avnet MaaXBoard and Odroid C2) for the drivers, the head file for public API provided by the library, aswell as a number of header files for wrapper around the U-Boot source code.

- src - this folder contains the source code for the libraries wrapper around U-Boot as well as its API, as well as supporting code for timer drivers and additional platform speciifc configuration data.

- uboot_stub - this folder provides stubs for various U-Boot source code files which now have functions provided by the seL4 kernel, including the console, logging, random number generation and enviroment varibales.

- uboot - this folder contains the entire U-Boot source, cloned from the sel4devkit/uboot repository on the libubootdrivers branch. The U-Boot source is brought in to provide the driver code as well as code for U-Boot commands, which can be executed via a interface in the libraries API. **NOTE: This folder is only added to the libubootdrivers when cloned using an appropiate manifest (e.g sel4devkit/camkes_manifest) with the repo tool.**

- CMakeLists.txt - this file contains much of the base configuration data for the library, including definitions for archetecture and platform speciific configuration variales, and settings for the default logging level.

## Platform specific configuration

Two main files are used to provide speicific configuration data for each platform, a C source file and associated header file. These are stored under src/plat/platform_name and include/plat/platform name respectively. These files contain information about what drivers and commands should be made availible for a platform, based on the driver/command string names from the U-Boot source. They also provide a list of enviroment varibales that should be made availible. Examples exist in the library for the Avnet MaaXBoard and the Odroid C2.

