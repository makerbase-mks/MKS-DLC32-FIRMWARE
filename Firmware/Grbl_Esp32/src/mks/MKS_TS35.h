#ifndef __MKS_TS35_H
#define __MKS_TS35_H

#include "../Grbl.h"
#include "SPI.h" 
#include "lvgl.h"
#include "TFT_eSPI.h"

#define LCD_WIDTH               480
#define LCD_HEIGHT              320

#define	CHX 	                  0x90
#define	CHY 	                  0xD0


#define TFT_COLOR_RED		                0xF800
#define TFT_COLOR_GREEN		              0x07E0
#define TFT_COLOR_BLUE		              0x001F
#define TFT_COLOR_BLACK		              0x0000
#define TFT_COLOR_WHITE		              0xFFFF
#define TFT_COLOR_YELLOW                0xFFE0

#define TFT_LCD_CS_H        digitalWrite(LCD_CS, HIGH)
#define TFT_LCD_CS_L        digitalWrite(LCD_CS, LOW)

#define TFT_TOUCH_CS_H      digitalWrite(TOUCH_CS, HIGH)
#define TFT_TOUCH_CS_L      digitalWrite(TOUCH_CS, LOW)

#if defined(USE_BEEP)
#define BEEP_ON             digitalWrite(BEEPER, HIGH)
#define BEEP_OFF            digitalWrite(BEEPER, LOW)
#else 
#define BEEP_ON            
#define BEEP_OFF           
#endif

#ifdef USE_VERSION_003
#define LCD_BLK_ON          digitalWrite(LCD_EN, HIGH)
#define LCD_BLK_OFF         digitalWrite(LCD_EN, LOW)
#else
#define LCD_BLK_ON          digitalWrite(LCD_EN, LOW)
#define LCD_BLK_OFF         digitalWrite(LCD_EN, HIGH)
#endif

// LCD Driver
extern TFT_eSPI tft;

void tft_TS35_init();
void tft_TS35_SPI_begin(void);
void ts35_beep_init();
void ts35_beep_on(void);
void ts35_beep_off(void);

void bsp_led_init(void);
void ts35_set_flag(bool flag);

void bsp_led_run(void);


#endif

