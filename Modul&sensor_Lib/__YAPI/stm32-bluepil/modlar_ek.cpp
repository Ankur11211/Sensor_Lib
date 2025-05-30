#if defined(STM32F1)

#include "stm32f1xx.h"

struct PinDef
{
    GPIO_TypeDef *port;
    uint16_t pin;
};

// Hızlı pinMode fonksiyonu
inline void pinmod_hizli(const PinDef &pin, uint8_t mode)
{
    uint8_t pinNum = 0;
    uint16_t pinMask = pin.pin;
    while (pinMask > 1)
    {
        pinMask >>= 1;
        pinNum++;
    }

    volatile uint32_t *crReg = (pinNum < 8) ? &pin.port->CRL : &pin.port->CRH;
    uint8_t shift = (pinNum % 8) * 4;
    uint32_t tmp = *crReg;

    if (mode == OUTPUT)
    {
        tmp &= ~(0xF << shift);
        tmp |= (0x3 << shift); // Output push-pull, max 50MHz
    }
    else
    {
        tmp &= ~(0xF << shift);
        tmp |= (0x8 << shift); // Input with pull-up/down
        pin.port->ODR |= pin.pin; // pull-up aktif et
    }

    *crReg = tmp;
}

// Hızlı digitalWrite fonksiyonu
inline void dgwrite_hizli(const PinDef &pin, uint8_t val)
{
    if (val == HIGH)
        pin.port->BSRR = pin.pin; // Set bit
    else
        pin.port->BRR = pin.pin;  // Reset bit
}

// DigitalRead fonksiyonu
inline uint8_t dgread_hizli(const PinDef &pin)
{
    return (pin.port->IDR & pin.pin) ? HIGH : LOW;
}

// AnalogRead fonksiyonu (ADC1, kanal eşleme gerekli)
// PinDef yerine ADC kanal numarasını parametre olarak alabiliriz.
uint16_t analogRead_hizli(uint8_t adcChannel)
{
    // ADC1 aktif değilse aç
    if (!(ADC1->CR2 & ADC_CR2_ADON))
    {
        ADC1->CR2 |= ADC_CR2_ADON;
        for (volatile int i=0; i<1000; i++); // ADC stabilize süresi
    }

    // Kanal seçimi (SQR3[4:0]) 5-bit ADC kanal seçimi
    ADC1->SQR3 = adcChannel & 0x1F;

    // Dönüşümü başlat
    ADC1->CR2 |= ADC_CR2_ADON | ADC_CR2_SWSTART;

    // Dönüşüm bitene kadar bekle
    while (!(ADC1->SR & ADC_SR_EOC))
        ;

    // ADC değerini oku
    return ADC1->DR;
}

// AnalogWrite fonksiyonu - PWM (timer ve kanal konfigürasyonu gerekir)
// Örnek: Timer2 kanal1 ile PWM yazma
void analogWrite_hizli(GPIO_TypeDef *port, uint16_t pin, uint16_t dutyCycle)
{
    // Burada pin ve timer/ch seçimi yapılmalı.
    // Örneğin: PA0 => TIM2_CH1

    // PWM konfigürasyonunun önceden yapılmış olması gerekir.

    // dutyCycle: 0-65535 arası

    // Örnek: dutyCycle 0-1000 scale olarak kabul edilirse
    // TIM2->CCR1 = dutyCycle;

    // Burada uygulamanın timer ve kanal bilgisine göre ayarlanmalı.
}

#endif
