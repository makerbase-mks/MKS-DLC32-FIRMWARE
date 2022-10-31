#ifndef __MKS_FREERTOS_TASK_
#define __MKS_FREERTOS_TASK_

#include "FreeRTOS.h"
#include "MKS_LVGL.h"
#include "MKS_draw_ready.h"


extern TaskHandle_t lv_disp_tcb;
extern TaskHandle_t frame_task_tcb;
void disp_task_init(void);
void disp_task_data_updata(void);

void frame_task_init(void );
#endif
