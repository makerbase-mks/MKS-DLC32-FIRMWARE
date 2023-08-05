#include "MKS_TS35.h"

#if defined(LCD_EN)
TFT_eSPI tft = TFT_eSPI(); 

void tft_LCD_Fill() {
    tft.fillRect(0, 0,480, 320, TFT_COLOR_RED);
}

void tft_TS35_init() {
    tft.begin();
    tft.setRotation(1);
    // tft.fillScreen(tft.color565(0X1A, 0X1A, 0X1A));
    tft.initDMA();
    delay_ms(100);
}   
#endif

void ts35_beep_init() { 
#if defined(BEEPER)
    pinMode(BEEPER, OUTPUT);
    digitalWrite(BEEPER, LOW);
#endif
}

void ts35_beep_on(void) {
#if defined(BEEPER)
    if(beep_status->get()) digitalWrite(BEEPER, HIGH);
#endif
}

void ts35_beep_off(void) {
#if defined(BEEPER)
    digitalWrite(BEEPER, LOW);
#endif
}

#define LED_RESOLUTION      10
#define LED_CHANNEL         14
void bsp_led_init(void) { 
#ifdef LED_PIN
    ledcSetup(LED_CHANNEL, 1000, LED_RESOLUTION); // 设置通道14 ,分辨率为10（2^10 = 1024）
    ledcAttachPin(LED_PIN, LED_CHANNEL);  // 将通道与对应的引脚连接
#endif
}

void bsp_set_duty(uint32_t duty) {
#ifdef LED_PIN
    ledcWrite(LED_CHANNEL, duty);
#endif
}

// 0-255
void bsp_led_pwm_speed(uint8_t speed) {
    uint32_t duty = 1024;
    duty = (duty * (255 - speed)) / 255;
    bsp_set_duty(duty);
}


uint32_t led_count = 0;;
uint8_t led_dir_flag = 1;
void bsp_led_run(void) {

    if(led_dir_flag) {
        led_count = led_count + 15;
        if(led_count >= 255) {
            led_dir_flag = 0;
        }
    }else{
        led_count = led_count - 15;
        if(led_count == 0) {
            led_dir_flag = 1;
        }
    }
    bsp_led_pwm_speed(led_count);
}










