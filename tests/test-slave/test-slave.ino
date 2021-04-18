#include "config.h" // Configuration
#include "buffer.h" // Implementation of the Buffer
#include "register.h" // Interface for the Shift-Register
#include "timer.h" // Better interface for Timers based on millis()-function calls
#include <SoftwareSerial.h> // Serial Connection



// Declare an instance of the serial interface
SoftwareSerial serial(SERIAL_RX, SERIAL_TX);

// Declare an instance of the buffer
Buffer *Temp = new Buffer();

// Declare an instance of the interface for the Shift-Register
Register *Output = new Register();

// Declare instances of the timers
Timer StreamTimer(StreamTimeout);
int delayTime = 1000;


void setup() {

  /*

  // initialise serial connection
  serial.begin(SERIAL_BAUD);
  serial.listen();


  // reset all Timers
  StreamTimer.reset();

  */
}



void loop() {

  /*

  // some data seems to be available. put it into the buffer
  if ( serial.available() ) {

    // for better performance: first put all received values into the buffer and reset the timer after that
    while ( serial.available() ) {
      Temp->push( serial.read() );
    }

    // reset the timer for the timeout of incomming signals
    StreamTimer.reset();
  }


  // when there is data in the buffer and the timer has expired:
  // send the configuration to the shift registers and the rest via software serial to the next controller
  if ( !Temp->empty() && StreamTimer.expired() ) {

    // load first received bytes into the buffer for the shift-registers
    for (unsigned char i = 0; i < LEN_SIGNAL; i++) {
      Output->add( Temp->pop() );
    }

    // send the rest via software serial to the next controller
    while ( !Temp->empty() ) {
      serial.write( Temp->pop() );
    }

    // and now perform the output on the shift-registers
    Output->send();
  }

  */



 

  
  unsigned short Out_Byte = 1; // 0b00000001
  for(int i = 0; i < 8; i++){

    digitalWrite(LED_STORE, LOW);
    shiftOut(LED_DATA, LED_CLOCK, MSBFIRST, 0);
    shiftOut(LED_DATA, LED_CLOCK, MSBFIRST, 0);
    shiftOut(LED_DATA, LED_CLOCK, MSBFIRST, Out_Byte);
    digitalWrite(LED_STORE, HIGH);

    Out_Byte = Out_Byte<<1;

    delay(delayTime );
  }



  Out_Byte = 1; // 0b00000001
  for(int i = 0; i < 8; i++){

    digitalWrite(LED_STORE, LOW);
    shiftOut(LED_DATA, LED_CLOCK, MSBFIRST, 0);
    shiftOut(LED_DATA, LED_CLOCK, MSBFIRST, Out_Byte);
    shiftOut(LED_DATA, LED_CLOCK, MSBFIRST, 0);
    digitalWrite(LED_STORE, HIGH);

    Out_Byte = Out_Byte<<1;

    delay(delayTime );
  }



  Out_Byte = 1; // 0b00000001
  for(int i = 0; i < 8; i++){

    digitalWrite(LED_STORE, LOW);
    shiftOut(LED_DATA, LED_CLOCK, MSBFIRST, Out_Byte);
    shiftOut(LED_DATA, LED_CLOCK, MSBFIRST, 0);
    shiftOut(LED_DATA, LED_CLOCK, MSBFIRST, 0);
    digitalWrite(LED_STORE, HIGH);

    Out_Byte = Out_Byte<<1;

    delay(delayTime );
  }

  




  /*

  digitalWrite(LED_STORE, LOW);
  shiftOut(LED_DATA, LED_CLOCK, MSBFIRST, 0b11111111);
  shiftOut(LED_DATA, LED_CLOCK, MSBFIRST, 0);
  shiftOut(LED_DATA, LED_CLOCK, MSBFIRST, 0);
  digitalWrite(LED_STORE, HIGH);
  
  delay(1000);


  digitalWrite(LED_STORE, LOW);
  shiftOut(LED_DATA, LED_CLOCK, MSBFIRST, 0);
  shiftOut(LED_DATA, LED_CLOCK, MSBFIRST, 0b11111111);
  shiftOut(LED_DATA, LED_CLOCK, MSBFIRST, 0);
  digitalWrite(LED_STORE, HIGH);

 // delay(1000);


  digitalWrite(LED_STORE, LOW);
  shiftOut(LED_DATA, LED_CLOCK, MSBFIRST, 0);
  shiftOut(LED_DATA, LED_CLOCK, MSBFIRST, 0);
  shiftOut(LED_DATA, LED_CLOCK, MSBFIRST, 0b11111111);
  digitalWrite(LED_STORE, HIGH);

  delay(1000);
  */

  
  /*
  digitalWrite(LED_STORE, LOW);
  shiftOut(LED_DATA, LED_CLOCK, MSBFIRST, 0xff);
  shiftOut(LED_DATA, LED_CLOCK, MSBFIRST, 0xff);
  shiftOut(LED_DATA, LED_CLOCK, MSBFIRST, 0xff);
  digitalWrite(LED_STORE, HIGH);
  */
 }
