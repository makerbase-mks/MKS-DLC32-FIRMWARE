#ifndef __MKS_updata_h
#define __MKS_updata_h

#include "MKS_draw_lvgl.h"

#define CFG_FILE_PATG           "/dlc_cfg.txt"
#define CFG_FILE_PATG2          "/bkg_cfg.txt"

#define CFG_FILE_CHECK_SUCCEED  "CFG is exit"
#define CFG_FILE_CHECK_FAIL     "CFG is no exit"

#define CFG_FILE_NUM            38    // 一共配置项
#define CFG_FILE_MAX_LINE       128

/*
    [ESP110]STA   		//设置wifi模式, STA/AP两种，如[ESP110]STA。
    [ESP100]MAKERBASE3D 	//wifi名字， 如：[ESP100]MAKERBASE3D
    [ESP101]makerbase3d	//wifi密码，如：[ESP101]makerbase3d
*/

typedef enum{
    UD_NONE,
    UD_CHEAK,                           // 正在检测是否存在更新文件
    UD_HAD_FILE,                        // 存在更新文件
    UD_NO_FILE,                         // 不存在文件，不需要更新
    UD_UPDATA_ING,                      // 正在更新
    UD_UPDATA_FINSH,                    // 更新完成
    UD_UPDATA_FAIL,                     // 更新失败
}UPDATA_CHECK_FLAG_T;

typedef struct {
    UPDATA_CHECK_FLAG_T updata_flag;    // 标记更新执行的状态
    uint32_t updata_cont;               // 用于选择更新第几个
    uint32_t updata_line;               // 用于记录更新到第几行
    uint8_t updata_persen;              // 更新百分比
    bool is_have_data_ud;               // 确定读到了数据进行更新
    bool is_have_board_1;               // 读到了主板信息1
    bool is_have_board_2;               // 读到了主板信息2
    bool is_have_board_3;               // 读到了主板信息3
}MKS_UPDATA_T;
extern MKS_UPDATA_T mks_updata;

typedef struct {

    lv_obj_t *label_updata_title;       // 更新界面的标题
    lv_obj_t *label_updata_persen;      // 更新百分比
    lv_obj_t *bar_updata;               // 更新界面的进度条

}UPDATA_PAGE_T;

void mks_updata_init(void);
void mks_draw_updata(void);
void mks_updata_data(void);
void mks_cfg_find(void);
void mks_cfg_rename(const char* path1);
#endif
