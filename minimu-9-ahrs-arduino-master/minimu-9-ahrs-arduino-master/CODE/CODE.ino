#include <Servo.h>

int SENSOR_SIGN[9] = {
  1,1,1,-1,-1,-1,1,1,1}; //Correct directions x,y,z - gyro, accelerometer, magnetometer

#include <Wire.h>


#define GRAVITY 256  //this equivalent to 1G in the raw data coming from the accelerometer 
#define ToRad(x) ((x)*0.01745329252)  // *pi/180
#define ToDeg(x) ((x)*57.2957795131)  // *180/pi
#define Gyro_Gain_X 0.07 //X axis Gyro gain
#define Gyro_Gain_Y 0.07 //Y axis Gyro gain
#define Gyro_Gain_Z 0.07 //Z axis Gyro gain
#define Gyro_Scaled_X(x) ((x)*ToRad(Gyro_Gain_X)) //Return the scaled ADC raw data of the gyro in radians for second
#define Gyro_Scaled_Y(x) ((x)*ToRad(Gyro_Gain_Y)) //Return the scaled ADC raw data of the gyro in radians for second
#define Gyro_Scaled_Z(x) ((x)*ToRad(Gyro_Gain_Z)) //Return the scaled ADC raw data of the gyro in radians for second
#define M_X_MIN -421
#define M_Y_MIN -639
#define M_Z_MIN -238
#define M_X_MAX 424
#define M_Y_MAX 295
#define M_Z_MAX 472
#define Kp_ROLLPITCH 0.02
#define Ki_ROLLPITCH 0.00002
#define Kp_YAW 1.2
#define Ki_YAW 0.00002
#define OUTPUTMODE 1
#define PRINT_ANALOGS 0 //Will print the analog raw data
#define PRINT_EULER 1   //Will print the Euler angles Roll, Pitch and Yaw
#define STATUS_LED 13 

float G_Dt=0.02;    // Integration time (DCM algorithm)  We will run the integration loop at 50Hz if possible

long timer=0;   //general purpuse timer
long timer_old;
long timer24=0; //Second timer used to print values 
int AN[6]; //array that stores the gyro and accelerometer data
int AN_OFFSET[6]={
  0,0,0,0,0,0}; //Array that stores the Offset of the sensors
int gyro_x;
int gyro_y;
int gyro_z;
int accel_x;
int accel_y;
int accel_z;
int magnetom_x;
int magnetom_y;
int magnetom_z;
float c_magnetom_x;
float c_magnetom_y;
float c_magnetom_z;
float MAG_Heading;
float Accel_Vector[3]= {
  0,0,0}; //Store the acceleration in a vector
float Gyro_Vector[3]= {
  0,0,0};//Store the gyros turn rate in a vector
float Omega_Vector[3]= {
  0,0,0}; //Corrected Gyro_Vector data
float Omega_P[3]= {
  0,0,0};//Omega Proportional correction
float Omega_I[3]= {
  0,0,0};//Omega Integrator
float Omega[3]= {
  0,0,0};
float roll;
float pitch;
float yaw;
float errorRollPitch[3]= {
  0,0,0}; 
float errorYaw[3]= {
  0,0,0};
unsigned int counter=0;
byte gyro_sat=0;
float DCM_Matrix[3][3]= {
  {
    1,0,0    }
  ,{
    0,1,0    }
  ,{
    0,0,1    }
}; 
float Update_Matrix[3][3]={
  {
    0,1,2  }
  ,{
    3,4,5  }
  ,{
    6,7,8  }
}; //Gyros here
float Temporary_Matrix[3][3]={
  {
    0,0,0    }
  ,{
    0,0,0    }
  ,{
    0,0,0    }
};

Servo right;
Servo left;

int motorleftpower = 90;
int motorrightpower = 90;
int error;

void setup()
{
  right.attach(6,1000,2000);
  left.attach(7,1000,2000);
  
  initialize();
}

void loop()
{

  displayValues();
  error = 0-ToDeg(yaw);
  if (error > 0)
  {
    motorrightpower = motorrightpower + (error);
   motorleftpower = motorleftpower - (error);
    
  }
  if (error<0)
  {
    motorleftpower = motorleftpower + (error);
    motorrightpower = motorrightpower - (error);
    
  }
  else
  {
    left.write(180-motorleftpower);
    right.write(motorrightpower);
  }
}

