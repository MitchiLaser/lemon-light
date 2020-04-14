// Baud-Rate, mit der die Getränkekästen untereinander komminizieren
#define SERIAL_BAUD 500000

// Globaler Wert für das Zeit intervall zwischen den Packeten in Millisekunden
#define PackageDelay 500

// Pins, über die die Daten an die Shift-Register weitergegeben werden
#define DataPin  11
#define ClockPin 10
#define StorePin 9



// Buffer für die Daten, die hereinkommen
byte buffer[3] = {0};

// globale Varialble, in der mitgezählt wird, wie viele Bytes aus aktuellen letzten Packet angekommen sind
byte bitCounter = 0;

// timer-basierte Überwachung der hereinkommenden Packeten
unsigned long lastPackageReceived = 0;



void setup()
{
  Serial.begin(SERIAL_BAUD);
}


void loop()
{
  // falls Daten über die serielle Schnittstelle angekommen sind, werden diese verarbeitet
  if (Serial.available())
  {
    for (byte i = 0; i < Serial.available(); i++)
    {
      // byte receivedByte = Serial.read();
      if (bitCounter < 3)
      {
        buffer[bitCounter] = Serial.read(); // = receivedByte
        bitCounter++;
      }
      else
      {
        Serial.write( Serial.read() ); // receivedByte
      }
    }
    lastPackageReceived = micros();
  }

  // Falls nach einer Weile keine Daten mehr angekomen sind, werden die erhaltenen Daten asugegeben
  if (micros() - lastPackageReceived > PackageDelay && bitCounter > 0)
  {
    bitCounter = 0;
    
    // Herausgabe der erhaltenen Daten an die Shift Register
    digitalWrite(StorePin, LOW);
    for(byte i = 0; i < sizeof(buffer); i++)
    {
      shiftOut(DataPin, ClockPin, MSBFIRST, buffer[i]);
    }
    digitalWrite(StorePin, HIGH);
  }
}
