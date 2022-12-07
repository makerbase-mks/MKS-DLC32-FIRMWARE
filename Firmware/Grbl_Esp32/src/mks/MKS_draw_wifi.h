#ifndef __mks_draw_wifi_h
#define __mks_draw_wifi_h

#include "MKS_draw_lvgl.h"
#include "../WebUI/WifiConfig.h"
#if defined(ENABLE_WIFI)

extern char wifi_send_username[128];
extern char wifi_send_password[128];

typedef enum {

    wifi_src1_x = 10,
    wifi_src1_y = 10,
    wifi_src1_x_size = 460,
    wifi_src1_y_size = 90,

    wifi_wifi_popup_x = 75,
    wifi_wifi_popup_y = 70,
    wifi_wifi_popup_x_size = 330,
    wifi_wifi_popup_y_size = 180,


    wifi_src_kb_x = 1,
    wifi_src_kb_y = 1,
    wifi_src_kb_x_size = 478,
    wifi_src_kb_y_size = 318,

    wifi_first_line_x = 10,
    wifi_first_line_y = 150,

    wifi_first_btn_x = 10,
    wifi_first_btn_y = 120,
    wifi_btn_w = 220,
    wifi_btn_h = 30,
    wifi_btn_label_size = 220,

    wifi_popup_btn_sure_x = 10,
    wifi_popup_btn_sure_y = 120,
    wifi_popup_btn_sure_size_x = 100,
    wifi_popup_btn_sure_size_y = 40,

    wifi_popup_btn_cancle_x = 220,
    wifi_popup_btn_cancle_y = 120,
    wifi_popup_btn_cancle_size_x = 100,
    wifi_popup_btn_cancle_size_y = 40,

    wifi_popup_btn_pw_enter_x = 100,
    wifi_popup_btn_pw_enter_y = 70,
    wifi_popup_btn_pw_enter_size_x = 130,
    wifi_popup_btn_pw_enter_size_y = 40,

}WIFI_XY_POS_t;

typedef enum{
    wifi_kb_none_flag,              // 按下wifi名字的按键，进入对话框
    wifi_kb_send_wifi_username,     
    wifi_kb_send_wifi_password,
    wifi_kb_send_wifi_STA_mode,
    wifi_kb_send_wifi_PORT,         // 8080
    wifi_kb_send_wifi_connect,

    wifi_kb_send_wifi_disconnect,   // 断开连接
}wifi_kb_flag_t;




typedef struct {

    wifi_kb_flag_t wifi_kb_flag;
    uint8_t wifi_send_num;       // 发送获取的第n个wifi名

    lv_obj_t *wifi_src_1;               // wifi应用部分被
    lv_obj_t *wifi_kb_src_1;            // 键盘的界面
    lv_obj_t *wifi_popup_scr_1;         // 弹窗1---wifi

    lv_style_t wifi_scr1_style;         // 样式
    lv_style_t wifi_kb_src_style;
    lv_style_t wifi_popup_scr_1_style;
    lv_style_t wifi_popup_btn_style;
    lv_style_t wifi_line_style;

    lv_obj_t *wifi_btn_line1;           // wifi行按键
    lv_obj_t *wifi_btn_line2; 
    lv_obj_t *wifi_btn_line3; 
    lv_obj_t *wifi_btn_line4; 
    lv_obj_t *wifi_btn_line5; 
    lv_obj_t *wifi_btn_line6; 
    lv_obj_t *wifi_btn_line7; 
    lv_obj_t *wifi_btn_line8; 

    lv_obj_t *wifi_popup_btn_sure; 
    lv_obj_t *wifi_popup_btn_cancle; 
    lv_obj_t *wifi_popup_btn_pw_enter; 

    lv_obj_t *wifi_line1;               // wifi绘制线
    lv_obj_t *wifi_line2;
    lv_obj_t *wifi_line3;
    lv_obj_t *wifi_line4;
    lv_obj_t *wifi_line5;
    lv_obj_t *wifi_line6;
    lv_obj_t *wifi_line7;
    lv_obj_t *wifi_line8;

    lv_obj_t *wifi_kb;                  // wifi键盘
    lv_obj_t *wifi_tb;
    lv_obj_t *wifi_btn_connect;
    lv_obj_t *wifi_label_username;
    lv_obj_t *wifi_label_password;
    lv_obj_t *wifi_label_connect;
    lv_obj_t *wifi_label_rssi;
    lv_obj_t *wifi_label_ip;
    lv_style_t wifi_src_style;
    lv_style_t wifi_kb_rel;
    lv_style_t wifi_kb_pre;

    lv_obj_t *wifi_btn_up; 
    lv_obj_t *wifi_btn_next;
    lv_obj_t *wifi_btn_scanf; 
    // lv_obj_t *wifi_btn_disconnect;      // 断开连接
    lv_obj_t *wifi_imgbtn_disconnect;      // 断开连接
    lv_obj_t *wifi_imgbtn_back;

    lv_obj_t *wifi_label_line1;         // label
    lv_obj_t *wifi_label_line2; 
    lv_obj_t *wifi_label_line3; 
    lv_obj_t *wifi_label_line4; 
    lv_obj_t *wifi_label_line5; 
    lv_obj_t *wifi_label_line6;
    lv_obj_t *wifi_label_line7; 
    lv_obj_t *wifi_label_line8;

    lv_obj_t *wifi_label_up;
    lv_obj_t *wifi_label_next;
    lv_obj_t *wifi_label_scanf; 
    lv_obj_t *wifi_label_back;
    lv_obj_t *wifi_label_disconnect;

    lv_obj_t *wifi_label_info;
}MKS_WIFI_PAGE_T;
extern MKS_WIFI_PAGE_T wifi_src;

void mks_draw_wifi(void);
void mks_clear_wifi(void);
void mks_wifi_connect_check(IPAddress ip);
void draw_wifi_loading(void);
void mks_wifi_del_label(void);
void mks_wifi_show_label(void);
void mks_draw_wifi_kb(void);
void mks_wifi_connect(char *username, char *password);
void mks_draw_wifi_disconnrcting(void);
void draw_pos_wifi_popup(const char *text, char *file_name);
void mks_draw_wifi_scanf(void);     // 提示扫描界面
void mks_draw_wifi_show(void);           // 正常wifi界面
void mks_draw_wifi_had_connect(void);
#endif

#endif