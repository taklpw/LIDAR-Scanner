/**
 * @author Kelly Lynch
 * @date 2018-03-25
 * @brief Arduino code to control a motor, IMU sensor, and LIDAR-Lite-V3.
 *
 * @bugs None known
 * @todo everything
 */

/* -- Includes -- */
#include <EnableInterrupt.h>
#include <PID_v1.h>
#include "pins.h"
#include "interrupts.h"
#include "motorFunctions.h"
#include "imuFunctions.h"
#include "lidarFunctions.h"


/* Setup Motor PID */
double setPoint, PIDInput, PIDOutput;
/* 
 *  Values produced via Ziegler–Nichols method 
 *  Ku = 4, Tu = 0.75
 *  Ki slightly reduced to improve stability
 *  Speed varies +/- 5RPM
 */

double Kp = 2.4, Ki = 2.6, Kd = 0.225;
PID spinnerPID(&PIDInput, &PIDOutput, &setPoint, Kp, Ki, Kd, DIRECT);

/* Global Variables */
unsigned long currentTime, previousTime, deltaT;
uint16_t currentEncoder, previousEncoder, deltaP;
int16_t encoderDiff;
float speedVal;

void setup() {
  /* Setup Serial Communication */
  Serial.begin(9600);
  
  /* Enable Interrupts */
  enableInterrupt(ENCODERA_PIN, encoderAChange, CHANGE);
  enableInterrupt(INDEX_PIN, indexFall, FALLING); 
  
  /* Setup Peripherals */
  setupMotor();
  setupLidar();
  setupIMU();

  /* Calibrate Sensors */
  
  
  /* Setup PID Values */
  setPoint = 105;
  spinnerPID.SetMode(AUTOMATIC);
  
  /* Seek Motor Index */
  seekIndex();

  currentTime = millis();
  currentEncoder = encoderPosition;
  delay(1000);
    /* Set motor direction */
  bitClear(PORTB, MOTOR_IN1_PORTB);
  bitSet(PORTB, MOTOR_IN2_PORTB);
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
    deltaP = encoderDiff % maxEncoderPosition;
    /* Speed */
    speedVal = (deltaP*73.5)/deltaT;    

    /* Adjust PID if reported speed is less than max possible */
    if(speedVal < 160){
      PIDInput = speedVal;
      spinnerPID.Compute();
      analogWrite(MOTOR_PWM_PIN, PIDOutput);
    }
  }
  
}
