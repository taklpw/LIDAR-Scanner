/**
 * @file imuFunctions.h
 * @author Kelly Lynch
 * @date 20 May 2018
 * @brief Header file for imu sensing functions
 */

#ifndef H_imuFunctions
#define H_imuFunctions

/* -- Includes -- */
#include <Arduino.h>
#include "pins.h"
#include <Wire.h>
#include <MPU6050.h>

/* -- Function Prototypes -- */
void setupIMU(MPU6050&);
String readAccel(MPU6050&);
String readGyro(MPU6050&);

#endif

