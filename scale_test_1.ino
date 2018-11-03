/* Car Seat MONITOR SYSTEM
   Created by: Brian L. Decker
   COPYRIGHT (C) 2018 Encom Lab.
   All rights reserved.
*/

//library file for load cells
#include "HX711.h"

// sim900 function calls via softserial
#include <SoftwareSerial.h>

// softserial inputs - tx, rx
SoftwareSerial SIM900(8, 9);

//loadcell setup
#define calibration_factor 696.0 //Calibration value set during manufacture
#define DOUT  3
#define CLK  2
HX711 scale(DOUT, CLK);

//input pins
// sms send
const int smsSend_B = 12;    // SMS MESSAGE A TRIGGER
int tempState = 0;  //temp trigger
const int smsSimLED_B = 10;   // SMS A TRANSMITTING LED


// OUTPUT PINS
const int ledPin = 4;      // LED SENSOR RED
const int ledPin_G = 5;      // LED SENSOR green

// VARIABLE TO STORE THE TEXT MESSAGE
String textMessage;

//test led
void led_light() {

  if ((scale.get_units() > 10) &&(tempState==HIGH))  {
    digitalWrite(ledPin, HIGH);
    digitalWrite(ledPin_G, LOW);

    //send text
    sendSMS_B();

  }
  else if (scale.get_units() <= 10) {
    digitalWrite(ledPin, LOW);
    digitalWrite(ledPin_G, HIGH);
  }
}

//SMS SEND_B
void sendSMS_B() {
  // AT command to set SIM900 to SMS mode
  SIM900.print("AT+CMGF=1\r");
  delay(100);

  // mobile phone number to send SMS to
  SIM900.println("AT + CMGS = \"+18599926861\"");
  delay(100);

  // SMS_B message content
  SIM900.println("CHILD DETECTED IN CAR SEAT - VEHICLE TEMPERATURE IS 95 DEG.");
  delay(100);

  // End AT command with a ^Z, ASCII code 26
  SIM900.println((char)26);
  delay(100);
  SIM900.println();

  // SMS send delay and led
  digitalWrite(smsSimLED_B, HIGH);
  delay(5000);
  digitalWrite(smsSimLED_B, LOW);
}

// SETUP START +++++
void setup() {
  Serial.begin(9600);
  Serial.println("Child Safety Seat demo");
  scale.set_scale(calibration_factor); //Calibration value set during manufacture
  scale.tare(); //Assuming there is no weight on the seat at start up, reset the scale to 0
  Serial.println("Readings:");

  // INPUT SETUP
  // initialize the button pin as a input:
  pinMode(smsSend_B, INPUT);

  // OUTPUT SETUP
  // initialize the LED as an output:
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin_G, OUTPUT);
  pinMode(smsSimLED_B, OUTPUT);


  // sim 900 initialization
  SIM900.begin(19200);

  // DELAY FOR SIM900 NETWORK STARTUP
  delay(20000);

  // AT command to set SIM900 to SMS mode
  SIM900.print("AT+CMGF=1\r");
  delay(100);

  // Set module to send SMS data to serial out upon receipt
  SIM900.print("AT+CNMI=2,2,0,0,0\r");
  delay(100);

}
// SETUP END -----

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// MAIN LOOP START +++++
void loop() {
  Serial.print("Reading: ");
  Serial.print(scale.get_units(), 1); //scale.get_units() returns a float
  Serial.print(" lbs"); //You can change this to kg but you'll need to refactor the calibration_factor
  Serial.println();

  tempState=digitalRead(smsSend_B);

  led_light();

}
// MAIN LOOP END -----
