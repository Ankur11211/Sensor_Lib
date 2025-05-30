#include "Moduller.h"

/**
 * @brief LED'i verilen pine kurar.
 *
 * Bu fonksiyon, belirtilen pini çıkış olarak ayarlar.
 * Eğer verilen pin `_MAX_KART_PIN` değerinden büyükse işlem yapmaz.
 *
 * @param pin_ Kurulacak pin numarası.
 */
inline void modul::Led::kur(uint8_t pin_)
{
    _pinn = pin_;
    if (pin_ >= _MAX_KART_PIN)
        return;
    pinmod_hizli(pin_, OUTPUT);
}

/**
 * @brief LED matrisini verilen pinler dizisi ile kurar.
 *
 * Bu fonksiyon, birden fazla pin için LED matrisini başlatır.
 * Her bir pin `_MAX_KART_PIN` kontrolüne tabi tutulur.
 *
 * @param pinler Pin numaralarını içeren dizinin adresi.
 * @param pin_sayisi Dizideki toplam pin sayısı.
 */
inline void modul::Led::matris::kur(uint8_t *pinler, uint8_t pin_sayisi)
{
    matris_priv->_atanan_pin_sayisi = 0;
    for (size_t y = 0; y < pin_sayisi; y++)
    {
        uint8_t pin_ = pinler[y];
        if (pin_ >= _MAX_KART_PIN)
            continue;
        matris_priv->_pinler[y] = pin_;
        matris_priv->_atanan_pin_sayisi++;
        pinmod_hizli(pin_, OUTPUT);
    }
}

/**
 * @brief Belirtilen pini PWM değeriyle açar.
 *
 * PWM değeri 0 ise pin LOW yapılır. PWM değeri belirtilirse `analogWrite` kullanılır.
 *
 * @param pin_ Açılacak pin numarası.Bos bırakabilirsiniz
 * @param pwm_deger PWM değeri (0-255 arası).
 */
inline void modul::Led::yak(uint8_t pin_, uint8_t pwm_deger)
{
    if (pin_ == 255)
        pin_ = _pinn;
    else if (pin_ >= _MAX_KART_PIN)
        return;
    if (pwm_deger != 0)
        dgwrite_hizli(pin_, 1);
    else
        analogWrite_hizli(pin_, pwm_deger);
}

/**
 * @brief Belirtilen pini LOW seviyesine çeker.
 *
 * @param pin_ Kapatılacak pin numarası.Bos bırakabilirsiniz
 */
inline void modul::Led::sondur(uint8_t pin_)
{
    if (pin_ == 255)
        pin_ = _pinn;
    else if (pin_ >= _MAX_KART_PIN)
        return;
    dgwrite_hizli(pin_, 0);
}

/**
 * @brief Belirtilen pinin durumunu değiştirir.
 *
 * Pin HIGH ise LOW yapılır, LOW ise HIGH yapılır.
 *
 * @param pin_ Durumu değiştirilecek pin numarası.Boş bırakabilirsiniz
 */
inline void modul::Led::degis(uint8_t pin_)
{
    if (pin_ == 255)
        pin_ = _pinn;
    else if (pin_ >= _MAX_KART_PIN)
        return;
    dgwrite_hizli(pin_, !dgread_hizli(pin_));
}

/**
 * @brief LED matrisinde dijital animasyon oynatır.
 *
 * Terse ya da düz sırayla LED'leri sırasıyla yakıp söndürür.
 *
 * @param bekleme_ms Her LED arasında bekleme süresi (ms).
 */
void modul::Led::matris::animasyon_dgtl(uint16_t bekleme_ms)
{
    if (terse_cevir)
    {
        for (int i = matris_priv->_atanan_pin_sayisi - 1; i >= 0; i--)
        {
            uint8_t pin_ = matris_priv->_pinler[i];
            pinmod_hizli(pin_, OUTPUT);
            dgwrite_hizli(pin_, HIGH);
            delay(bekleme_ms);
            dgwrite_hizli(pin_, LOW);
        }
    }
    else
    {
        for (uint8_t i = 0; i < matris_priv->_atanan_pin_sayisi; i++)
        {
            uint8_t pin_ = matris_priv->_pinler[i];
            pinmod_hizli(pin_, OUTPUT);
            dgwrite_hizli(pin_, HIGH);
            delay(bekleme_ms);
            dgwrite_hizli(pin_, LOW);
        }
    }
    terse_cevir = !terse_cevir;
}
/**
 * @brief LED matrisinde PWM ile animasyon oynatır.
 *
 * Ters ya da düz yönde her LED için PWM değeri artırılıp azaltılır.
 *
 * @param bekleme_ms Bir LED için tüm PWM süreci boyunca toplam bekleme süresi (ms).
 *
 * @note PWM yumuşak parlama efektleri için kullanılabilir.
 */
void modul::Led::matris::animasyon_pwm(uint16_t bekleme_ms)
{
    if (terse_cevir)
    {
        for (int i = matris_priv->_atanan_pin_sayisi - 1; i >= 0; i--)
        {
            uint8_t pin_ = matris_priv->_pinler[i];
            pinmod_hizli(pin_, OUTPUT);
            for (uint8_t pwm_val = 0; pwm_val <= 255; pwm_val++)
            {
                analogWrite_hizli(pin_, pwm_val);
                delay(bekleme_ms / 255);
            }
            for (int pwm_val = 255; pwm_val >= 0; pwm_val--)
            {
                analogWrite_hizli(pin_, pwm_val);
                delay(bekleme_ms / 255);
            }
            analogWrite_hizli(pin_, 0);
        }
    }
    else
    {
        for (uint8_t i = 0; i < matris_priv->_atanan_pin_sayisi; i++)
        {
            uint8_t pin_ = matris_priv->_pinler[i];
            pinmod_hizli(pin_, OUTPUT);
            for (uint8_t pwm_val = 0; pwm_val <= 255; pwm_val++)
            {
                analogWrite_hizli(pin_, pwm_val);
                delay(bekleme_ms / 255);
            }
            for (int pwm_val = 255; pwm_val >= 0; pwm_val--)
            {
                analogWrite_hizli(pin_, pwm_val);
                delay(bekleme_ms / 255);
            }
            analogWrite_hizli(pin_, 0);
        }
    }
    terse_cevir = !terse_cevir;
}

/**
 * @brief Dijital yürüyen ışık animasyonu.
 *
 * Tüm LED’leri sırayla yakıp sadece biri açık kalacak şekilde yürüyen ışık efekti uygular.
 *
 * @param bekleme_ms Her adımda bekleme süresi (ms).
 */
void modul::Led::matris::animasyon_yuruyen_isik_dgtl(uint16_t bekleme_ms)
{
    if (terse_cevir)
    {
        for (int i = matris_priv->_atanan_pin_sayisi - 1; i >= 0; i--)
        {
            for (uint8_t j = 0; j < matris_priv->_atanan_pin_sayisi; j++)
            {
                dgwrite_hizli(matris_priv->_pinler[j], j == i ? HIGH : LOW);
            }
            delay(bekleme_ms);
        }
    }
    else
    {
        for (uint8_t i = 0; i < matris_priv->_atanan_pin_sayisi; i++)
        {
            for (uint8_t j = 0; j < matris_priv->_atanan_pin_sayisi; j++)
            {
                dgwrite_hizli(matris_priv->_pinler[j], j == i ? HIGH : LOW);
            }
            delay(bekleme_ms);
        }
    }
}

/**
 * @brief Tüm LED’leri PWM ile birlikte senkron bir şekilde artırıp azaltır.
 *
 * Matrisdeki tüm LED’lerin parlaklığı birlikte yavaşça artar ve azalır.
 *
 * @param bekleme_ms Tam bir artış veya azalış için gereken toplam süre (ms).
 */
void modul::Led::matris::animasyon_hepsi_artista_pwm(uint16_t bekleme_ms)
{
    for (uint8_t val = 0; val <= 255; val++)
    {
        for (uint8_t i = 0; i < matris_priv->_atanan_pin_sayisi; i++)
        {
            analogWrite_hizli(matris_priv->_pinler[i], val);
        }
        delay(bekleme_ms / 255);
    }
    for (int val = 255; val >= 0; val--)
    {
        for (uint8_t i = 0; i < matris_priv->_atanan_pin_sayisi; i++)
        {
            analogWrite_hizli(matris_priv->_pinler[i], val);
        }
        delay(bekleme_ms / 255);
    }
}

/**
 * @brief Çiftli LED blink (yanıp sönme) animasyonu.
 *
 * Matristen sırayla çift ve tek indexli pinler yakılıp söndürülür.
 *
 * @param bekleme_ms Her yak/söndür döngüsü için bekleme süresi (ms).
 *
 * @note 5 tekrar döngüsü uygular.
 */
void modul::Led::matris::animasyon_ciftliBlink(uint16_t bekleme_ms)
{
    for (int cycle = 0; cycle < 5; cycle++)
    {
        for (uint8_t i = 0; i < matris_priv->_atanan_pin_sayisi; i++)
        {
            bool on = (i % 2 == 0);
            dgwrite_hizli(matris_priv->_pinler[i], on);
        }
        delay(bekleme_ms);
        for (uint8_t i = 0; i < matris_priv->_atanan_pin_sayisi; i++)
        {
            bool on = (i % 2 == 1);
            dgwrite_hizli(matris_priv->_pinler[i], on);
        }
        delay(bekleme_ms);
    }
}
