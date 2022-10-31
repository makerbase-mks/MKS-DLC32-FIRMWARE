#include "MKS_FREERTOS_TASK.h"

#define DISP_TASK_STACK                 4096*2
#define DISP_TASK_PRO                   2
#define DISP_TASK_CORE                  1

#define FRAME_TASK_STACK                4096*2
#define FRAME_TASK_PRO                  3          // 尝试巡边的线程与lvgl线程同优先级
#define FRAME_TASK_CORE                 1

TaskHandle_t lv_disp_tcb = NULL;
TaskHandle_t frame_task_tcb = NULL;

#define USE_DelayUntil

static void mks_page_data_updata(void);

IRAM_ATTR void lvgl_disp_task(void *parg) { 

#if defined(USE_DelayUntil)
    TickType_t       xLastWakeTime;
    const TickType_t xDisplayFrequency = 5;                  
    xLastWakeTime = xTaskGetTickCount();   
#endif

    bool logo_flag = true;   
    uint16_t logo_flag_count = 0; 
    mks_lvgl_init();

    mks_global_style_init();

    mks_draw_logo();

    // 创建二值量
    is_fram_need = xSemaphoreCreateBinary();
    frame_task_init();
    mks_grbl.wifi_connect_enable = true;

    while(1) {

        if(logo_flag == true) {
            lv_task_handler();
            logo_flag_count++;

            if(logo_flag_count == 100) {
                 LCD_BLK_ON;
            }

            if(logo_flag_count == 400) {
                mks_lv_clean_ui();

                if(mks_updata.updata_flag == UD_HAD_FILE) {
                    mks_draw_updata();
                }else {
                    if(mks_grbl.is_test_mode == true) {
                        mks_draw_test_ui();
                    }else{
                        mks_draw_ready();
                    }
                }
                logo_flag = false;
            }
        }else {
            lv_task_handler();
            mks_page_data_updata();
        }

#if defined(USE_DelayUntil)
    vTaskDelayUntil(&xLastWakeTime, xDisplayFrequency); //使用相对延时，保证时间精准
#else
        vTaskDelay(5); // 5ms 绝对延时
#endif   
    }
}

uint8_t count_updata = 0;
uint8_t fram_count = 0;
static void mks_page_data_updata(void) { 
    
    if(mks_ui_page.mks_ui_page == MKS_UI_PAGE_LOADING) {
        /* Do not updata */
        return ;
    }
    else if(mks_ui_page.mks_ui_page == MKS_UI_TEST) {
        if((count_updata == 200) || (count_updata > 200) ) { 
            draw_testing();
        }
    }
    else if (mks_ui_page.mks_ui_page == MKS_UI_Ready) {  //只有在当前页面才更新数据

        if((count_updata == 20) || (count_updata > 20) ) {        // 20*5=100ms
            if(SD_ready_next == false) ready_data_updata();
            count_updata = 0;
        }
    }
    else if(mks_ui_page.mks_ui_page == MKS_UI_Pring) { // 雕刻界面更新数据

        if((count_updata == 200) || (count_updata > 200) ) { // 200*5=1000ms = 1s

            if(SD_ready_next == false) {
                mks_print_data_updata();
            } 
            count_updata = 0;
        }
    }
    else if(mks_ui_page.mks_ui_page == MKS_UI_inFile) { // 雕刻界面更新数据

        if((count_updata == 200) || (count_updata > 200) ) { // 200*5=1000ms = 1s
            move_pos_update();
            probe_check();
        }
    }
    else if(mks_ui_page.mks_ui_page == MKS_UI_Control) {  //控制界面

        if((count_updata == 20) || (count_updata > 20) ) { // 20*5=100ms = 1s
            hard_home_check();
            soft_home_check();
            move_pos_update();
            probe_check();
            count_updata = 0;
        }
    }
    else if(mks_ui_page.mks_ui_page == MKS_UI_Frame) { 
        
        if((count_updata == 100) || (count_updata > 100) ) { // 100*5=100ms = 1s
            if(frame_ctrl.is_begin_run == true) {
                    if(mks_get_frame_status() == true) {

                        if(fram_count == 10) {
                            if(mks_grbl.popup_1_flag != true){
                                frame_finsh_popup();
                                // void mks_draw_cavre_popup();
                            }
                            frame_ctrl.is_begin_run = false;
                            fram_count = 0;
                        }
                        fram_count++;
                    }else {
                        fram_count = 0;
                    }
            }
        }
    }
#if defined(ENABLE_WIFI)
    else if(mks_ui_page.mks_ui_page == MKS_UI_Wifi) {

        if((mks_wifi.wifi_scanf_status == wifi_scanf_begin) || (mks_wifi.wifi_scanf_status == wifi_scanf_waitting)) {
            mks_wifi_scanf();
        }else if(mks_wifi.wifi_scanf_status == wifi_scanf_succeed) {
            mks_lv_clean_ui();
            mks_draw_wifi_show();
            mks_wifi.wifi_scanf_status = wifi_none;
        }else if(mks_wifi.wifi_scanf_status == wifi_connecting) {
            if(mks_grbl.wifi_connect_status == true) {          // 正在连接状态
                if(mks_get_wifi_status() == true) {             // 确认连接上
                    mks_lv_clean_ui();
                    mks_draw_wifi();
                    mks_wifi.wifi_scanf_status = wifi_none;
                }
            }
        }
        else if(mks_wifi.wifi_scanf_status == wifi_scanf_fail) {
            mks_lv_clean_ui();
            mks_draw_wifi();
            mks_wifi.wifi_scanf_status = wifi_none;
        }
        else if(mks_wifi.wifi_scanf_status == wifi_disconnecting) {
            
            if(mks_get_wifi_status() == false) {  // 确认断开
                mks_wifi.wifi_scanf_status = wifi_scanf_begin;   // 重新扫描
                mks_lv_clean_ui();
                mks_draw_wifi();
            }
        }
        count_updata = 0;
    }
#endif
    else if(mks_ui_page.mks_ui_page == MKS_UI_UPDATA) {
        if(mks_updata.updata_flag == UD_UPDATA_ING) {
            mks_cfg_find();
        }
        else if(mks_updata.updata_flag == UD_UPDATA_FINSH) {
            // 更新完成弹窗
            mks_draw_common_pupup_info("Info", "Update succeed", "Please restart!");
            mks_cfg_rename(CFG_FILE_PATG2);
            mks_updata.updata_flag = UD_NONE;
        }
        else if(mks_updata.updata_flag == UD_UPDATA_FAIL) {
            // 更新失败弹窗
            mks_draw_common_pupup_info("Error", "Update Fail", "Please Check mkscfg.txt or sdcard");
            mks_updata.updata_flag = UD_NONE;
        }
    }
    count_updata++;
}

// // tskNO_AFFINIT 
IRAM_ATTR void disp_task_init(void) {

    xTaskCreatePinnedToCore(lvgl_disp_task,     // task
                            "lvglTask",         // name for task
                            DISP_TASK_STACK,    // size of task stack
                            NULL,               // parameters
                            DISP_TASK_PRO,      // priority
                            // nullptr,
                            &lv_disp_tcb,
                            DISP_TASK_CORE      // must run the task on same core
                                                // core
    );
}

/*-------------------------------------------------------------------------------------------------*/

IRAM_ATTR void frame_task(void *parg) {

    grbl_send(CLIENT_SERIAL,"Creat frame task succeed\n");
    // 定义一个信号量返回值
    BaseType_t sem_receive = pdPASS;

    while(1) {  

        sem_receive = xSemaphoreTake(is_fram_need, portMAX_DELAY);

        if(sem_receive == pdTRUE) {
            // 触发信号量
            grbl_send(CLIENT_SERIAL,"receive sem succeed\n");
            vTaskSuspend(lv_disp_tcb); // 挂起任务da
            mks_run_frame(frame_ctrl.file_name);
            vTaskResume(lv_disp_tcb);   // 恢复任务
        }
    }
}

IRAM_ATTR void frame_task_init(void ) {

    xTaskCreatePinnedToCore(frame_task,             // task
                            "Frame task",           // name for task
                            FRAME_TASK_STACK,       // size of task stack
                            NULL,                   // parameters
                            FRAME_TASK_PRO,         // priority
                            // nullptr,
                            &frame_task_tcb,
                            FRAME_TASK_CORE         // must run the task on same core
                                                    // core
    );
}

/*-------------------------------------------测试模式----------------------------------------------*/

#define TEST_CODE_TASK_STACK                4096
#define TEST_CODE_TASK_PRO                  3          
#define TEST_CODE_TASK_CORE                 1

TaskHandle_t test_code_handle = NULL;


void dlc32_test_debug(void *parg) {

    while(1) {
    
    }
}

void test_code_task_init() {

    xTaskCreatePinnedToCore(dlc32_test_debug,             // task
                            "Test code",           // name for task
                            TEST_CODE_TASK_STACK,       // size of task stack
                            NULL,                   // parameters
                            TEST_CODE_TASK_PRO,         // priority
                            // nullptr,
                            &test_code_handle,
                            TEST_CODE_TASK_CORE         // must run the task on same core
                                                    // core
    );


}
