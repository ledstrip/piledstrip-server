# piledstrip-server
piledstrip-server is the link between low performance hardware (e.g. RaspberryPi Zero) controlling WS2812b LED strips and high performance hardware (e.g. your laptop).
piledstrip-server opens UDP / TCP servers allowing you to stream cool effects from your computer (e.g. using [pyledstrip](https://github.com/ledstrip/pyledstrip)).

# How to use?
Execution of piledstrip-server is fairly easy. Simply execute
```bash
sudo ./piledstrip-server
```

Sudo is required as use of GPIOs, DMA etc. by default requires root permissions.

## Hardware / GPIOs
For details on which GPIOs are used in the RaspberryPi case please check out the [rpi-many-ws2812b](https://github.com/lukaslihotzki/rpi-many-ws2812b) library readme.

## Configuration
A JSON configuration file must be provided which
Here is an exemplary JSON configuration file:
```json
{
  "strips": [
    {
      "led_count": 300,
      "flip": true
    },
    {
      "led_count": 300,
      "flip": false
    }
  ],
  "servers": [
    {
      "protocol": "tcp",
      "port": "7777",
      "interpreter": "opc"
    },
    {
      "protocol": "udp",
      "port": "7777",
      "interpreter": "esp"
    }
  ]
}
```

This file configures two LED strips with length of 300 LEDs each. Please note the first LED strip is marked as flipped. This is important as piledstrip-server exposes multiple strips as one single long strip for easy interfacing. Using the flip option you can ensure proper concatenation.

Two servers are set up through which data streams are accepted. TCP servers can handle data for strips with a total of ***insert large number here*** LEDs as TCP handles the proper grouping of multiple splitted network packets for you. With UDP you are limited to ***insert not so large number here*** LEDs as the maximum payload for UDP packages is 65507 bytes.
Currently two protocols are implemented: OPC and ESP. OPC stands for [OpenPixelControl](http://openpixelcontrol.org/). The ESP protocol is an imitation of the interpreter implemented in the firmware [esp8266ws2812i2s](https://github.com/cnlohr/esp8266ws2812i2s).

# Dependencies
For the RaspberryPi the library [rpi-many-ws2812b](https://github.com/lukaslihotzki/rpi-many-ws2812b) is used which allows high performance control of multiple LED strips from a single Pi. The library is set up as a submodule in this repository. 

Additional dependencies: boost.

# Build
This is a C++ project based on CMake using modern C++ features (at least C++11). The simplest (and slowest) method is to check out the code on the RaspberryPi, install the dependencies and then follow these steps:

```bash
cd piledstrip-server
cmake .
make
```

# Compatibility
In the current state the project is configured to work with a RaspberryPi but extension to other platforms is trivially possible. Simply copy the DemoInterface.cpp/.h and integrate the GPIO controlling library.
If you add other hardware support pull requests are welcome!

# License
This project is published under the MIT license
