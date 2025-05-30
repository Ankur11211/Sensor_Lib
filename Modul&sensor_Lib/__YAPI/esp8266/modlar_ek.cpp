#if defined(ESP8266)

#include <Arduino.h>

inline void pinmod_hizli(uint8_t pin, uint8_t mode) 
{ 
    pinMode(pin, mode); 
}

inline void dgwrite_hizli(uint8_t pin, uint8_t val)
{
    if (val == LOW)
        GPIO_REG_WRITE(GPIO_OUT_W1TC_ADDRESS, 1 << pin); 
    else
        GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, 1 << pin); 
}

inline uint8_t dgread_hizli(uint8_t pin)
{
    return digitalRead(pin);
}
inline uint16_t analogRead_hizli()
{
    return analogRead(A0);
}
inline void analogWrite_hizli(uint8_t pin, uint8_t dutyCycle)
{
    analogWrite(pin, dutyCycle);
}

#endif
