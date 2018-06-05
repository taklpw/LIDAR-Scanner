/**
 * @author Kelly Lynch
 * @date 2018-05-20
 * @brief Arduino code to control a motor, IMU sensor, and LIDAR-Lite-V3.
 *
 * @bugs None known
 */

/* -- Includes -- */
#include <math.h>
#include <EnableInterrupt.h>
#include <PID_v1.h>
#include <Wire.h>
#include <LIDARLite.h>
#include <MPU6050.h>
#include <HMC5883L.h>
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

/* Setup Accelerometer and Gyroscope*/
MPU6050 mpu;

/* Magnetometer */
HMC5883L mag;


/* Global Variables */
unsigned long currentTime, previousTime, deltaT;
uint16_t currentEncoder, previousEncoder, deltaP;
int16_t encoderDiff;
float speedVal;
const int MAX_TICKS = 816;
unsigned long accelTime, gyroTime, imuTime;

void setup() {
  /* Setup Serial Communication */
  Serial.begin(250000);
  
  /* Enable Interrupts */
  enableInterrupt(ENCODERA_PIN, encoderAChange, CHANGE);
  enableInterrupt(INDEX_PIN, indexFall, FALLING); 
  
  /* Setup Peripherals */
  setupMotor();
  setupLidar(lidar);
  setupIMU(mpu, mag);
  
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

  /* Wait before starting the program */
  delay(1000);

  /* Mark time for sensor readings */
  accelTime = millis();
  gyroTime = millis();
  imuTime = millis();
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
  
  /* -- Report IMU Measurements -- */
//  if(millis()-imuTime >= 20){
//    imuTime = millis();
//    Serial.print(readIMU(mpu));  
//  }
  
  /* -- Report LIDAR Measurements -- */
  /* Check if LIDAR is busy */  
  /* Read lidar as fast as possible (no set frequency) */
  byte statusArray[1];
  char statusReg = 0x01;
  lidar.read(statusReg, 1, statusArray, false, LIDARLITE_ADDR_DEFAULT);
  int busyFlag = bitRead(statusArray[0], 0);
  if(busyFlag == 0){
    Serial.print(readLidar(lidar));
  }
  

//    if(mpu.readActivites().isDataReady){
      Serial.print(readAccel(mpu));
//    }
  
//    if(mpu.readActivites().isDataReady){
      Serial.print(readGyro(mpu));
//    }

//    if(mag.readActivities().isDataReady){
      Serial.print(readMag(mag));  
//    }
}

