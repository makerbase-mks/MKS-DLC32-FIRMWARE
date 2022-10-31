#ifndef __MKS_draw_carving_h
#define __MKS_draw_carving_h

#include "MKS_draw_lvgl.h"

extern char file0_name[40];
extern char file1_name[40];
extern char file2_name[40];
extern char file3_name[40];
extern char file4_name[40];
extern char file5_name[40];
extern char file_print_send[128];
extern bool file_popup_select_flag;

typedef enum {

    caving_src1_x = 10,
    caving_src1_y = 10,
    caving_src1_size_x = 460,
    caving_src1_size_y = 90,

    caving_back_x = 20,
    caving_back_y = 60,

    caving_up_x = -120,
    caving_up_y = -15,
    caving_next_x = -30,
    caving_next_y = -15,

    caving_first_file_x = -180,
    caving_first_file_y = 0,
    caving_first_file_label_x = -180,
    caving_first_file_label_y = 30,
    caving_first_x_offset = 120,
    caving_first_y_offset = 105,

    caving_file_name_show = 80,

}CAVING_XY_t;

#define MKS_FILE_NUM 8
#define MKS_FILE_NAME_LENGTH   64
#define MKS_FILE_DEEP  1
typedef struct {
    char filename_str[MKS_FILE_NUM][MKS_FILE_NAME_LENGTH];
    uint32_t file_size[MKS_FILE_NUM];
    uint16_t file_begin_num;        // 它用来计数，并选择数组的位置,filename_str[file_begin_num][...]
    uint16_t file_count;            // 它用来计算从第几个文件开始存名字
    uint16_t file_page;             // 它从1开始
    uint8_t file_choose;            //标记选择的是第几个文件
}MKS_FILE_LIST_t;
extern MKS_FILE_LIST_t mks_file_list;

void mks_draw_craving(void);
void mks_clear_craving(void);
void mks_draw_sd_file(uint8_t status, uint8_t file_num, const char *filename);
// void mks_draw_caving_popup(char* text);
void mks_del_file_obj(void);


void draw_file_btmimg(void);
void draw_file_btmimg_1(uint8_t num);
void draw_filexx(uint8_t num, char *name);


void mks_del_file_obj_1(uint8_t num);

void mks_draw_caving_popup(uint8_t text, char *srt);
void mks_draw_file_loadig(void);
void get_print_file_name(char *srt);
void start_print(void);
#endif
