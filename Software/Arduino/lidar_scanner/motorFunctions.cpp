/**
 * @file motorFunctions.cpp
 * @author Kelly Lynch
 * @date 18 Mar 2018
 * @brief Contains functions for motor control
 * @todo Implement
 */
 
/* -- Includes -- */
#include "motorFunctions.h"
#include "interrupts.h"

/* -- Functions -- */
/**
* @brief Setup Motor Function
* Setup Motor Related Pins
* 
* @return Does not return
*/
void setupMotor(){    
  pinMode(ENCODERA_PIN, INPUT);
  pinMode(ENCODERB_PIN, INPUT);
  pinMode(INDEX_PIN, INPUT);
  pinMode(MOTOR_PWM_PIN, OUTPUT);
  pinMode(MOTOR_IN1_PIN, OUTPUT);
  pinMode(MOTOR_IN2_PIN, OUTPUT);  
}


/**
* @brief Seek Index Function
* Rotates the motor until the index is found
* 
* @return Does not return
*/
void seekIndex(){ 
  /* Set motor direction */
  bitClear(PORTB, MOTOR_IN1_PORTB);
  bitSet(PORTB, MOTOR_IN2_PORTB);
  /* Seek at ~39% speed */
  analogWrite(MOTOR_PWM_PIN, 100);
  /* Wait until index is found */
  while(!indexFlag);
  /* Apply brakes to the motor quickly */
  bitSet(PORTB, MOTOR_IN1_PORTB);
  analogWrite(MOTOR_PWM_PIN, 0);
}


/**
* @brief Set Motor Speed Function
* Long desc
* 
* @return Does not return
* @TODO Implement
*/
void setMotorSpeed(int rpm){
  
}


/**
* @brief Stop Motor Function
* Long desc
* 
* @return Does not return
* @TODO Implement
*/
void stopMotor(){
  
}

