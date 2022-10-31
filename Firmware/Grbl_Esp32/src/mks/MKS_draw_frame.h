#ifndef __mks_draw_frame_h
#define __mks_draw_frame_h

#include "MKS_draw_lvgl.h"

typedef enum {

    FRAME_SRC_X = 0,
    FRAME_SRC_Y = 0,
    FRAME_SRC_SIZE_X = 480-1,
    FRAME_SRC_SIZE_Y = 320-1,

    FRAME_IMGBTN_X = 10,
    FRAME_IMGBTN_Y = 10,

    FRAME_LABEL_FILE_NAME_X = 150,
    FRAME_LABEL_FILE_NAME_Y = 80,

    FRAME_LABEL_RUN_STATUS_X = 180,
    FRAME_LABEL_RUN_STATUS_Y = 150,

}FRAM_XY_POS;


typedef struct {

    lv_obj_t *frame_src;
    lv_style_t frame_src_style;
    
    lv_obj_t *btn_cancle;
    lv_obj_t *label_cancle;
    lv_obj_t *label_text;
}FRAME_PAGE_T;


typedef enum {
    FRAME_NONE,
    FRAWM_READ_SD,
    FRAWM_READ_SD_BUSY,
    FRAM_RUN,
    FRAM_RUN_WAIT,
    FRAM_END,
}FRAME_STATUS;


#define FRAME_BUFF_SIZE             64
#define FRAME_READ_FILE_BUFF        1024    // 读取1K数据

typedef struct {

    FRAME_STATUS    frame_starus;
    float           x_max;
    float           y_max;
    float           x_min;
    float           y_min;
    float           x_temp;
    float           y_temp;

    // make sure is have g0 g1 g91
    bool            have_g0;
    bool            have_g1;
    bool            have_G91;

    // 
    uint32_t        safe_count;
    char            *x_or_y;

    char            x_value[10];
    char            y_value[10];
    uint8_t         cancle_enable;

    bool            is_finsh_run;                  // 检测是否巡边完成
    bool            is_begin_run;                  // 开始检测
    bool            is_read_file = false;          // 判断是否开始读文件巡边
    bool            is_use_lb = false;             // 
    bool            out;

    // 用于记录文件信息
    uint32_t        file_size;
    char            file_name[128];

    uint32_t        current_file_size;
    char            current_file_name[128];

    bool            is_use_same_file = false;

}FRAME_CRTL_T;
extern FRAME_CRTL_T frame_ctrl;

extern SemaphoreHandle_t is_fram_need;

void mks_draw_frame(void);
void mks_run_frame(char *parameter);
void mks_frame_init(void);
bool mks_get_frame_status(void);
void frame_finsh_popup(void);
void frame_run(void);
void lb_polocte_cmd(char *str);
void frame_run(bool is_lb);
#endif
