/**
 * @file imuFunctions.cpp
 * @author Kelly Lynch
 * @date 20 May 2018
 * @brief Contains IMU sensing functions
 */

/* -- Includes -- */
#include <Arduino.h>
#include "imuFunctions.h"
#include "pins.h"

/* -- Functions -- */
/**
* @brief Setup IMU Function
* Sets up and initialises the Accelerometer, Gyroscope, and Magnetometer.
* 
* @return Does not return
*/
void setupIMU(MPU6050 &mpuObject){
  /* Check MPU605 exists */
  while(!mpuObject.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G)){
    Serial.println("Could not find valid MPU6050");
    delay(500);
  }
  
  /* Calibrate Gyroscope and set sensitivity threshold */
  mpuObject.calibrateGyro();
  mpuObject.setThreshold(3);
}


/**
* @brief Read Accelerometer Function
* Reads the Accelerometer.
* 
* @return String with Accelerometer information
*/
String readAccel(MPU6050 &mpuObject){
    /* Record time, then measurements */
  unsigned long imuReadTime = micros();
  Vector accelNorm = mpuObject.readNormalizeAccel(); 

  
  /* Send Acceleration information */
  String AccelString = "~A:";
  AccelString += imuReadTime;
  AccelString += ",";
  AccelString += String(accelNorm.XAxis, 6);
  AccelString += ",";
  AccelString += String(accelNorm.YAxis, 6);
  AccelString += ",";
  AccelString += String(accelNorm.ZAxis, 6);
  AccelString += "~~\n\r";
  return AccelString;
}


/**
* @brief Read Gyroscopic Function
* Reads the Gyroscope.
* 
* @return String with Gyroscope information
*/
String readGyro(MPU6050 &mpuObject){
  /* Record time, then measurements */
  unsigned long gyroReadTime = micros();
  Vector gyroNorm = mpuObject.readNormalizeGyro();

  /* Send Gyroscope information */
  String GyroString = "~G:";
  GyroString += gyroReadTime;
  GyroString += ",";
  GyroString += String(gyroNorm.XAxis, 6);
  GyroString += ",";
  GyroString += String(gyroNorm.YAxis, 6);
  GyroString += ",";
  GyroString += String(gyroNorm.ZAxis, 6);
  GyroString += "~~\n\r";
  return GyroString;
}


/**
* @brief Read Both Function
* Reads the Gyroscope and accelerometer.
* 
* @return String with Gyroscope and Accelerometer information
*/
String readIMU(MPU6050 &mpuObject){
  /* Record time, then measurements */
  unsigned long imuReadTime = micros();
  Vector accelNorm = mpuObject.readNormalizeGyro();
  Vector gyroNorm = mpuObject.readNormalizeGyro();

  /* Send Gyroscope information */
  String IMUString = "~I:";
  IMUString += imuReadTime;
  IMUString += ",";
  IMUString += String(accelNorm.XAxis, 6);
  IMUString += ",";
  IMUString += String(accelNorm.YAxis, 6);
  IMUString += ",";
  IMUString += String(accelNorm.ZAxis, 6);
  IMUString += ",";
  IMUString += String(gyroNorm.XAxis, 6);
  IMUString += ",";
  IMUString += String(gyroNorm.YAxis, 6);
  IMUString += ",";
  IMUString += String(gyroNorm.ZAxis, 6);
  IMUString += "~~\n\r";
  return IMUString;
}

