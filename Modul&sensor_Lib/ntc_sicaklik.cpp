#include "Moduller.h"
#include <math.h>

#define TEST_DEBUG 0

/**
 * @brief NTC sensörü kalibrasyonu için direnç-sıcaklık çifti ekler.
 *
 * @param ohm_degeri NTC'nin bilinen sıcaklıktaki direnç değeri (ohm cinsinden).
 * @param sicaklik_degeri Direnç değerinin ölçüldüğü sıcaklık (°C).
 */
void modul::Ntc::kalibre_ekle(uint32_t ohm_degeri, int8_t sicaklik_degeri)
{
    if (veri_syc >= max_kalibre_veri)
        return;

    int i = veri_syc - 1;
    while (i >= 0 && direnc_degerler[i] < ohm_degeri)
    {
        direnc_degerler[i + 1] = direnc_degerler[i];
        sicaklik_degerler[i + 1] = sicaklik_degerler[i];
        i--;
    }
    direnc_degerler[i + 1] = ohm_degeri;
    sicaklik_degerler[i + 1] = sicaklik_degeri;
    veri_syc++;

#if TEST_DEBUG
    Serial.print("Kalibrasyon eklendi: Direnc=");
    Serial.print(ohm_degeri);
    Serial.print(" Ohm, Sicaklik=");
    Serial.print(sicaklik_degeri);
    Serial.println(" C");
#endif
}

/**
 * @brief NTC sistemi başlatılır. Referans direnç ve ADC maksimum değeri atanır.
 *
 * @param referansDirenc Seriye bağlı bilinen sabit direnç (ohm).
 * @param adcMaksimum ADC'nin ulaşabileceği maksimum değer (örneğin 1023 veya 4095).
 */
void modul::Ntc::kur(double referansDirenc, uint16_t adcMaksimum)
{
    Rref = referansDirenc;
    adc_max = adcMaksimum;
    veri_syc = 0;

#if TEST_DEBUG
    Serial.print("NTC kuruldu: Rref=");
    Serial.print(Rref);
    Serial.print(" Ohm, adc_max=");
    Serial.println(adc_max);
#endif
}

/**
 * @brief Verilen ADC değeriyle NTC direncini hesaplar.
 *
 * @param adc ADC'den okunan dijital değer.
 * @return double Hesaplanan NTC direnç değeri (ohm).
 */
double modul::Ntc::direnc_oku(int adc)
{
    if (adc <= 0)
        return NAN;

    double Rntc = Rref * ((double)adc_max / adc - 1.0);

#if TEST_DEBUG
    Serial.print("ADC=");
    Serial.print(adc);
    Serial.print(", Hesaplanan NTC direnci=");
    Serial.print(Rntc);
    Serial.println(" Ohm");
#endif

    return Rntc;
}

/**
 * @brief ADC değeri ile sıcaklık ölçümü yapar (°C).
 *
 * @param adc ADC'den okunan dijital değer.
 * @return double Sıcaklık değeri Celsius (°C) cinsinden.
 */
double modul::Ntc::sicaklik_oku_C(int adc)
{
    if (adc <= 0 || veri_syc < 2)
        return NAN;

    double Rntc = direnc_oku(adc);

    if (Rntc >= direnc_degerler[0])
        return sicaklik_degerler[0];
    if (Rntc <= direnc_degerler[veri_syc - 1])
        return sicaklik_degerler[veri_syc - 1];

    for (int i = 0; i < veri_syc - 1; i++)
    {
        if (Rntc <= direnc_degerler[i] && Rntc >= direnc_degerler[i + 1])
        {
            double R0 = direnc_degerler[i];
            double R1 = direnc_degerler[i + 1];
            double T0 = sicaklik_degerler[i];
            double T1 = sicaklik_degerler[i + 1];
            double oran = (Rntc - R1) / (R0 - R1);
            double sicaklik = T1 + oran * (T0 - T1);

#if TEST_DEBUG
            Serial.print("Interpolasyon: Rntc=");
            Serial.print(Rntc);
            Serial.print(", Aralık: [");
            Serial.print(R0);
            Serial.print(", ");
            Serial.print(R1);
            Serial.print("], Sicaklik: ");
            Serial.println(sicaklik);
#endif

            return sicaklik;
        }
    }
    return NAN;
}

/**
 * @brief ADC değeri ile sıcaklık ölçümü yapar (Kelvin).
 *
 * @param adc ADC'den okunan dijital değer.
 * @return double Sıcaklık değeri Kelvin (K) cinsinden.
 */
double modul::Ntc::sicaklik_oku_K(int adc)
{
    double c = sicaklik_oku_C(adc);
    if (isnan(c))
        return NAN;
    return c + 273.15;
}

/**
 * @brief ADC değeri ile sıcaklık ölçümü yapar (Fahrenheit).
 *
 * @param adc ADC'den okunan dijital değer.
 * @return double Sıcaklık değeri Fahrenheit (°F) cinsinden.
 */
double modul::Ntc::sicaklik_oku_F(int adc)
{
    double c = sicaklik_oku_C(adc);
    if (isnan(c))
        return NAN;
    return c * 9.0 / 5.0 + 32.0;
}