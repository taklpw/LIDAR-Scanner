/**
 * @file pins.cpp
 * @author Kelly Lynch
 * @date 18 Mar 2018
 * @brief Contains values for pins
 */

/* -- Includes -- */
#include <Arduino.h>

/* -- Pin Definitions -- */
/* UART Pins */
uint8_t UART_TX_PIN   = 1;
uint8_t UART_TX_PORTD = 1;
uint8_t UART_RX_PIN   = 0;
uint8_t UART_RX_PORTD = 0;
/* Encoder Pins */
uint8_t INDEX_PIN    = 2;
uint8_t INDEX_PORTD  = 2;
uint8_t ENCODERA_PIN = 3;
uint8_t ENCODERA_PORTD = 3;
uint8_t ENCODERB_PIN = 4;
uint8_t ENCODERB_PORTD = 4; 
/* Motor Pins */
uint8_t MOTOR_PWM_PIN = 9;
uint8_t MOTOR_PWM_PORTB = 1;
uint8_t MOTOR_EN_PIN  = 10;
uint8_t MOTOR_EN_PORTB = 2;
/* I2C Pins */
uint8_t SCL_PIN = A5;
uint8_t SCL_PORTC = 5;
uint8_t SDA_PIN = A4;
uint8_t SDA_PORTC = 4;
