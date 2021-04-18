#include "config.h"
#include "Arduino.h"

#include "timer.h"

Timer::Timer(unsigned long Delay) {
  this->reset();
  delay = Delay;
}


Timer::~Timer() { // it seems like theres no special destructor needed

}


void Timer::reset() { // reset Timer to start counting from the beginning
  lastUpdate = millis();
}


bool Timer::expired() { // chech weather the timer has been waiting for long enough or not
  return ( (millis() - lastUpdate) > delay ) ? true : false;
}
