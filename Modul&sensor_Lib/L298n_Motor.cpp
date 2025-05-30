#include "Moduller.h"

// #define TEST_

void modul::L298n::ayarla::motorA(uint8_t pin1, uint8_t pin2, uint8_t pinPWM)
{
#ifdef TEST_
    Serial.println(F("motorA ayarına başlandı"));
#endif

    l298nAyar_priv->L298n._pinPWM_A = pinPWM;
    l298nAyar_priv->L298n._pin1 = pin1;
    l298nAyar_priv->L298n._pin2 = pin2;

    if (pinPWM != 0xFF)
    {
#ifdef TEST_
        Serial.println(F("PWM pini OUTPUT moduna ayarlanıyor"));
#endif
        pinmod_hizli(pinPWM, OUTPUT);
    }
    pinmod_hizli(pin1, OUTPUT);
    pinmod_hizli(pin2, OUTPUT);

    dgwrite_hizli(pin1, LOW);
    dgwrite_hizli(pin2, LOW);

#ifdef TEST_
    Serial.println(F("motorA pinleri ayarlandı ve LOW yapıldı"));
#endif
}

void modul::L298n::ayarla::motorB(uint8_t pin3, uint8_t pin4, uint8_t pinPWM)
{
#ifdef TEST_
    Serial.println(F("motorB ayarına başlandı"));
#endif

    l298nAyar_priv->L298n._pinPWM_B = pinPWM;
    l298nAyar_priv->L298n._pin3 = pin3;
    l298nAyar_priv->L298n._pin4 = pin4;

    if (pinPWM != 0xFF)
    {
#ifdef TEST_
        Serial.println(F("PWM pini OUTPUT moduna ayarlanıyor"));
#endif
        pinmod_hizli(l298nAyar_priv->L298n._pinPWM_B, OUTPUT);
    }
    pinmod_hizli(pin3, OUTPUT);
    pinmod_hizli(pin4, OUTPUT);

    dgwrite_hizli(pin3, LOW);
    dgwrite_hizli(pin4, LOW);

#ifdef TEST_
    Serial.println(F("motorB pinleri ayarlandı ve LOW yapıldı"));
#endif
}

void modul::L298n::hiz_A(uint8_t hiz)
{
#ifdef TEST_
    Serial.print(F("motor A hız ayarlanıyor: "));
    Serial.println(hiz);
#endif

    if (_pinPWM_A != 0xFF)
    {
        _sonhizA = hiz;
        analogWrite_hizli(_pinPWM_A, _sonhizA);

#ifdef TEST_
        Serial.println(F("PWM çıkışı ayarlandı"));
#endif
    }
}

void modul::L298n::motor::ileriA()
{
#ifdef TEST_
    Serial.println(F("motor A ileri"));
#endif

    dgwrite_hizli(l298nMotor_priv->L298n._pin1, HIGH);
    dgwrite_hizli(l298nMotor_priv->L298n._pin2, LOW);
}

void modul::L298n::motor::geriA()
{
#ifdef TEST_
    Serial.println(F("motor A geri"));
#endif

    dgwrite_hizli(l298nMotor_priv->L298n._pin1, LOW);
    dgwrite_hizli(l298nMotor_priv->L298n._pin2, HIGH);
}

void modul::L298n::motor::durA()
{
#ifdef TEST_
    Serial.println(F("motor A durduruldu"));
#endif

    dgwrite_hizli(l298nMotor_priv->L298n._pin1, LOW);
    dgwrite_hizli(l298nMotor_priv->L298n._pin2, LOW);
}

void modul::L298n::hiz_B(uint8_t hiz)
{
#ifdef TEST_
    Serial.print(F("motor B hız ayarlanıyor: "));
    Serial.println(hiz);
#endif

    if (_pinPWM_B != 0xFF)
    {
        _sonhizB = hiz;
        analogWrite_hizli(_pinPWM_B, _sonhizB);

#ifdef TEST_
        Serial.println(F("PWM çıkışı ayarlandı"));
#endif
    }
}

void modul::L298n::motor::ileriB()
{
#ifdef TEST_
    Serial.println(F("motor B ileri"));
#endif

    dgwrite_hizli(l298nMotor_priv->L298n._pin3, HIGH);
    dgwrite_hizli(l298nMotor_priv->L298n._pin4, LOW);
}

void modul::L298n::motor::geriB()
{
#ifdef TEST_
    Serial.println(F("motor B geri"));
#endif

    dgwrite_hizli(l298nMotor_priv->L298n._pin3, LOW);
    dgwrite_hizli(l298nMotor_priv->L298n._pin4, HIGH);
}

void modul::L298n::motor::durB()
{
#ifdef TEST_
    Serial.println(F("motor B durduruldu"));
#endif

    dgwrite_hizli(l298nMotor_priv->L298n._pin3, LOW);
    dgwrite_hizli(l298nMotor_priv->L298n._pin4, LOW);
}

void modul::L298n::otonomMotor::ileri_A(uint16_t hareket_ms, bool &aktiflik)
{
#ifdef TEST_
    Serial.print(F("Otonom motor A ileri hareket süresi (ms): "));
    Serial.println(hareket_ms);
#endif

    uint16_t syc = 0;
    unsigned long gecen_zaman = millis();

    while ((syc * g_ms < hareket_ms) && aktiflik)
    {
        if (millis() - gecen_zaman >= g_ms)
        {
            gecen_zaman = millis();
            syc++;
        }
        l298nOtonom_priv->L298n.motor.ileriA();
    }
    l298nOtonom_priv->L298n.motor.durA();

#ifdef TEST_
    Serial.println(F("Otonom motor A hareketi tamamlandı"));
#endif
}

void modul::L298n::otonomMotor::geri_A(uint16_t hareket_ms, bool &aktiflik)
{
#ifdef TEST_
    Serial.print(F("Otonom motor A geri hareket süresi (ms): "));
    Serial.println(hareket_ms);
#endif

    uint16_t syc = 0;
    unsigned long gecen_zaman = millis();

    while ((syc * g_ms < hareket_ms) && aktiflik)
    {
        if (millis() - gecen_zaman >= g_ms)
        {
            gecen_zaman = millis();
            syc++;
        }
        l298nOtonom_priv->L298n.motor.geriA();
    }
    l298nOtonom_priv->L298n.motor.durA();

#ifdef TEST_
    Serial.println(F("Otonom motor A geri hareket tamamlandı"));
#endif
}

void modul::L298n::otonomMotor::ileri_B(uint16_t hareket_ms, bool &aktiflik)
{
#ifdef TEST_
    Serial.print(F("Otonom motor B ileri hareket süresi (ms): "));
    Serial.println(hareket_ms);
#endif

    uint16_t syc = 0;
    unsigned long gecen_zaman = millis();

    while ((syc * g_ms < hareket_ms) && aktiflik)
    {
        if (millis() - gecen_zaman >= g_ms)
        {
            gecen_zaman = millis();
            syc++;
        }
        l298nOtonom_priv->L298n.motor.ileriB();
    }
    l298nOtonom_priv->L298n.motor.durB();

#ifdef TEST_
    Serial.println(F("Otonom motor B hareketi tamamlandı"));
#endif
}

void modul::L298n::otonomMotor::geri_B(uint16_t hareket_ms, bool &aktiflik)
{
#ifdef TEST_
    Serial.print(F("Otonom motor B geri hareket süresi (ms): "));
    Serial.println(hareket_ms);
#endif

    uint16_t syc = 0;
    unsigned long gecen_zaman = millis();

    while ((syc * g_ms < hareket_ms) && aktiflik)
    {
        if (millis() - gecen_zaman >= g_ms)
        {
            gecen_zaman = millis();
            syc++;
        }
        l298nOtonom_priv->L298n.motor.geriB();
    }
    l298nOtonom_priv->L298n.motor.durB();

#ifdef TEST_
    Serial.println(F("Otonom motor B geri hareket tamamlandı"));
#endif
}

void modul::L298n::otonomMotor::dur_A(uint16_t hareket_ms, bool &aktiflik)
{
#ifdef TEST_
    Serial.print(F("Otonom motor A dur hareket süresi (ms): "));
    Serial.println(hareket_ms);
#endif

    uint16_t syc = 0;
    unsigned long gecen_zaman = millis();

    while ((syc * g_ms < hareket_ms) && aktiflik)
    {
        if (millis() - gecen_zaman >= g_ms)
        {
            gecen_zaman = millis();
            syc++;
        }
        l298nOtonom_priv->L298n.motor.durA();
    }

#ifdef TEST_
    Serial.println(F("Otonom motor A durduruldu"));
#endif
}

void modul::L298n::otonomMotor::dur_B(uint16_t hareket_ms, bool &aktiflik)
{
#ifdef TEST_
    Serial.print(F("Otonom motor B dur hareket süresi (ms): "));
    Serial.println(hareket_ms);
#endif

    uint16_t syc = 0;
    unsigned long gecen_zaman = millis();

    while ((syc * g_ms < hareket_ms) && aktiflik)
    {
        if (millis() - gecen_zaman >= g_ms)
        {
            gecen_zaman = millis();
            syc++;
        }
        l298nOtonom_priv->L298n.motor.durB();
    }

#ifdef TEST_
    Serial.println(F("Otonom motor B durduruldu"));
#endif
}
