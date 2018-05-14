/**
 * @author Kelly Lynch
 * @date 2018-03-25
 * @brief Arduino code to control a motor, IMU sensor, and LIDAR-Lite-V3.
 *
 * @bugs None known
 * @todo everything
 */

/* -- Includes -- */
#include <math.h>
#include <EnableInterrupt.h>
#include <PID_v1.h>
#include <Wire.h>
#include <LIDARLite.h>
#include "pins.h"
#include "interrupts.h"
#include "motorFunctions.h"
#include "imuFunctions.h"
#include "lidarFunctions.h"


/* Setup Motor PID */
double setPoint, PIDInput, PIDOutput;
/* 
 *  Values produced via Zieglerâ€“Nichols method 
 *  Ku = 700, Tu = 0.75
 *  Ki slightly reduced to improve stability
 *  Speed varies +/- 3RPM
 */
double Kp = 420, Ki = 900, Kd = 39.375;
PID spinnerPID(&PIDInput, &PIDOutput, &setPoint, Kp, Ki, Kd, DIRECT);

/* Setup LIDAR */
LIDARLite lidar;

/* Global Variables */
unsigned long currentTime, previousTime, deltaT;
uint16_t currentEncoder, previousEncoder, deltaP;
int16_t encoderDiff;
float speedVal;
const int MAX_TICKS = 816;

void setup() {
  /* Setup Serial Communication */
  Serial.begin(250000);
  
  /* Enable Interrupts */
  enableInterrupt(ENCODERA_PIN, encoderAChange, CHANGE);
  enableInterrupt(INDEX_PIN, indexFall, FALLING); 
  
  /* Setup Peripherals */
  setupMotor();
  setupLidar(lidar);
  setupIMU();
  
  /* Calibrate Sensors */
  
  
  /* Setup PID Values */
  setPoint = 50;
  spinnerPID.SetMode(AUTOMATIC);
  spinnerPID.SetOutputLimits(0, 65535);
  
  /* Seek Motor Index */
  seekIndex();

  /* Setup Time and encoder tracking */
  currentTime = millis();
  currentEncoder = encoderPosition;
  
  /* Allow motor to be controlled */
  startMotor();
  setup16bitPWM();

  delay(1000);
}


void loop() {   
  /* -- Report motor RPM -- */ 
  /* Check if enough time has elapsed to run the RPM calculations */
  deltaT = millis()-previousTime;
  if(deltaT > 50){
    /* Record Time at calculation */
    previousTime = millis();
    /* Position Difference */
    previousEncoder = currentEncoder;
    currentEncoder = encoderPosition;
    /* Deal with the encoder count resetting to zero */
    encoderDiff = currentEncoder - previousEncoder;
    deltaP = encoderDiff % MAX_TICKS;
    /* Speed */
    speedVal = (deltaP*73.5)/deltaT;    

    /* Adjust PID if reported speed is less than max possible */
    if(speedVal < 160){
      PIDInput = speedVal;
      spinnerPID.Compute();
      OCR1B = PIDOutput;
    }
    

  }

  /* Record time */
  unsigned long distReadTime = micros();
  /* Read distance of lidar */
  int lidarDistance = lidar.distance();
  /* Read intensity of the return */
  byte arrayToSave[1];
  char intensityReg = 0x0e;
  lidar.read(intensityReg, 1, arrayToSave, true, LIDARLITE_ADDR_DEFAULT);
  byte lidarIntensity = arrayToSave[0];
  /* Report Angle of Spinner */
  double angleRads = 2 * fmod(encoderPosition*0.0076968, 1.047198) + (PI/6);

  Serial.print("~S:");
  Serial.print(distReadTime);
  Serial.print(",");
  Serial.print(angleRads, 6);
  Serial.print(",");
  Serial.print(lidarDistance);
  Serial.print(", ");
  Serial.print(lidarIntensity);
  Serial.print("~~\n\r");  
}

