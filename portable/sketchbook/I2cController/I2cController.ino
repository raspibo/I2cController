/*
   From https://github.com/lincomatic/LiquidTWI2
   and http://blog.think3dprint3d.com/2012/12/mcp23017-i2c.html
 */

// include the library code:
#include <Wire.h>
#include <LiquidTWI2.h>


// Connect via i2c, default address #0 (A0-A2 not jumpered)
LiquidTWI2 contr(0);
int encoderPos=0;
uint8_t clickButton;



// Interrupts from the MCP will be handled by this PIN on Arduino
byte arduinoIntPin = 3;
// ... and this interrupt vector
byte arduinoInterrupt = 1;

volatile boolean awakenByInterrupt = false;


void handleInterrupt() {
/*
	Serial.print("INTCAPA: ");
	Serial.println(contr.readRegister(MCP23017_INTCAPA),BIN);
	Serial.println("Int");
	//reset the flag

	//rotating=false;   

	//get the interrupt status of the pins
	//this will clear the flag allowing further interrupts
	uint8_t intCap=contr.readRegister(MCP23017_INTCAPA);
	uint8_t test = intCap & 0b00000111;
	//only deal with the situation where the encoder is turned without the
	//click button held down
	if (test == 0b101) encoderPos += 1;
	if (test == 0b110) encoderPos -= 1;
	//mask out the encoder bits and look at the click button
	test &= 0b100;
	if (test == 0b000) clickButton = LOW;

	// while interrupts are still occurring clear the register (switch bouncing) 
	while(digitalRead(arduinoIntPin) > 0)
	{
		contr.readRegister(MCP23017_INTCAPA);
		delay (10);
	}
	cleanInterrupts();
*/
  // disable interrupts while handling them.
  detachInterrupt(arduinoInterrupt);

  contr.getLastInterruptPin();
  Serial.println(contr.getLastInterruptPin());
  contr.getLastInterruptPinValue();
  
  cleanInterrupts();
  //we set callback for the arduino INT handler.
  attachInterrupt(arduinoInterrupt, intCallBack, FALLING);
}


void intCallBack() {
	awakenByInterrupt = true;
}

void cleanInterrupts() {
	EIFR = 0x01;
	awakenByInterrupt = false;
}

void setup() {
	Serial.begin(115200);
	Serial.println("Startup");
	// set the LCD type
	contr.setMCPType(LTI_TYPE_MCP23017); 
	// set up the LCD's number of rows and columns:
	contr.begin(16, 2);
	// Print a message to the LCD.
	contr.print("hello, world!");
	/*

	   contr.setRegister(MCP23017_DEFVALA,0x07); //set the default values as 1
	//Serial.println(contr.readRegister(MCP23017_DEFVALA),BIN);
	//set to compare with register values set to 1 with DEFVALB 
	contr.setRegister(MCP23017_INTCONA,0x07);
	contr.setRegister(MCP23017_IOCONA,0x02); //enable active high for interrupt pin

	contr.setRegister(MCP23017_GPINTENA,0x07); //enable interrupts on 3 pins
	//read the interrupt capture register to reset it
	uint8_t reg = contr.readRegister(MCP23017_INTCAPA);
	Serial.print("INTCAPA: ");
	Serial.println(reg,BIN);
	//Arduino interrupt pin setup
	pinMode(arduinoIntPin, INPUT); //set arduinoIntPin as input
	//attach an interrupt using the pinChangeInt library to interruptPin
	//PCintPort::attachInterrupt(interruptPin, &quickInt, RISING); 
	 */
        for (int count = 0; count < 16; count++) {
            contr.pinMode(count, OUTPUT);
        }
	contr.setupEncoder(1,2,0);
	contr.setupInterruptPin(0, CHANGE);
	attachInterrupt(arduinoInterrupt, intCallBack, FALLING);

}

void loop() {
	// set the cursor to column 0, line 1
	// (note: line 1 is the second row, since counting begins with 0):
	contr.setCursor(0, 1);
	// print the number of seconds since reset:
	contr.print(millis()/1000);

	//contr.setBacklight(HIGH);
	//delay(500);
	//contr.setBacklight(LOW);
	//delay(500);

	if (awakenByInterrupt) handleInterrupt();
	delay(30);
}

