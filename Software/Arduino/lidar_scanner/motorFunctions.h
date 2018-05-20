/**
 * @file motorFunctions.h
 * @author Kelly Lynch
 * @date 20 May 2018
 * @brief Header file for the motor functions
 */

#ifndef H_motorFunctions
#define H_motorFunctions

/* -- Includes -- */
#include <Arduino.h>
#include <PID_v1.h>
#include "pins.h"

/* -- Function Prototypes -- */
void setupMotor(void);
void seekIndex(void);
void startMotor(void);
void stopMotor(void);
void setup16bitPWM(void);

#endif

