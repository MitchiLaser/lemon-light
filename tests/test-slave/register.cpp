#include "config.h"
#include "Arduino.h"

#include "register.h"

// A simple interface for the Shift-Registers
// Just because outsorcing this interface makes the main program more readable

Register::Register() {
  // drive the pins, attached to the shift-registers
  pinMode(LED_DATA , OUTPUT);
  pinMode(LED_CLOCK, OUTPUT);
  pinMode(LED_STORE, OUTPUT);

  digitalWrite(LED_DATA , LOW);
  digitalWrite(LED_CLOCK, LOW);
  digitalWrite(LED_STORE, LOW);

  // clear all values in the buffer
  for (int i = 0; i < LEN_SIGNAL; i++) {
    Buffer[i] = (unsigned char)0;
  }
}


Register::~Register() {
  // turn pins off because they arn't in use any more
  digitalWrite(LED_DATA , LOW);
  digitalWrite(LED_CLOCK, LOW);
  digitalWrite(LED_STORE, LOW);
}


void Register::add( unsigned char Data ) {
  // move all values in buffer one step further
  for (int i = (LEN_SIGNAL - 1); i > 0; i--) {
    Buffer[i] = Buffer[i - 1];
  }
  // add new Value
  Buffer[0] = Data;
}


void Register::send( void ) {
  digitalWrite(LED_STORE, LOW);

  // send all data in the buffer
  for (int i = 0; i < LEN_SIGNAL; i++) {
    shiftOut(LED_DATA, LED_CLOCK, MSBFIRST, Buffer[i]);
  }

  digitalWrite(LED_STORE, HIGH);
}
