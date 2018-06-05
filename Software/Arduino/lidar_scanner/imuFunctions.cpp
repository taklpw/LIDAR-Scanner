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
void setupIMU(MPU6050 &mpuObject, HMC5883L &magObject){
  /* Check MPU605 exists */
  while(!mpuObject.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G)){
    Serial.println("Could not find valid MPU6050");
    delay(500);
  }

  mpuObject.setI2CMasterModeEnabled(false);
  mpuObject.setI2CBypassEnabled(true);
  mpuObject.setSleepEnabled(false);
  /* Check HMC5883 Exists */
  while(!magObject.begin()){
      Serial.println("Could not find valid HMC5883L");
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
  unsigned long endTime = micros();
  
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
* @brief Read Magnetometer Function
* Reads the Magnetometer.
* 
* @return String with Magnetometer information
*/
String readMag(HMC5883L &magObject){
  /* Record time, then measurements */
  unsigned long magReadTime = micros();
  Vector magNorm = magObject.readNormalize();

  /* Send Gyroscope information */
  String MagString = "~M:";
  MagString += magReadTime;
  MagString += ",";
  MagString += String(magNorm.XAxis, 6);
  MagString += ",";
  MagString += String(magNorm.YAxis, 6);
  MagString += ",";
  MagString += String(magNorm.ZAxis, 6);
  MagString += "~~\n\r";
  return MagString;
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

