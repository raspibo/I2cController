#include "I2cControllerLib.h"


void I2cController::begin(uint8_t addr) {
  Adafruit_MCP23017 mcp;
  mcp.begin(addr); 
  //DigitalPot pot;     ipotetico begin del potenziometro digitale
  //pot.begin(addr+x); 
}

void I2cController::pinMode(uint8_t p, uint8_t d) {
  Adafruit_MCP23017 mcp;
  mcp.pinMode(p,d);  
}

void I2cController::digitalWrite(uint8_t pin, uint8_t d) {
  Adafruit_MCP23017 mcp;
  mcp.digitalWrite(pin,d);
}

uint8_t I2cController::digitalRead(uint8_t pin) {
  Adafruit_MCP23017 mcp;
  return mcp.digitalRead(pin);
}

void I2cController::pullUp(uint8_t p, uint8_t d) {
  Adafruit_MCP23017 mcp;
  mcp.pullUp(p, d);
}

void I2cController::setupInterrupts(uint8_t mirroring, uint8_t openDrain, uint8_t polarity){
  Adafruit_MCP23017 mcp;
  mcp.setupInterrupts(mirroring, openDrain, polarity);
}

void I2cController::setupInterruptPin(uint8_t pin, uint8_t mode) {
  Adafruit_MCP23017 mcp;
  mcp.setupInterruptPin(pin, mode);
}

void I2cController::setupEncoder(uint8_t pinL, uint8_t pinR, uint8_t pinBut)  {
  Adafruit_MCP23017 mcp;
  //Define Encoder rotation pin
  mcp.pinMode(pinL,INPUT);
  mcp.pullUp(pinL, HIGH);
  mcp.setupInterruptPin(pinL, CHANGE);
  mcp.pinMode(pinR,INPUT);
  mcp.pullUp(pinR, HIGH);
  mcp.setupInterruptPin(pinR, CHANGE);
  //Define button pin
  mcp.pinMode(pinBut,INPUT);
  mcp.pullUp(pinBut, HIGH);
  mcp.setupInterruptPin(pinBut, CHANGE);
  
  mcp.setupInterrupts(true, false, LOW);
}

uint8_t I2cController::getLastInterruptPin(){
  Adafruit_MCP23017 mcp;
  //Define button pin
  return mcp.getLastInterruptPin();
}

uint8_t I2cController::getLastInterruptPinValue(){
  Adafruit_MCP23017 mcp;
  //Define button pin
  return mcp.getLastInterruptPinValue();
}

