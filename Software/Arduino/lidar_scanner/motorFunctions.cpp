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
  startMotor();
  /* Seek at ~39% speed */
  analogWrite(MOTOR_PWM_PIN, 100);
  /* Wait until index is found */
  while(!indexFlag);
  /* Apply brakes to the motor quickly */
  stopMotor();
}


/**
* @brief Start Motor Function
* Sets up motor to be able to rotate
* 
* @return Does not return
*/
void startMotor(){
  bitClear(PORTB, MOTOR_IN1_PORTB);
  bitSet(PORTB, MOTOR_IN2_PORTB);  
}


/**
* @brief Stop Motor Function
* Stops the motor
* 
* @return Does not return
*/
void stopMotor(){
  bitSet(PORTB, MOTOR_IN1_PORTB);
  bitSet(PORTB, MOTOR_IN2_PORTB);
  analogWrite(MOTOR_PWM_PIN, 0);  
}


/**
* @brief Setup 16 bit PWM Function
* Sets Pin 10 to have a 16-bit output
* 
* Credit to https://www.arduinoslovakia.eu/blog/2017/7/16-bitove-rozlisenie-pwm-pre-arduino?lang=en
* for this example.
* 
* @return Does not return
*/
void setup16bitPWM(){
  uint16_t icr = 0xffff;
  DDRB  |= _BV(PB1) | _BV(PB2);       /* set pins as outputs */
  TCCR1A = _BV(COM1A1) | _BV(COM1B1)  /* non-inverting PWM */
        | _BV(WGM11);                 /* mode 14: fast PWM, TOP=ICR1 */
  TCCR1B = _BV(WGM13) | _BV(WGM12)
        | _BV(CS10);                  /* prescaler 1 */
  ICR1 = icr;                         /* TOP counter value (freeing OCR1A*/
}



