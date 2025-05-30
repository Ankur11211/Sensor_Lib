# Modül Kütüphanesi

Bu kütüphane, Arduino ve benzeri mikrodenetleyici platformları için çeşitli modüllerin (L298n motor sürücü, NTC/PTC sıcaklık sensörleri, LED'ler ve Buzzer) kullanımını kolaylaştıran fonksiyonlar içerir.

## İçerik

- [Modül Kütüphanesi](#modül-kütüphanesi)
  - [İçerik](#i̇çerik)
  - [Genel Kullanım](#genel-kullanım)
  - [L298n Motor Sürücü Modülü](#l298n-motor-sürücü-modülü)
    - [Kurulum](#kurulum)
      - `motorA(uint8_t pin1, uint8_t pin2, uint8_t pinPWM = 0xFF)`
      - `motorB(uint8_t pin3, uint8_t pin4, uint8_t pinPWM = 0xFF)`
    - [Hız Kontrolü](#hız-kontrolü)
      - `hiz_A(uint8_t hiz)`
      - `hiz_B(uint8_t hiz)`
    - [Motor Hareket Kontrolü](#motor-hareket-kontrolü)
      - `ileriA()`
      - `geriA()`
      - `durA()`
      - `ileriB()`
      - `geriB()`
      - `durB()`
    - [Otonom Motor Hareketleri (Zaman Bazlı)](#otonom-motor-hareketleri-zaman-bazlı)
      - `ileri_A(uint16_t hareket_ms, bool &aktiflik)`
      - `geri_A(uint16_t hareket_ms, bool &aktiflik)`
      - `ileri_B(uint16_t hareket_ms, bool &aktiflik)`
      - `geri_B(uint16_t hareket_ms, bool &aktiflik)`
      - `dur_A(uint16_t hareket_ms, bool &aktiflik)`
      - `dur_B(uint16_t hareket_ms, bool &aktiflik)`
  - [NTC Sıcaklık Sensörü Modülü](#ntc-sıcaklık-sensörü-modülü)
    - [Kurulum ve Kalibrasyon](#kurulum-ve-kalibrasyon)
      - `kur(double referansDirenc, uint16_t adcMaksimum)`
      - `kalibre_ekle(uint32_t ohm_degeri, int8_t sicaklik_degeri)`
    - [Sıcaklık Okuma](#sıcaklık-okuma)
      - `direnc_oku(int adc)`
      - `sicaklik_oku_C(int adc)`
      - `sicaklik_oku_K(int adc)`
      - `sicaklik_oku_F(int adc)`
  - [PTC Sıcaklık Sensörü Modülü](#ptc-sıcaklık-sensörü-modülü)
    - [Kurulum ve Kalibrasyon](#kurulum-ve-kalibrasyon-1)
      - `kur(double referansDirenc, uint16_t adcMaksimum)`
      - `kalibre_ekle(uint32_t ohm_degeri, int8_t sicaklik_degeri)`
    - [Sıcaklık Okuma](#sıcaklık-okuma-1)
      - `direnc_oku(int adc)`
      - `sicaklik_oku_C(int adc)`
      - `sicaklik_oku_K(int adc)`
      - `sicaklik_oku_F(int adc)`
  - [LED Modülü](#led-modülü)
    - [Tekil LED Fonksiyonları](#tekil-led-fonksiyonları)
      - `kur(uint8_t pin_)`
      - `yak(uint8_t pin_ = 255, uint8_t pwm_deger)`
      - `sondur(uint8_t pin_ = 255)`
      - `degis(uint8_t pin_ = 255)`
    - [LED Matris Fonksiyonları](#led-matris-fonksiyonları)
      - `kur(uint8_t *pinler, uint8_t pin_sayisi)`
      - `animasyon_dgtl(uint16_t bekleme_ms = 300)`
      - `animasyon_pwm(uint16_t bekleme_ms = 100)`
      - `animasyon_yuruyen_isik_dgtl(uint16_t bekleme_ms = 400)`
      - `animasyon_hepsi_artista_pwm(uint16_t bekleme_ms = 100)`
      - `animasyon_ciftliBlink(uint16_t bekleme_ms = 200)`
  - [Buzzer Modülü](#buzzer-modülü)
    - [Kurulum](#kurulum-1)
      - `kur(uint8_t pin)`
    - [Ses Çalma ve Durdurma](#ses-çalma-ve-durdurma)
      - `cal(uint16_t frekans, uint8_t pin = 255)`
      - `sustur(uint8_t pin = 255)`
      - `sureli_cal(uint16_t frekans, uint16_t calma_ms, bool &calma_izni, uint8_t pin = 255)`
    - [Animasyon Sesleri](#animasyon-sesleri)
      - `animasyon_onay(bool &izin, uint8_t pin = 255)`
      - `animasyon_red(bool &izin, uint8_t pin = 255)`
      - `animasyon_hata(bool &izin, uint8_t pin = 255)`
      - `animasyon_dogru(bool &izin, uint8_t pin = 255)`
    - [Ayarlar](#ayarlar)
      - `ayar_notabasi_calmaMS(uint16_t milisaniye = 50)`

## Genel Kullanım

Kütüphaneyi kullanmak için `Moduller.h` dosyasını projenize dahil etmeniz gerekmektedir. Tüm modüller `modul` sınıfı altında yer almaktadır.

```cpp
#include "Moduller.h"

modul myModule; // modul sınıfından bir nesne oluşturun

void setup() {
  // Modül fonksiyonlarını buradan çağırabilirsiniz
}

void loop() {
  // Modül fonksiyonlarını buradan çağırabilirsiniz
}
