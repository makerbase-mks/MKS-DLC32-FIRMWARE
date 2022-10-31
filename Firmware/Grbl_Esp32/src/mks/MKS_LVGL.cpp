#include "MKS_LVGL.h"
#include "TFT_eSPI.h"

#define LV_BUF_SIZE              10 * LV_HOR_RES_MAX
GRBL_CRTL mks_grbl;
LVGL_UI_PAGE_t mks_ui_page;

static lv_disp_buf_t    disp_buf;
static lv_color_t       bmp_public_buf[LV_BUF_SIZE];
// static lv_color_t       bmp_private_buf1[LV_BUF_SIZE]; 

/* Function */
void my_disp_flush(lv_disp_drv_t * disp, const lv_area_t * area, lv_color_t * color_p);
bool my_indev_touch(struct _lv_indev_drv_t * indev_drv, lv_indev_data_t * data);

// 1ms
void mks_lvgl_init(void) {
    
    /*
        static lv_disp_buf_t disp_buf_2;
        static lv_color_t buf2_1[LV_HOR_RES_MAX * 20];                       
        static lv_color_t buf2_2[LV_HOR_RES_MAX * 20];                       
        lv_disp_buf_init(&disp_buf_2, buf2_1, buf2_2, LV_HOR_RES_MAX * 10);  
    */
    lv_init();
    // lv_disp_buf_init(&disp_buf, bmp_public_buf, bmp_private_buf1, LV_BUF_SIZE); // Initialize the display buffer
    lv_disp_buf_init(&disp_buf, bmp_public_buf, NULL, LV_BUF_SIZE);  

    /* display driver register */
    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = LV_HOR_RES_MAX;
    disp_drv.ver_res = LV_VER_RES_MAX;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.buffer = &disp_buf;
    lv_disp_drv_register(&disp_drv);

    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);  
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_indev_touch;
    lv_indev_drv_register(&indev_drv);
}

void my_disp_flush(lv_disp_drv_t * disp, const lv_area_t * area, lv_color_t * color_p) {

    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    
#if defined(USE_LCD_DMA)
    tft.pushColorsDMA(&color_p->full, w * h, true);
#else 
    tft.pushColors(&color_p->full, w * h, true);
#endif
    

    tft.endWrite();
    lv_disp_flush_ready(disp);
}

bool my_indev_touch(struct _lv_indev_drv_t * indev_drv, lv_indev_data_t * data) {

    uint16_t touchX=0, touchY=0;
    static uint16_t last_x = 0;
    static uint16_t last_y = 0;
    boolean touched = tft.getTouch(&touchY, &touchX);

    if(touchX > 480) {
        touchX = 480;
    }

    touchX = 480 - touchX;
    touchY = 320 - touchY;
    
    if( touched != false ) {
        last_x = touchX;
        last_y = touchY;
        data->point.x = last_x;
        data->point.y = last_y;
        data->state = LV_INDEV_STATE_PR;
        
        BEEP_ON;
    }
    else {
        data->point.x = last_x;
        data->point.y = last_y;
        data->state = LV_INDEV_STATE_REL;
        BEEP_OFF;
    }
    return false;
}   

#if USE_LV_LOG != 0
/* Serial debugging */
void my_print(lv_log_level_t level, const char * file, uint32_t line, const char * dsc)
{
    Serial.printf("%s@%d->%s\r\n", file, line, dsc);
    Serial.flush();
}
#endif

void mks_grbl_parg_init(void) {

    if(language_select->get() == 0) mks_grbl.language = SimpleChinese;
    else if(language_select->get() == 1) mks_grbl.language = English;
    else if(language_select->get() == 1) mks_grbl.language = Deutsch;
    
    mks_grbl.light_status = GRBL_Light_Off;
    mks_grbl.move_dis = M_10_MM;
    mks_grbl.move_speed = HIGHT_SPEED;
    mks_grbl.bl_status = BL_NONE;               
    mks_grbl.is_mks_ts35_flag = false;
    mks_grbl.wifi_check_status = false;
    mks_grbl.wifi_connect_status = false;
    mks_grbl.power_persen = P_0_PERSEN;
    mks_grbl.mks_sd_status = 0;
    mks_grbl.wifi_back_from = 0;
    mks_grbl.is_need_ref_sd_file = true;
    mks_ui_page.mks_ui_page = MKS_UI_Ready;                     // 开机进入主页的标记
    mks_ui_page.wait_count = DEFAULT_UI_COUNT;                  // 允许10个周期的等待lvgl数据缓冲
    mks_grbl.carve_times = 0;                                   // 雕刻次数
    #if defined(ENABLE_WIFI)
    mks_wifi.wifi_show_page = 1;
    mks_wifi.wifi_scanf_status = wifi_none;
    mks_grbl.wifi_connect_enable = false;
    mks_wifi.begin_scanf_num = 0;
    #endif
}

/* MKS SD FILE */
uint8_t mks_sd_file_init() { 

    return 0;
}

uint8_t mks_get_sd_file_name() { 
    return 0;
}

uint8_t mks_upDir(void) {
    return 0;
}

uint8_t mks_nextDir(void) { 
    return 0;
}

uint8_t mks_operation_File() { 
    return 0;
}

uint8_t mks_stopSD() { 
    return 0;
}

SDState mks_readSD_Status(void) { 
    
    // uint8_t sd_status = SDCARD_IDLE;
    SDState sd_status;
    sd_status = get_sd_state(true);
    return sd_status;
}

float mks_caving_persen(void) { 
    return sd_report_perc_complete();
}
