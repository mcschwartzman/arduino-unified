#include <Servo.h>
#include <Wire.h>
#include <Encoder.h>

//#include <L3G.h>
#include <PID_v1.h>

Servo left;
Servo right;
Servo back;

float X = 0;
float Y = 0;

long incr = 0;
int part = 0;

//L3G gyro;

Encoder leftE(0,0);
Encoder rightE(0,0);
Encoder backE(0,0);

int oldVal[3] = {0,0,0};
int newVal[3] = {0,0,0};

int oldTime[3] = {0,0,0};
int newTime[3] = {0,0,0};

//int offset = 0;
//
//int conv = 15;
//double accel = 0;

//double rot = 0;
//double rotVal = 0;

double L = 0;
double R = 0;

double LVal = 0;
double RVal = 0;

double speedLVal = 0;
double speedRVal = 0;

//PID myPID(&accel, &rot, &rotVal,.005,.0025,.0005, DIRECT);
PID speedL(&L, &speedLVal, &LVal, 0, 0, 0, DIRECT);
PID speedR(&R, &speedRVal, &RVal, 0, 0, 0, DIRECT);

void setup(){ 
  left.attach(4, 1000, 2000);
  right.attach(5, 1000, 2000);
  back.attach(6, 1000, 2000);
  Serial.begin(9600);
  Wire.begin();
  delay(50);

//  gyro.init();
//  gyro.writeReg(L3G_CTRL_REG4, 0x20); // 2000 dps full scale
//  gyro.writeReg(L3G_CTRL_REG1, 0x0F); // normal power mode, all axes enabled, 100 Hz
//  
//  gyro.enableDefault();
//  gyro.read();
//  offset = (float)gyro.g.x;
  
//  myPID.SetMode(AUTOMATIC);
//  myPID.SetOutputLimits(-90, 90);
//  rotVal = 0;
  speedL.SetMode(AUTOMATIC);
  speedL.SetOutputLimits(-90, 90);
  speedR.SetMode(AUTOMATIC);
  speedR.SetOutputLimits(-90, 90);
  
  delay(35);
  
}

void loop(){
//  gyro.read();
//  accel = -((float)gyro.g.x - offset);
//  Serial.println(accel);
//  myPID.Compute();
  
//  left.write(((-.5)*X - (sqrt(3)/2)*Y + R) + 90);
//  right.write(((-.5)*X + (sqrt(3)/2)*Y + R) + 90);
//  back.write((X + R) + 90);
  
  if(part == 0) {
    moveY(90);
  }
  else if(part == 1){
    moveX(90);
  }
  else if(part == 2){
    moveY(-90);
  }
  else if(part == 3){
    moveX(-90);
  }
  
  if(incr > 200){
    if(part == 0){
      part = 1;
    }
    else if(part == 1){
      part = 2;
    }
    else if(part == 2){
      part = 3;
    }
    else if(part == 3){
      part = 0;
    }
    incr = 0;
  }
  incr++;
  
}

double encSpeedL(){
  oldVal[0] = newVal[0];
  newVal[0] = leftE.read();
  
  oldTime[0] = newTime[0];
  newTime[0] = millis();
  
  return (double)(newVal[0] - oldVal[0])/(newTime[0] - oldTime[0]);
}

double encSpeedR(){
  oldVal[1] = newVal[1];
  newVal[1] = rightE.read();
  
  oldTime[1] = newTime[1];
  newTime[1] = millis();
  
  return (double)(newVal[1] - oldVal[1])/(newTime[1] - oldTime[1]);
}

double encSpeedB(){
  oldVal[2] = newVal[2];
  newVal[2] = backE.read();
  
  oldTime[2] = newTime[2];
  newTime[2] = millis();
  
  return (double)(newVal[2] - oldVal[2])/(newTime[2] - oldTime[2]);
}

void moveX(int val) {
  speedLVal = encSpeedL();
  speedRVal = encSpeedR();
  
  LVal = encSpeedB()/2;
  RVal = LVal;
  
  speedL.Compute();
  speedR.Compute();
  
  back.write(val);
  left.write(L);
  right.write(R);
}

void moveY(int val){
  left.write(-val);
  right.write(val);
  back.write(0);
}

void rot(int val){
  left.write(val);
  right.write(val);
  back.write(val);
}
