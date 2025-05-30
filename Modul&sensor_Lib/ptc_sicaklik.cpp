#include "Moduller.h"
#include <math.h>

/// @brief PTC kalibrasyon verisi ekler (artan direnç sıralı yerleştirir).
/// @param ohm_degeri Direnç değeri (ohm cinsinden)
/// @param sicaklik_degeri Sıcaklık değeri (°C)
void modul::Ptc::kalibre_ekle(uint32_t ohm_degeri, int8_t sicaklik_degeri)
{
    if (veri_syc >= max_kalibre_veri)
        return;

    int i = veri_syc - 1;
    while (i >= 0 && direnc_degerler[i] > ohm_degeri)
    {
        direnc_degerler[i + 1] = direnc_degerler[i];
        sicaklik_degerler[i + 1] = sicaklik_degerler[i];
        i--;
    }
    direnc_degerler[i + 1] = ohm_degeri;
    sicaklik_degerler[i + 1] = sicaklik_degeri;
    veri_syc++;
}

/// @brief PTC yapılandırmasını başlatır.
/// @param referansDirenc Referans direnç değeri (ohm cinsinden)
/// @param adcMaksimum ADC çözünürlüğü (ör: 1023 ya da 4095)
void modul::Ptc::kur(double referansDirenc, uint16_t adcMaksimum)
{
    Rref = referansDirenc;
    adc_max = adcMaksimum;
    veri_syc = 0;
}

/// @brief ADC değerinden PTC sensörünün direnç değerini hesaplar.
/// @param adc ADC okuması (0 < adc <= adc_max)
/// @return PTC'nin direnç değeri (ohm), geçersizse NAN
double modul::Ptc::direnc_oku(int adc)
{
    if (adc <= 0)
        return NAN;
    return Rref * ((double)adc_max / adc - 1.0);
}

/// @brief PTC sıcaklık okumasını Santigrat cinsinden yapar.
/// @param adc ADC değeri
/// @return Sıcaklık (°C), geçersizse NAN
double modul::Ptc::sicaklik_oku_C(int adc)
{
    if (adc <= 0 || veri_syc < 2)
        return NAN;

    double Rptc = direnc_oku(adc);

    if (Rptc <= direnc_degerler[0])
        return sicaklik_degerler[0];
    if (Rptc >= direnc_degerler[veri_syc - 1])
        return sicaklik_degerler[veri_syc - 1];

    for (int i = 0; i < veri_syc - 1; i++)
    {
        if (Rptc >= direnc_degerler[i] && Rptc <= direnc_degerler[i + 1])
        {
            double R0 = direnc_degerler[i];
            double R1 = direnc_degerler[i + 1];
            double T0 = sicaklik_degerler[i];
            double T1 = sicaklik_degerler[i + 1];
            double oran = (Rptc - R0) / (R1 - R0);
            return T0 + oran * (T1 - T0);
        }
    }
    return NAN;
}

/// @brief PTC sıcaklık okumasını Kelvin cinsinden yapar.
/// @param adc ADC değeri
/// @return Sıcaklık (K), geçersizse NAN
double modul::Ptc::sicaklik_oku_K(int adc)
{
    double c = sicaklik_oku_C(adc);
    if (isnan(c))
        return NAN;
    return c + 273.15;
}

/// @brief PTC sıcaklık okumasını Fahrenheit cinsinden yapar.
/// @param adc ADC değeri
/// @return Sıcaklık (°F), geçersizse NAN
double modul::Ptc::sicaklik_oku_F(int adc)
{
    double c = sicaklik_oku_C(adc);
    if (isnan(c))
        return NAN;
    return c * 9.0 / 5.0 + 32.0;
}
