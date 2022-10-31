#ifndef mks_draw_tool_h
#define mks_draw_tool_h

#include "MKS_draw_lvgl.h"

typedef enum {

    about_src1_x = 10,
    about_src1_y = 10,
    about_src1_x_size = 460,
    about_src1_y_size = 90,

    about_first_line_x = 10,
    about_first_line_y = 150,

}ABOUT_XY_POS_t;

typedef struct {

    lv_obj_t *imgbtn_back;
    lv_obj_t *imgbtn_wifi;

    lv_obj_t *label_back;
    lv_obj_t *label_wifi;

    lv_obj_t *label_board_version;
    lv_obj_t *label_fw_version;
    lv_obj_t *label_CPU_INFO;       // CHIP ID, TEMP;


}TOOL_PAGE_T;



// void lv_draw_tool(void);
void mks_draw_tool(void);
void mks_clear_tool(void);
void light_img_change(uint8_t status);
void mks_draw_pos_pupop(void);

#endif
