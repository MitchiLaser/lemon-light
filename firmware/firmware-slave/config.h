#ifndef __configh__
#define __configh__


// Baudrate for the serial connection
#define SERIAL_BAUD 19200


// Pins, chosen for the UART interface
#define SERIAL_RX 0
#define SERIAL_TX 1


// Pins, connected to the shift registers
#define LED_DATA  4
#define LED_CLOCK 3
#define LED_STORE 2


// The amount of bytes which contain the information of the LEDs, driven by one single controller
#define LEN_SIGNAL 3


// The timeout for a stream which sends all the data to the controllers. Values in milliseconds
#define StreamTimeout 2


#endif
