#pragma once
#include "__YAPI/__ortak.h"

#if defined(ARDUINO_AVR_UNO)
#define _MAX_KART_PIN 20
#elif defined(ARDUINO_AVR_NANO)
#define _MAX_KART_PIN 20
#elif defined(ARDUINO_AVR_MEGA2560)
#define _MAX_KART_PIN 54
#elif defined(ESP8266)
#define _MAX_KART_PIN 11
#elif defined(ESP32)
#define _MAX_KART_PIN 34
#elif defined(STM32F103xB)
#define _MAX_KART_PIN 37
#elif defined(ARDUINO_SAMD_ZERO)
#define _MAX_KART_PIN 22
#elif defined(ARDUINO_ARCH_SAMD)
#define _MAX_KART_PIN 22
#elif defined(ARDUINO_ARCH_STM32)
#define _MAX_KART_PIN 37
#else
#define _MAX_KART_PIN 0
#endif

class modul
{
private:
    friend class L298N;
    friend class Ntc;
    friend class Ptc;
    friend class Led;

public:
    class L298n
    {
    private:
        uint8_t _pin1 = 0xFF;
        uint8_t _pin2 = 0xFF;
        uint8_t _pin3 = 0xFF;
        uint8_t _pin4 = 0xFF;
        uint8_t _pinPWM_A = 0xFF;
        uint8_t _pinPWM_B = 0xFF;
        uint8_t _sonhizA = 0;
        uint8_t _sonhizB = 0;

        friend class modul;
        modul *l298n_priv;

    public:
        void hiz_A(uint8_t hiz);
        void hiz_B(uint8_t hiz);

        class ayarla
        {
        private:
            friend class L298n;
            modul *l298nAyar_priv;

        public:
            void motorA(uint8_t pin1, uint8_t pin2, uint8_t pinPWM = 0xFF);
            void motorB(uint8_t pin3, uint8_t pin4, uint8_t pinPWM = 0xFF);
        } ayarla;
        class motor
        {
        private:
            friend class L298n;
            modul *l298nMotor_priv;

        public:
            void ileriA();
            void geriA();
            void durA();
            void ileriB();
            void geriB();
            void durB();
        } motor;
        class otonomMotor
        {
        private:
            unsigned long gecen_zaman = 0;
            const uint8_t g_ms = 10;
            friend class modul;
            modul *l298nOtonom_priv;

        public:
            void ileri_A(uint16_t hareket_ms, bool &aktiflik);
            void geri_A(uint16_t hareket_ms, bool &aktiflik);
            void ileri_B(uint16_t hareket_ms, bool &aktiflik);
            void geri_B(uint16_t hareket_ms, bool &aktiflik);
            void dur_A(uint16_t hareket_ms, bool &aktiflik);
            void dur_B(uint16_t hareket_ms, bool &aktiflik);
        } otonomMotor;
    } L298n;
    class Ntc
    {
    private:
        friend class modul;
        modul *NTC_priv;
        static const uint8_t max_kalibre_veri = 10;
        uint8_t veri_syc = 0;
        uint32_t direnc_degerler[max_kalibre_veri];
        int8_t sicaklik_degerler[max_kalibre_veri];
        float Rref;
        uint16_t adc_max;

    public:
        void kalibre_ekle(uint32_t ohm_degeri, int8_t sicaklik_degeri);
        void kur(double referansDirenc, uint16_t adcMaksimum);
        double direnc_oku(int adc);
        double sicaklik_oku_C(int adc);
        double sicaklik_oku_K(int adc);
        double sicaklik_oku_F(int adc);
    } Ntc;
    class Ptc
    {
    private:
        friend class modul;
        modul *NTC_priv;
        static const uint8_t max_kalibre_veri = 10;
        uint8_t veri_syc = 0;
        uint32_t direnc_degerler[max_kalibre_veri];
        int8_t sicaklik_degerler[max_kalibre_veri];
        float Rref;
        uint16_t adc_max;

    public:
        void kalibre_ekle(uint32_t ohm_degeri, int8_t sicaklik_degeri);
        void kur(double referansDirenc, uint16_t adcMaksimum);
        double direnc_oku(int adc);
        double sicaklik_oku_C(int adc);
        double sicaklik_oku_K(int adc);
        double sicaklik_oku_F(int adc);
    } Ptc;
    class Led
    {
    private:
        friend class modul;
        modul *Led_priv;
        uint8_t _atanan_pin_sayisi = 0; // matriste bulunan pin
        uint8_t _pinler[_MAX_KART_PIN]; // atanmis pinler (3,9,7,5,...) kullanıcıya baglı
        uint8_t _pinn;

    public:
        void kur(uint8_t pin_);
        void yak(uint8_t pin_ = 255, uint8_t pwm_deger);
        void sondur(uint8_t pin_ = 255);
        void degis(uint8_t pin_ = 255);

        class matris
        {
        private:
            friend class Led;
            Led *matris_priv;
            bool terse_cevir = false;

        public:
            void kur(uint8_t *pinler, uint8_t pin_sayisi);
            void animasyon_dgtl(uint16_t bekleme_ms = 300);
            void animasyon_pwm(uint16_t bekleme_ms = 100);
            void animasyon_yuruyen_isik_dgtl(uint16_t bekleme_ms = 400);
            void animasyon_hepsi_artista_pwm(uint16_t bekleme_ms = 100);
            void animasyon_ciftliBlink(uint16_t bekleme_ms = 200);
        } matris;
    } Led;
    class buzzer
    {
    private:
        friend class modul;
        modul *buzzer_priv;
        uint8_t _pin;
        unsigned long gecen_zaman = 0;
        uint8_t g_ms = 10;
        uint16_t nota_basina_calma_ms = 50;
        uint8_t nota_ms_syc = 0;
        bool caliyor_mu = false;
        uint8_t asama = 0;

    public:
        void kur(uint8_t pin);
        void cal(uint16_t frekans, uint8_t pin = 255);
        void sustur(uint8_t pin = 255);
        void sureli_cal(uint16_t frekans, uint16_t calma_ms, bool &calma_izni, uint8_t pin = 255);
        void animasyon_onay(bool &izin, uint8_t pin = 255);
        void animasyon_red(bool &izin, uint8_t pin = 255);
        void animasyon_hata(bool &izin, uint8_t pin = 255);
        void animasyon_dogru(bool &izin, uint8_t pin = 255);
        void ayar_notabasi_calmaMS(uint16_t milisaniye = 50) { nota_basina_calma_ms = milisaniye; }

    } buzzer;
};