#ifndef __mks_draw_inFile_h
#define __mks_draw_inFile_h

#include "MKS_draw_lvgl.h"


typedef enum {

    inFILE_SRC_X = 0,
    inFILE_SRC_Y = 0,
    inFILE_SRC_SIZE_X = 480-1,
    inFILE_SRC_SIZE_Y = 320-1,

    inFILE_SRC1_X = 0,
    inFILE_SRC1_Y = 0,
    inFILE_SRC1_SIZE_X = 480-1,
    inFILE_SRC1_SIZE_Y = 320-1,

    inFILE_SRC2_X = 0,
    inFILE_SRC2_Y = 0,
    inFILE_SRC2_SIZE_X = 480-1,
    inFILE_SRC2_SIZE_Y = 320-1,

    inFILE_SRC3_X = 0,
    inFILE_SRC3_Y = 0,
    inFILE_SRC3_SIZE_X = 480-1,
    inFILE_SRC3_SIZE_Y = 320-1,


    inFILE_BTN_X = 30,
    inFILE_BTN_Y = 10,
    inFILE_BTN_SIZE_X = 120,
    inFILE_BTN_SIZE_Y = 55,
    inFILE_BTN_X_OFFSET = inFILE_BTN_SIZE_X + 0,
    inFILE_BTN_Y_OFFSET = inFILE_BTN_SIZE_Y + 10,
}inFILE_XY_POS;



typedef struct {

    // lv_obj_t *inFile_src;
    lv_obj_t *cavre_popup;                   // 雕刻页面
    lv_obj_t *popup_add;                // 雕刻次数加
    lv_obj_t *popup_dec;                // 雕刻次数减
    lv_obj_t *popup_sure;               // 确认雕刻     
    lv_obj_t *popup_back;               // 返回雕刻
    lv_obj_t *popup_label_count;        // 显示雕刻次数
    lv_obj_t *popup_label_file;         // 文件名
    lv_style_t popup_style;            

    lv_obj_t *btn_pos;              // 定点
    lv_obj_t *btn_sure_print;       // 打印
    lv_obj_t *btn_frame;            // 巡边
    lv_obj_t *btn_cancle;           // 返回
    lv_obj_t *btn_speed;            // 速度
    lv_obj_t *btn_len;              // 步长
    lv_obj_t *btn_set_move;              // 设置为移动
    lv_obj_t *btn_set_carve;              // 设置为雕刻

    lv_obj_t *btn_sculpture;             

    // lv_obj_t *imgbtn_X_N;
    // lv_obj_t *imgbtn_X_P;
    // lv_obj_t *imgbtn_Y_N;
    // lv_obj_t *imgbtn_Y_P;

    lv_style_t src_color;
    lv_style_t btn_color;
    lv_style_t btn_press_color;

    lv_obj_t *label_sure_print;
    lv_obj_t *label_xy_pos;
    lv_obj_t *label_z_pos;
    lv_obj_t *label_frame;
    lv_obj_t *label_cancle;
    lv_obj_t *label_file_name;
    lv_obj_t *label_file_size;
    lv_obj_t *label_speed;              // 速度
    lv_obj_t *label_len;                // 步长
    lv_obj_t *label_sculpture;

}inFILE_PAGE_T;


void mks_draw_inFile(char *fn);
void mks_draw_freaure(void);
void mks_draw_setting(void);
void infile_clean_obj(lv_obj_t *obj_src);
void mks_draw_cavre_popup(char *fn);
#endif