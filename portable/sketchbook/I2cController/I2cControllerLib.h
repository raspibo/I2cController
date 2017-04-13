#include <stdint.h>
#include "Adafruit_MCP23017.h" // https://github.com/adafruit/Adafruit-MCP23017-Arduino-Library
#include "LiquidCrystal_I2C.h"
class I2cController {
  public:
    void begin(uint8_t addr);
    void begin(void);

    void pinMode(uint8_t p, uint8_t d);
    void digitalWrite(uint8_t p, uint8_t d);
    void pullUp(uint8_t p, uint8_t d);
    uint8_t digitalRead(uint8_t p);

    void setupInterrupts(uint8_t mirroring, uint8_t openDrain, uint8_t polarity);
    void setupInterruptPin(uint8_t pin, uint8_t mode);
    uint8_t getLastInterruptPin();
    uint8_t getLastInterruptPinValue();
    
    
    
    void setupEncoder(uint8_t pinL, uint8_t pinR, uint8_t pinBut);
    


  private:
    uint8_t i2caddr;

    uint8_t bitForPin(uint8_t pin);
    uint8_t regForPin(uint8_t pin, uint8_t portAaddr, uint8_t portBaddr);

    uint8_t readRegister(uint8_t addr);
    void writeRegister(uint8_t addr, uint8_t value);

    /**
       Utility private method to update a register associated with a pin (whether port A/B)
       reads its value, updates the particular bit, and writes its value.
    */
    void updateRegisterBit(uint8_t p, uint8_t pValue, uint8_t portAaddr, uint8_t portBaddr);

};

