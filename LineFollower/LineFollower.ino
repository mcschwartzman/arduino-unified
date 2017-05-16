#include <Servo.h>

//Photoresistors
int photoLeft = 1; //value read in
int photoRight = 1;
const int photoPinLeft = 31; //pin location
const int photoPinRight = 32; //(D32)
//Motors
Servo left;
Servo right;
Servo arm;
const int revLeftPower = 70; //low power
const int revRightPower = 120;
const int highLeftPower = 110; //high power
const int highRightPower = 60;
const int motorPinLeft = 6; //pin locations
const int motorPinRight = 7;
const int motorPinArm = 8;
const int gain = 1;
//misc.
int running = 1; //switch to end code
int down = 292;
int up = 413;
int sensorValue = analogRead(A0);
int potpos = sensorValue;
int posvar;
int errorvalue = potpos - posvar;
boolean roadBlock;

void setup(){
  
  left.attach(motorPinLeft);
  right.attach(motorPinRight);
  arm.attach(motorPinArm);
  pinMode(photoPinLeft, INPUT);
  pinMode(photoPinRight, INPUT);
  Serial.begin(9600);
}

void loop ()
{
  
}

void grab() 

{
  while (roadBlock == false)
  {   
    linefollowing ();
  } 
 while (potpos != posvar)
  {
    if (errorvalue > 0)
    {
      arm.write(180);
    }
    if (errorvalue < 0)
    {
      arm.write(0);
    }
  }  
}

void printVolt()
{
    int sensorValue = analogRead(A0);
    Serial.println(sensorValue);
    delay(1);
}

void linefollowing(){
    
    //read photoresistors
    photoLeft = digitalRead(photoPinLeft);
    photoRight = digitalRead(photoPinRight);
    
    while(photoLeft != LOW || photoRight != LOW){ //while not on black perpindicular
    
    //read photoresistors
    photoLeft = digitalRead(photoPinLeft);
    photoRight = digitalRead(photoPinRight);
    Serial.println(photoLeft);
    Serial.println(photoRight);
    
    //write the motors
    if(photoLeft == HIGH){ //if over a white area
      left.write(highLeftPower); //high speed
    }
    else{                   //if over a black line
      left.write(revLeftPower); //backwards
    }
    if(photoRight == HIGH){
      right.write(highRightPower);
    }
    else{
      right.write(revRightPower);
    }
  } //while loop ends
    
  left.write(highLeftPower); //stop
  right.write(highRightPower);
  roadBlock = true;
}
