#include <Adafruit_MCP23017.h>
#include "I2cControllerLib.h"

I2cController contr;

// Interrupts from the MCP will be handled by this PIN on Arduino
byte arduinoIntPin = 3;
// ... and this interrupt vector
byte arduinoInterrupt = 1;

bool InterruptTriggered = false;

void intCallBack() {
  InterruptTriggered = true;
  Serial.println("Interrupt");
}

void cleanInterrupts() {
  EIFR = 0x01;
  InterruptTriggered = false;
}

void handleInterrupt() {
  // disable interrupts while handling them.
  detachInterrupt(arduinoInterrupt);

  contr.getLastInterruptPin();
  Serial.println(contr.getLastInterruptPin());
  contr.getLastInterruptPinValue();
  
  cleanInterrupts();
  //we set callback for the arduino INT handler.
  attachInterrupt(arduinoInterrupt, intCallBack, FALLING);
}  

void setup() {
  Serial.begin(115200);
  Serial.println("---- Startup ----------");
  contr.begin(0);
  for (int count = 0; count < 16; count++) {
    contr.pinMode(count, OUTPUT);
  }
  contr.setupEncoder(0,1,2);
  pinMode(arduinoIntPin, INPUT);
  attachInterrupt(arduinoInterrupt, intCallBack, FALLING);
}

void loop() {
  if (InterruptTriggered) handleInterrupt();
  delay(100);
}
