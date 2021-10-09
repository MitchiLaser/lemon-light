# lemon-light
a couple of people trying to build a fancy monochrome LED display with bottles

## used bottles
We used lemonade bottles with 500 ml infill. The drinks boxes we bougth them inside had space for 26 bottles (4 x 6) and had a surface of 30cm x 40cm. Every box has its own controller which receives data via UART and drives the LEDs. The controllers can be connected in a dasy chain configuration.

## controllerboards

### slave

Every drink box has its own slave controller which is nothing more then an Attiny85 and 3 74hc595 shift registers. The Attiny receives the data via a UART interface, gets the first 3 bytes ans sends the rest to the next controller (also UART). Furthermore there is also an lm7805 attached because all boxes are powered via a voltage higher then 7,5V. This decision was made to prevent a voltage drop.

### master

The whole display is being controlled by a single master-controller which is made out of an ESP8266 and some electronics to translate the voltage for the serial interface between 3,3V and 5V. It is connected to a WiFi-interface and provides a web interface to control the boxes.

## 3D objects

There are some 3d printed objects that were used in the original build of this project: A part to hold the leds in the bottles and a box for the slave controller.

