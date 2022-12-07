#ifndef __MKS_CTRL_H
#define __MKS_CTRL_H

#include "../Grbl.h"
#include "MKS_LVGL.h"

/**************************************************** BLTouch ************************************************/
typedef enum {
    sp_none,    // 开始没状态
    sp_begin,   // 电机开始空闲
    sp_load,    // 装载start的时间值
    sp_wait,    // 等待时间到
    sp_end,     // 激光头关闭后
}spindle_check_status;

typedef struct {
    uint32_t                    tick_start;
    uint32_t                    tick_end;
    spindle_check_status        spindle_status;
    uint32_t                    out_time;
}mks_spindle_t;
extern mks_spindle_t mks_spindle;


/***************************************************** WIFI *************************************************/

typedef enum {

    wifi_none,
    wifi_scanf_begin,
    wifi_scanf_waitting,
    wifi_scanf_succeed,
    wifi_scanf_fail,
    wifi_connecting,
    wifi_disconnecting,

}mks_wifi_status_t;


#if defined(ENABLE_WIFI)
#define MKS_WIFI_NUM            16
#define MKS_WIFI_NAME_LEN       128
#define MKS_WIFI_DIS_PAGE       2
typedef struct {
    char wifi_name_str[MKS_WIFI_NUM][MKS_WIFI_NAME_LEN];
    int32_t wifi_rssi[MKS_WIFI_NUM];    // wifi信号强度
    uint8_t begin_scanf_num;        // 每次记录都从0开始    
    uint8_t wifi_show_page;         // 记录WiFi显示的页码   
    mks_wifi_status_t wifi_scanf_status;    
    uint8_t wifi_choose;  
    char wifi_name_connect[MKS_WIFI_NAME_LEN];
}MKS_WIFI_t;
extern MKS_WIFI_t mks_wifi;
#endif


// class SD_UPDATA {

// private:
    
// public:

// };













void bltouch_init(void);
void mks_motor_move(void);
void bltouch_duty(uint32_t duty);
void BLTOUCH_push_down(void);
void BLTOUCH_push_up(void);
void BLtouch_reset_and_push_up(void);
void bltouch_reset(void);

void spindle_check_init(void);
void spindle_check(void);


void mks_wifi_scanf(void);
void mks_wifi_connect(void);
bool mks_get_wifi_status(void);
bool mks_wifi_check_is_out(bool flag);
#endif
