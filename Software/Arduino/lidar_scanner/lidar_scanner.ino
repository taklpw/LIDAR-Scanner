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
#include "pins.h"
#include "interrupts.h"
#include "motorFunctions.h"
#include "imuFunctions.h"
#include "lidarFunctions.h"


/* Setup Motor PID */
double setPoint, PIDInput, PIDOutput;
/* 
 *  Values produced via Ziegler–Nichols method 
 *  Ku = 700, Tu = 0.75
 *  Ki slightly reduced to improve stability
 *  Speed varies +/- 3RPM
 */
double Kp = 420, Ki = 900, Kd = 39.375;
PID spinnerPID(&PIDInput, &PIDOutput, &setPoint, Kp, Ki, Kd, DIRECT);

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
  setupLidar();
  setupIMU();

  /* Calibrate Sensors */
  
  
  /* Setup PID Values */
  setPoint = 105;
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

  /* Report Angle of Spinner */
  double angleRads = fmod(encoderPosition*0.0076968, 1.047198);
  //double angleRads = encoderPosition*0.0076968 % 1.047198;
  
  Serial.println(angleRads, 5);
  //Serial.print(",\n\r");
  Serial.print("\t");
  //Serial.println(encoderPosition);
  
}
