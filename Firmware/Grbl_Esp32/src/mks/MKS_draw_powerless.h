#ifndef __MKS_draw_powerless_h
#define __MKS_draw_powerless_h

#include "MKS_draw_lvgl.h"

#define PL_FILE_PATG           "/PLA.txt"

typedef struct {

    bool is_pl_flag;
    char pl_file_name[128];
    uint32_t file_line;                 // 从PLA.txt文件中读取行数
}MKS_PL_T;


#endif
