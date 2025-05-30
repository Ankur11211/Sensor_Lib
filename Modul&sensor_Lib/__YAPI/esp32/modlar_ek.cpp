#if defined(ESP32)

#include <driver/gpio.h>

inline void pinmod_hizli(uint8_t pin, uint8_t mode)
{
    if (mode == OUTPUT)
        gpio_set_direction((gpio_num_t)pin, GPIO_MODE_OUTPUT);
    else if (mode == INPUT)
        gpio_set_direction((gpio_num_t)pin, GPIO_MODE_INPUT);
    else if (mode == INPUT_PULLUP)
    {
        gpio_set_direction((gpio_num_t)pin, GPIO_MODE_INPUT);
        gpio_pulldown_dis((gpio_num_t)pin);
        gpio_pullup_en((gpio_num_t)pin);
    }
    else if (mode == INPUT_PULLDOWN)
    {
        gpio_set_direction((gpio_num_t)pin, GPIO_MODE_INPUT);
        gpio_pullup_dis((gpio_num_t)pin);
        gpio_pulldown_en((gpio_num_t)pin);
    }
}
inline void dgwrite_hizli(uint8_t pin, uint8_t val)
{
    if (val == HIGH)
        GPIO.out_w1ts = (1 << pin);
    else
        GPIO.out_w1tc = (1 << pin);
}

inline uint8_t dgread_hizli(uint8_t pin)
{
    return (GPIO.in >> pin) & 1;
}

inline uint16_t analogRead_hizli(uint8_t pin)
{
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_channel_t channel;
    switch (pin)
    {
    case 32:
        channel = ADC1_CHANNEL_4;
        break;
    case 33:
        channel = ADC1_CHANNEL_5;
        break;
    case 34:
        channel = ADC1_CHANNEL_6;
        break;
    case 35:
        channel = ADC1_CHANNEL_7;
        break;
    case 36:
        channel = ADC1_CHANNEL_0;
        break;
    case 39:
        channel = ADC1_CHANNEL_3;
        break;
    default:
        return 0;
    }
    return adc1_get_raw(channel);
}

inline void analogWrite_hizli(uint8_t pin, uint8_t dutyCycle)
{
    static bool initialized = false;
    static const int freq = 5000;
    static const int ledChannel = 0;
    static const int resolution = 8;

    if (!initialized)
    {
        ledcSetup(ledChannel, freq, resolution);
        ledcAttachPin(pin, ledChannel);
        initialized = true;
    }
    ledcWrite(ledChannel, dutyCycle);
}

#endif
