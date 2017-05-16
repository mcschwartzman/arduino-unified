#include <Servo.h>

Servo motor;

unsigned long lastTime;
double Input, Output, Setpoint;
double errSum, lastErr;
double kp, ki, kd;

// Min = 0, Max = 1023



 

void setup()
{
  motor.attach(9, 1000, 2000);
  Serial.begin(9600);
  Setpoint = 700;
}

void loop()
{
  Input = analogRead(A1);
    
  PIDtune(.001,0,100);
  PIDcalc();  
  
  motor.write((int)(90 + Output));
  
  Serial.print(Input);
  Serial.print(' ');
  Serial.println(Output);
}

void PIDcalc()
{  
  unsigned long now = millis();
  double timeChange = (double)(now-lastTime);
  
  double error = -(Setpoint-Input);
  errSum += (error*timeChange);
  double dErr = (error-lastErr)/timeChange;
  
  Output = kp*error + ki*errSum +kd*dErr;
  
  lastErr = error;
  lastTime = now;
  
}

void PIDtune(double Kp, double Ki, double Kd)
{
  kp = Kp;
  ki = Ki;
  kd = Kd;
}
