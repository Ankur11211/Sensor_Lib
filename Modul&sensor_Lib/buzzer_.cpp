#include "Moduller.h"
/// @brief buzzer kurulumu yapar
/// @param pin buzzer pini
void modul::buzzer::kur(uint8_t pin)
{
    if (pin >= _MAX_KART_PIN)
        return;
    pinmod_hizli(pin, OUTPUT);
    analogWrite_hizli(pin, 0);
    caliyor_mu = false;
}
/// @brief buzzer ile ses çalar
/// @param frekans calacagınız ton
/// @param pin buzzer pini(kurulum yaptıysanız boş kalabilir)
void modul::buzzer::cal(uint16_t frekans, uint8_t pin)
{
    if (pin == 255)
        pin = _pin;
    else if (pin >= _MAX_KART_PIN)
        return;
    caliyor_mu = true;
#if defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_AVR_UNO)
    tone(pin, frekans); // Arduino AVR ve SAMD için
#elif defined(ESP8266)
    analogWriteFreq(frekans);
    analogWrite(pin, 512); // Yaklaşık %50 duty
#elif defined(ESP32)
    ledcAttachPin(pin, 0);    // Kanal 0
    ledcSetup(0, frekans, 8); // 8-bit çözünürlük
    ledcWrite(0, 128);        // %50 duty
#elif defined(STM32F1xx)
    // STM32 için HAL kütüphaneleri veya Timer yapılandırması gerekebilir
    // Basit yazım için PWM destekli pinlerde analogWrite kullanılabilir
    analogWrite(pin, 128);
#else
    // Platform desteklenmiyor
#endif
}
/// @brief buzzeri susturur
/// @param pin buzzer pini(kurulum yaptıysanız boş kalabilir)
void modul::buzzer::sustur(uint8_t pin)
{
    if (pin == 255)
        pin = _pin;
    else if (pin >= _MAX_KART_PIN)
        return;
    caliyor_mu = false;
#if defined(ARDUINO_ARCH_AVR) || defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_AVR_UNO)
    noTone(pin);
#elif defined(ESP8266)
    analogWrite(pin, 0);
#elif defined(ESP32)
    ledcWrite(0, 0);
#elif defined(STM32F1xx)
    analogWrite(pin, 0);
#else
    // Platform desteklenmiyor
#endif
}
/// @brief buzzeri döngü içinde millis kontrollü yapıda süreli şekilde çalar veya susturur
/// @param frekans çalınacak frekans
/// @param calma_ms ne kadar süre çalınacağı
/// @param calma_izni çalmak için gerekli ek izin
/// @param pin buzzer pini(kurulum yaptıysanız boş kalabilir)
void modul::buzzer::sureli_cal(uint16_t frekans, uint16_t calma_ms, bool &calma_izni, uint8_t pin)
{
    if (pin == 255)
        pin = _pin;
    else if (pin >= _MAX_KART_PIN)
        return;
    if (calma_izni)
    {
        if (millis() - gecen_zaman >= calma_ms)
        {
            sustur(pin);
            gecen_zaman = millis();
        }
        else if (!caliyor_mu)
        {
            cal(frekans, pin);
            gecen_zaman = millis();
        }
    }
    else
    {
        sustur(pin);
    }
}
/// @brief onaylama sesi çıkarır millis tabanlıdır döngüde kullanılabilir
/// @param izin çalmak için ek izin
/// @param pin buzzer pini(kurulum yaptıysanız boş kalabilir)
void modul::buzzer::animasyon_onay(bool &izin, uint8_t pin)
{
    if (!izin)
    {
        sustur(pin);
        asama = 0;
        return;
    }
    if (pin == 255)
        pin = _pin;
    else if (pin >= _MAX_KART_PIN)
        return;

    static const uint16_t frekans = 1500;
    static const uint16_t bip_sure = 100;

    if (millis() - gecen_zaman >= bip_sure)
    {
        gecen_zaman = millis();
        switch (asama++)
        {
        case 0:
            cal(frekans, pin);
            break;
        case 1:
            sustur(pin);
            break;
        case 2:
            cal(frekans, pin);
            break;
        case 3:
            sustur(pin);
            asama = 0;
            break;
        }
    }
}

/// @brief reddetme sesi çıkarır millis tabanlıdır
/// @param izin çalmak için ek izin
/// @param pin buzzer pini(kurulum yaptıysanız boş kalabilir)
void modul::buzzer::animasyon_red(bool &izin, uint8_t pin)
{
    if (!izin)
    {
        sustur(pin);
        asama = 0;
        return;
    }
    if (pin == 255)
        pin = _pin;
    else if (pin >= _MAX_KART_PIN)
        return;

    static const uint16_t frekans = 400;
    static const uint16_t bip_sure = 600;

    if (millis() - gecen_zaman >= bip_sure)
    {
        gecen_zaman = millis();
        switch (asama++)
        {
        case 0:
            cal(frekans, pin);
            break;
        case 1:
            sustur(pin);
            asama = 0;
            break;
        }
    }
}

/// @brief hata sesi çıkarır millis tabanlıdır
/// @param izin çalmak için ek izin
/// @param pin buzzer pini(kurulum yaptıysanız boş kalabilir)
void modul::buzzer::animasyon_hata(bool &izin, uint8_t pin)
{
    if (!izin)
    {
        sustur(pin);
        asama = 0;
        return;
    }
    if (pin == 255)
        pin = _pin;
    else if (pin >= _MAX_KART_PIN)
        return;

    static const uint16_t frekans = 600;
    static const uint16_t bip_sure = 100;

    if (millis() - gecen_zaman >= bip_sure)
    {
        gecen_zaman = millis();
        switch (asama++)
        {
        case 0:
            cal(frekans, pin);
            break;
        case 1:
            sustur(pin);
            break;
        case 2:
            cal(frekans, pin);
            break;
        case 3:
            sustur(pin);
            break;
        case 4:
            cal(frekans, pin);
            break;
        case 5:
            sustur(pin);
            asama = 0;
            break;
        }
    }
}

/// @brief dogru işlem sesi çıkarır millis tabanlıdır
/// @param izin çalmak için ek izin
/// @param pin buzzer pini(kurulum yaptıysanız boş kalabilir)
void modul::buzzer::animasyon_dogru(bool &izin, uint8_t pin)
{
    if (!izin)
    {
        sustur(pin);
        asama = 0;
        return;
    }
    if (pin == 255)
        pin = _pin;
    else if (pin >= _MAX_KART_PIN)
        return;

    static const uint16_t tonlar[] = {800, 1200, 1600};
    static const uint16_t bip_sure = 150;

    if (millis() - gecen_zaman >= bip_sure)
    {
        gecen_zaman = millis();
        if (asama < 3)
        {
            cal(tonlar[asama], pin);
            asama++;
        }
        else
        {
            sustur(pin);
            asama = 0;
        }
    }
}
