#ifndef __mks_draw_language_h
#define __mks_draw_language_h

#include "MKS_draw_lvgl.h"
#include "MKS_LVGL.h"


typedef struct {

    lv_obj_t *imgbtn_back;
    lv_obj_t *imgbtn_simple_cn;         // 简体中文
    lv_obj_t *imgbtn_en;                // 英语
    lv_obj_t *imgbtn_de;                // 德语

    lv_obj_t *label_back;
    lv_obj_t *label_simple_cn;         // 简体中文
    lv_obj_t *label_en;                // 英语
    lv_obj_t *label_de;                // 德语


}LANGUAGE_PAGE_T;


typedef struct {

    /* 公共 */
    const char *back;
    const char *yes;
    const char *no;

    /* 主页 */
    const char *control;
    const char *sculpture;
    const char *tool;
    const char *Mpos;
    const char *Wpos;

    const char *wifi_connect;
    const char *wifi_disconnect;

    /* 控制界面 */
    const char *xy_clear;
    const char *z_clear;
    const char *knife;
    const char *next;
    const char *up;
    const char *cooling;
    const char *position;
    const char *speed_high;
    const char *speed_mid;
    const char *speed_low;
    const char *spindle;
    const char *carve;

    /* 文件界面 */
    const char *dis_no_sd_card;

    /* 雕刻界面 */
    const char *hold;
    const char *cycle;
    const char *stop;
    const char *adjust;
    const char *spindle_speed;
    const char *feed_rate;
    const char *rapid_speed;
    const char *carve_times;


    
    
    
    /* 提示语 */
    const char *dis_stop_print;
    const char *dis_homing;
    const char *dis_no_hard_homing;
    const char *dis_homing_succeed;
    const char *dis_homing_fail;
    const char *dis_probe_set;
    const char *dis_probe_succeed;
    const char *dis_probe_fail;
}mc_lg_muilt_t;
extern mc_lg_muilt_t mc_language;


void draw_language(void);
void mks_clear_language(void);

#endif
