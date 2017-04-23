/*
   From https://github.com/lincomatic/I2cControllerLib
   and http://blog.think3dprint3d.com/2012/12/mcp23017-i2c.html
 */

// include the library code:
#include <Wire.h>
#include <I2cController.h>

#define EN_A 0 // Encored scroll A0
#define EN_B 1 // Encoder scroll A1 
#define EN_C 2 // Encoder click  A2
#define BTN_NUM 5
#define BTN_1 3 // Single button A3
#define BTN_2 4 // Single button A4
#define BTN_3 5 // Single button A5
#define BTN_4 6 // Single button A6
#define BTN_5 7 // Single button A7

// Connect via i2c, default address #0 (A0-A2 not jumpered)
I2cControllerLib contr(0);
int encoderPos=0;
uint8_t clickButton;

uint8_t Pot;
char ActEnc, OldEnc;


// Interrupts from the MCP will be handled by this PIN on Arduino
byte arduinoIntPin = 3;
// ... and this interrupt vector
byte arduinoInterrupt = 1;

volatile boolean awakenByInterrupt = false;
unsigned long lastMillisInterrupt = 0;

void handleInterrupt() {

	detachInterrupt(arduinoInterrupt);
	if (millis()>lastMillisInterrupt+25) {
		//A    |¯¯|__|¯¯|__|¯
		//B  - __|¯¯|__|¯¯|__  +
		// Nel canale A pin D3 condensatore ceramico 103K 10nF
		uint8_t ActEnc=0;
		uint8_t intPin=contr.getLastInterruptPin();
		uint8_t valPin=contr.getLastInterruptPinValue();
		//Serial.print("intPin: ");
		//Serial.println(intPin);
		//Switch che  decide come gestire interrupt in base al pin che lo ha generato se A0 o A1 interpreta encoder altrimenti interpretazione come pulsante
		switch(intPin) {
			case EN_A:
			case EN_B:
				bitWrite(ActEnc, 0,contr.digitalRead(EN_A));
				bitWrite(ActEnc, 1,contr.digitalRead(EN_B));
				switch(ActEnc) {
					case 0: //00
						if(OldEnc==2) Pot++;  //10
						if(OldEnc==1) Pot--;  //01
						break;
					case 1: //01
						if(OldEnc==0) Pot++;  //00
						if(OldEnc==3) Pot--;  //11
						break;
					case 2: //10
						if(OldEnc==3) Pot++;  //11
						if(OldEnc==0) Pot--;  //00
						break;
					case 3: // 11
						if(OldEnc==1) Pot++;  //01
						if(OldEnc==2) Pot--;  //10
						break;
				}
				OldEnc=ActEnc;
				Serial.print("Pot: ");
				Serial.println(Pot);
				break;
			case EN_C:
				Serial.println("click");
				break;
			default:
				Serial.print("Button: A");
				Serial.println(intPin);
				break;
		}
	}



	lastMillisInterrupt=millis();
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
	contr.setMCPType(LTI_TYPE_MCP23017); 
	// set up the LCD's number of rows and columns:
	contr.begin(16, 2);
	// Print a message to the LCD.
	contr.print("hello, world!");
	for (int count = 0; count < 16; count++) {
		contr.pinMode(count, OUTPUT);
	}
	contr.setupEncoder(EN_A,EN_B,EN_C); // Encoder setup 
//	contr.setIntBtn(BTN_1);                 // Single button setup
	//contr.setupInterruptPin(BTN_1, FALLING); //Use this if you want receive continuos interrupt on button pressed. Useful ??
        uint8_t btnCross[BTN_NUM] = { BTN_1, BTN_2, BTN_3, BTN_4, BTN_5}; // Five button on cross disposition, setup function
        contr.setIntCross(btnCross, BTN_NUM);
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

