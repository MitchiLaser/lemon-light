// ===== configuration =====


// baud rate for serial connection
#define serial_baud 115200

// pins attached to the shift registers
#define shift_data 12
#define shift_clock 11
#define store_clock 10


const byte len_buffer = 3;

// ===== configuration end =====

// states for the state machine driving the whole device
enum State {S_INIT = 0, S_IDLE = 1, S_FIRST = 2, S_RECEIVE = 3, S_OUTPUT = 4, S_PASS = 5};
State sm_state; // store the current state

// output buffer
byte buffer[ len_buffer ] = {0};
byte buffer_pointer;

unsigned long int timer = 0;
unsigned long int timeout = 20; // in milliseconds
unsigned long int current_time;


// flush buffer to shift registers
void out_buffer( void ) {

  digitalWrite(store_clock, LOW);
  for (int i = len_buffer - 1; i >= 0; i--) {
    shiftOut(shift_data, shift_clock, MSBFIRST, buffer[i] );
  }
  digitalWrite(store_clock, HIGH);

}


void setup() {
  // initialiation
  sm_state = S_INIT;

  // initialize serial interface
  Serial.begin(serial_baud);

  // intialize GPIO Pins to drive shift register
  pinMode(shift_data, OUTPUT);
  pinMode(shift_clock, OUTPUT);
  pinMode(store_clock, OUTPUT);

  // set buffer pointer to 0
  buffer_pointer = 0;

  // output 0x000000 to reset all shift registers
  out_buffer();

  // next state
  sm_state = S_IDLE;
}


void loop() {
  
  switch (sm_state) {
    case S_IDLE:
      if ( Serial.available() )
        sm_state = S_FIRST;
      break;

    case S_FIRST:
      buffer_pointer = 0;
      buffer[ buffer_pointer ] = Serial.read();
      buffer_pointer++;
      timer = millis();
      sm_state = S_RECEIVE;
      break;

    case S_RECEIVE:
      current_time = millis();

      if (Serial.available()) {
        buffer[ buffer_pointer ] = Serial.read();
        buffer_pointer++;
        timer = millis();
        if (buffer_pointer < len_buffer) {
          sm_state = S_RECEIVE;
        } else {
          sm_state = S_OUTPUT;
        }
      } else {
        if ( current_time > timeout + timer) {
          sm_state = S_IDLE;
        }
      }
      break;

    case S_OUTPUT:
      out_buffer();
      timer = millis();
      sm_state = S_PASS;
      break;

    case S_PASS:
      current_time = millis();

      if ( Serial.available() ) {
        byte received = Serial.read();
        Serial.write( received );
        timer = millis();
      } else {
        if ( current_time > timeout + timer) {
          sm_state = S_IDLE;
        }
      }
      break;

  }

}
