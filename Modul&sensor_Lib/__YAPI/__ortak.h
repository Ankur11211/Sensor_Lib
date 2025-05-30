#pragma once
#include "Arduino.h"

#if defined(__AVR__)
#include <avr/pgmspace.h>
#elif defined(ESP8266) || defined(ESP32)
#include <pgmspace.h>
#elif defined(STM32F1xx) || defined(STM32F4xx) || defined(STM32L4xx)
#include "stm32f1xx_hal.h"
#else
#endif

extern void pinmod_hizli(uint8_t pin, uint8_t mode);
extern void dgwrite_hizli(uint8_t pin, uint8_t val);
extern void analogWrite_hizli(uint8_t pin, uint8_t dutyCycle);
extern uint8_t dgread_hizli(uint8_t pin);
extern uint16_t analogRead_hizli(uint8_t pin);
