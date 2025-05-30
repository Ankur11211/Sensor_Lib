#if defined(__AVR__)

#include "Arduino.h"

#define PIN_TO_PORT_REG(pin) ((pin) < 8 ? &PORTD : ((pin) < 14 ? &PORTB : &PORTC))
#define PIN_TO_DDR_REG(pin) ((pin) < 8 ? &DDRD : ((pin) < 14 ? &DDRB : &DDRC))
#define PIN_TO_BIT(pin) ((pin) < 8 ? (pin) : ((pin) < 14 ? (pin - 8) : (pin - 14)))

inline void pinmod_hizli(uint8_t pin, uint8_t mode)
{
    volatile uint8_t *ddr = PIN_TO_DDR_REG(pin);
    uint8_t bit = PIN_TO_BIT(pin);
    if (mode == OUTPUT)
        *ddr |= (1 << bit);
    else
        *ddr &= ~(1 << bit);
}

inline void dgwrite_hizli(uint8_t pin, uint8_t val)
{
    volatile uint8_t *port = PIN_TO_PORT_REG(pin);
    uint8_t bit = PIN_TO_BIT(pin);
    if (val == HIGH)
        *port |= (1 << bit);
    else
        *port &= ~(1 << bit);
}

inline uint8_t dgread_hizli(uint8_t pin)
{
    volatile uint8_t *pinReg;
    uint8_t bit = PIN_TO_BIT(pin);

    if (pin < 8)
        pinReg = &PIND;
    else if (pin < 14)
        pinReg = &PINB;
    else
        pinReg = &PINC;

    return ((*pinReg) & (1 << bit)) ? HIGH : LOW;
}

inline uint16_t analogRead_hizli(uint8_t pin)
{
    uint8_t analogPin = pin;
    if (pin >= 14)
        analogPin = pin - 14;

    ADMUX = (ADMUX & 0xF0) | (analogPin & 0x0F);
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC))
        ;
    return ADC;
}

inline void analogWrite_hizli(uint8_t pin, uint8_t dutyCycle)
{
    analogWrite(pin, dutyCycle);
}

#endif
