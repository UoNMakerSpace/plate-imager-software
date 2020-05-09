//Uses the TeensyStep - Fast Stepper Library for PJRC Teensy boards V.1
//https://github.com/luni64/TeensyStep/tree/Version-1

#include "StepControl.h"

// stepper and  controller
constexpr int stpPin = 20, dirPin = 21;
Stepper motor(stpPin, dirPin);
StepControl<> controller;

// hall sensor
const int hallPin = 1;     // the number of the hall effect sensor pin
int hallState = 0;          // variable for reading the hall sensor status
const int homePin = 0;
int homeState = 0;
int move_finished = 1;  // Used to check if move is completed
int lastPos = 0;
long PositionX;  // Used to store the X step or position value




void setup()
{
  while (!Serial);
  //Serial.println("Enter steps to move");
  motor.setMaxSpeed(40000);
  motor.setAcceleration(20000);
  pinMode(hallPin, INPUT_PULLUP);  // hall sensor pin as an input
  pinMode(homePin, INPUT_PULLUP);  // hall sensor pin as an input
}




void loop()
{
  handleCommands();   // handle incomming serial commands on the serial interface
  hallState = digitalRead(hallPin);
  homeState = digitalRead(homePin);

// home sensor
while (homeState == LOW )
  {
    controller.emergencyStop();         
    // motor.setTargetRel(-1);
    // controller.move(motor);
    homeState = digitalRead(homePin);
    //Serial.println("Endstop sensor activated");
  }

// endstop hall sensor
while (hallState == LOW )
  {
    controller.emergencyStop();         
    motor.setTargetRel(1);
    controller.move(motor);
    motor.setPosition(0);
    hallState = digitalRead(hallPin);
    //Serial.println("Home sensor activated");
  }

  
// return position 
  if ((!controller.isRunning()) && (move_finished == 0))
  {
    int currentPos = motor.getPosition(); 
    Serial.println ("Completed");

    //these 2 lines can be used for testing to report the current position
    //Serial.print("Position: ");
    //Serial.println(currentPos);
    
    move_finished = 1;
  } 
}



void handleCommands()
{
  while (Serial.available() > 0)  { // Check if values are available in the Serial Buffer
    PositionX = Serial.parseInt(); // Put first numeric value from buffer in PositionX variable
    PositionX = constrain(PositionX,-2000,71800);
    //Serial.print(PositionX);
    //Serial.print(" X position: ");
    motor.setTargetAbs(PositionX);
    controller.moveAsync(motor);
   //Serial.println("...moving");
    move_finished = 0; // Set variable for checking move of the Steppers

  }

}
