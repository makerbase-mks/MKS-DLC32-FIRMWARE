#ifndef __MKS_LVGL_H
#define __MKS_LVGL_H

#include "MKS_TS35.h"
#include "MKS_FREERTOS_TASK.h"
#include "MKS_ctrl.h"
#include "lvgl.h"
#include "../Serial.h"
#include "MKS_SDCard.h"


#define MKS_GRBL_CMD_SEND(A)            serila_write_into_buffer((uint8_t *)A)   

#define MKS_GRBL_WEB_CMD_SEND(A)        serial_web_input_into_buffer((uint8_t *)A)
#define MKS_GRBL_WEB_HEX_CMD_SEND(A)    serial_web_input_into_hex(A)

typedef enum {
    M_0_1_MM,           // move 0.1mm
    M_1_MM,             // move 1mm
    M_10_MM,            // move 10mm
}GRBL_MOVE_DIS;

typedef enum {
    LOW_SPEED,
    MID_SPEED,
    HIGHT_SPEED,
}GRBL_MOVE_SPEED;

typedef enum {
    SimpleChinese,      
    English,
    Deutsch,
}GRBL_Language;

typedef enum {
    GRBL_Light_On,
    GRBL_Light_Off,
}GRBL_LIGHT_STATUS;

typedef enum {
    P_1_PERSEN,
    P_10_PERSEN,
    P_0_PERSEN,
}GRBL_POWER;

typedef enum {
    GRBL_RUN,
    GRBL_PAUSE,
    GRBL_STOP,          
    GRBL_RESTARTING,
}GRBL_RUN_STATUS_t;

typedef enum {
    BL_NONE,
    BL_BEGIN,           //开始调整
    BL_UP,
    BL_DOWN,
    BL_ADJ_ING,         //正在调整
    BL_ADJ_END,         //调整结束
}BLTOUCH_STATUS;

typedef enum {
    GRBL_CNC_OFF,
    GRBL_CNC_50,
    GRBL_CNC_100,
}GRBL_CNC_PWR;


typedef struct{
    GRBL_MOVE_DIS               move_dis;           // 移动距离
    GRBL_MOVE_SPEED             move_speed;         // 移动速度
    GRBL_Language               language;           // 语言设置
    GRBL_LIGHT_STATUS           light_status;       // 灯状态
    GRBL_POWER                  power_persen;       // 功率步长设置
    BLTOUCH_STATUS              bl_status;          // BLTOUCH运行状态
    GRBL_CNC_PWR                cnc_pwr;            // CNC功率设置
    uint16_t                    X_Pos;              // x坐标
    uint16_t                    Y_Pos;              // y坐标
    uint8_t                     mks_sd_status;      // sd卡是否插入, 0:没检测到SD卡， 1:检测到SD卡
    uint16_t                    mks_sd_file_times;  // 读取时，以6个文件为基础，从times*6开始显示文件名
    bool                        is_mks_ts35_flag;   // 是否是通过TS35执行的雕刻功能
    bool                        wifi_connect_status;// wifi連接轉態檢測
    bool                        wifi_check_status;  // 是否需要檢測wifi連接
    uint8_t                     wifi_back_from;     // 判断wifi界面是从哪里进去的 0=主界面， 1=关于界面
    bool                        wifi_connect_enable;// 允许wifi自动连接
    bool                        popup_1_flag;
    bool                        is_need_ref_sd_file;
    uint32_t                    carve_times;        // 雕刻次数
    bool                        is_test_mode = false;       // 是否使用生产模式
}GRBL_CRTL;
extern GRBL_CRTL mks_grbl;

typedef enum {

    MKS_UI_PAGE_LOADING,        // 跳转页面时的标志

    /* 页面标志 */
    MKS_UI_Logo,    // logo显示
    MKS_UI_Ready,   // 主界面
    MKS_UI_Adjust,  // 功率调整界面
    MKS_UI_Control, // 移动控制界面
    MKS_UI_Caving,  // 雕刻界面
    MKS_UI_Pring,   // 打印界面
    MKS_UI_Tool,    // 工具界面
    MKS_UI_Wifi,    // WIFI相关界面
    MKS_UI_Frame,   // 巡边界面
    MKS_UI_inFile,  // 打开文件后的界面
    MKS_UI_UPDATA,  // 配置文件更新界面
    MKS_UI_LANGUAGE,    // 语言设置界面
    MKS_UI_TEST,    // 测试模式的页面
}mks_ui_page_t;


#define DEFAULT_UI_COUNT    1
typedef struct {
    mks_ui_page_t mks_ui_page;
    uint8_t wait_count;             // 等待LVGL事件生成缓冲，用于界面数据更新
}LVGL_UI_PAGE_t;
extern LVGL_UI_PAGE_t mks_ui_page;




void mks_lvgl_init(void);
void lvgl_test(void);
SDState mks_readSD_Status(void);
float mks_caving_persen(void);
void mks_grbl_parg_init(void);
#endif
