#include "MKS_draw_frame.h"
#include "../WebUI/WebSettings.h"
#include "../WebUI/ESPResponse.h"
#include "../SDCard.h"


FRAME_CRTL_T frame_ctrl;
FRAME_PAGE_T frame_page;

uint8_t x1point_flag = false;
uint8_t x2point_flag = false;
uint8_t y1point_flag = false;
uint8_t y2point_flag = false;

// 创建内核对象句柄
SemaphoreHandle_t is_fram_need = NULL;

LV_IMG_DECLARE(back);		

static void event_handler_cancle(lv_obj_t* obj, lv_event_t event) {

    if(event == LV_EVENT_RELEASED) {
        
        if(frame_ctrl.cancle_enable == true) {
            mks_ui_page.mks_ui_page = MKS_UI_PAGE_LOADING;
            mks_ui_page.wait_count = 1;
            lv_obj_clean(mks_src);
            mks_draw_ready();
        }
    }
}

void mks_draw_frame(void) { 

    mks_frame_init();

    BaseType_t sem_give = pdPASS;

    lv_style_copy(&frame_page.frame_src_style, &lv_style_scr);
    frame_page.frame_src_style.body.main_color = LV_COLOR_MAKE(0x1F, 0x23, 0x33); 
    frame_page.frame_src_style.body.grad_color = LV_COLOR_MAKE(0x1F, 0x23, 0x33); 
    frame_page.frame_src_style.text.color = LV_COLOR_WHITE;
    frame_page.frame_src_style.body.radius = 17;

    frame_page.frame_src = lv_obj_create(mks_global.mks_src, NULL);

    lv_obj_set_size(frame_page.frame_src ,FRAME_SRC_SIZE_X, FRAME_SRC_SIZE_Y);
	lv_obj_set_pos(frame_page.frame_src, FRAME_SRC_X, FRAME_SRC_Y);
	lv_obj_set_style(frame_page.frame_src, &frame_page.frame_src_style);

    frame_page.label_text = mks_lvgl_long_sroll_label_with_wight_set_center(frame_page.frame_src, 
                                                                            frame_page.label_text, 
                                                                            FRAME_LABEL_RUN_STATUS_X, 
                                                                            FRAME_LABEL_RUN_STATUS_Y+10, 
                                                                            "Loading File...", 
                                                                            255);
    mks_ui_page.mks_ui_page = MKS_UI_Frame;
    mks_ui_page.wait_count = 1;
    lv_refr_now(lv_refr_get_disp_refreshing());

    // 重新获取文件信息
    memset(frame_ctrl.current_file_name, 0, sizeof(frame_ctrl.current_file_name));
	memcpy(frame_ctrl.current_file_name, frame_ctrl.file_name, 128);
    frame_ctrl.current_file_size = mks_file_list.file_size[mks_file_list.file_choose]; 

    // 发送信号量通知线程进入巡边
    sem_give = xSemaphoreGive(is_fram_need);

    if(sem_give == pdTRUE) {
        grbl_send(CLIENT_SERIAL ,"sem send succeed\n");
    }
    else{
        grbl_send(CLIENT_SERIAL ,"sem send fail\n");
    }
}

void mks_openSDFile(char* parameter) {
    
    if (*parameter == '\0') {    // 用来判断文件名是否为空
        return;
    }

    String path = trim(parameter);
    
    SDState state = get_sd_state(true);
    if (state != SDState::Idle) {
        if (state == SDState::NotPresent) {
            // webPrintln("No SD Card");
            return;
        } 
        else 
        {
            // webPrintln("SD Card Busy");
            return;
        }
    }

    if (!openFile(SD, path.c_str())) {
        
        return;
    }
}

// uint8_t *p;

void polocte_cmd(char *str) {
    
    if(strstr(str, "G0")) {
        frame_ctrl.have_g0 = true;
    }

    else if(strstr(str, "G1")) {
        frame_ctrl.have_g1 = true;
    }
    frame_ctrl.safe_count = 0;
    while( *str != '\n' && frame_ctrl.safe_count < FRAME_BUFF_SIZE -1 ) {      
        if((frame_ctrl.have_g0 == true) || (frame_ctrl.have_g1 == true)) {
            if(*str == 'X') {
                str++;
                memset(frame_ctrl.x_value, '\0', sizeof(frame_ctrl.x_value));
                frame_ctrl.x_or_y = &frame_ctrl.x_value[0];
                while(*str !=' ' && *str!='\r' && *str!='\n') {
                    *frame_ctrl.x_or_y = *str;
                    str++;
                    frame_ctrl.x_or_y++;
                }
                frame_ctrl.x_temp = atof(frame_ctrl.x_value);
                
                if(frame_ctrl.x_temp > frame_ctrl.x_max) frame_ctrl.x_max = frame_ctrl.x_temp;
                if(frame_ctrl.x_temp < frame_ctrl.x_min) frame_ctrl.x_min = frame_ctrl.x_temp;
                // else if(frame_ctrl.x_min == 0) frame_ctrl.x_min = frame_ctrl.x_temp;
            }
            
            if(*str == 'Y') {
                str++;
                memset(frame_ctrl.y_value, '\0', sizeof(frame_ctrl.x_value));
                frame_ctrl.x_or_y = &frame_ctrl.y_value[0];
                while(*str !=' ' && *str!='\r' && *str!='\n') {
                    *frame_ctrl.x_or_y = *str;
                    str++;
                    frame_ctrl.x_or_y++;
                }
                frame_ctrl.y_temp = atof(frame_ctrl.y_value);
                if(frame_ctrl.y_temp > frame_ctrl.y_max) frame_ctrl.y_max = frame_ctrl.y_temp;
                if(frame_ctrl.y_temp < frame_ctrl.y_min) frame_ctrl.y_min = frame_ctrl.y_temp;
            }
        } // end
        str++;
        frame_ctrl.safe_count++;
    }
    frame_ctrl.have_g0 = false;
    frame_ctrl.have_g1 = false;
}

void lb_polocte_cmd(char *str) {

    frame_ctrl.safe_count = 0;
    while (*str != '\n' && (frame_ctrl.safe_count < FRAME_BUFF_SIZE)) {
        grbl_send(CLIENT_SERIAL, "\n enter find buff");
        grbl_sendf(CLIENT_SERIAL, "safe_count=:%d\n", frame_ctrl.safe_count);
        if (*str == 'X') {
            str++;
            memset(frame_ctrl.x_value, '\0', sizeof(frame_ctrl.x_value));
            frame_ctrl.x_or_y = &frame_ctrl.x_value[0];

            while (*str != ' ' && *str != '\r' && *str != '\n') {
                *frame_ctrl.x_or_y = *str;
                str++;
                frame_ctrl.x_or_y++;
            }

            frame_ctrl.x_temp = atof(frame_ctrl.x_value);

            if (x1point_flag == false) {

                frame_ctrl.x_min = frame_ctrl.x_temp;
                x1point_flag = true;
            }
            else if (x2point_flag == false) {
                frame_ctrl.x_max = frame_ctrl.x_temp;
                x2point_flag = true;
            }
        }

        if (*str == 'Y') {
            str++;
            memset(frame_ctrl.y_value, '\0', sizeof(frame_ctrl.y_value));
            frame_ctrl.x_or_y = &frame_ctrl.y_value[0];
            while (*str != ' ' && *str != '\r' && *str != '\n') {
                *frame_ctrl.x_or_y = *str;
                str++;
                frame_ctrl.x_or_y++;
            }
            frame_ctrl.y_temp = atof(frame_ctrl.y_value);

            if (y1point_flag == false) {
                frame_ctrl.y_min = frame_ctrl.y_temp;
                y1point_flag = true;
            }
            else if (y2point_flag == false) {
                frame_ctrl.y_max = frame_ctrl.y_temp;
                y2point_flag = true;
            }
        }
        str++;
        frame_ctrl.safe_count++;
    }
}

void mks_frame_init(void) { 

    frame_ctrl.frame_starus = FRAME_NONE;

    if(frame_ctrl.is_use_same_file != true) {
        frame_ctrl.x_max = 0;
        frame_ctrl.x_min = 0;
        frame_ctrl.y_max = 0;
        frame_ctrl.y_min = 0;
    }
    
    frame_ctrl.y_temp = 0;
    frame_ctrl.x_temp = 0;
    frame_ctrl.have_g0 = false;
    frame_ctrl.have_g1 = false;
    frame_ctrl.have_G91 = false;
    frame_ctrl.cancle_enable = false;
    frame_ctrl.is_begin_run = false;
    frame_ctrl.is_finsh_run = false;
    frame_ctrl.is_read_file = true;
}


void event_handle_yes(lv_obj_t* obj, lv_event_t event) {

    if(event == LV_EVENT_RELEASED) {
        lv_obj_del(com_p1.com_popup_src);
        frame_ctrl.out = false;
        mks_ui_page.mks_ui_page = MKS_UI_PAGE_LOADING; 
	    mks_ui_page.wait_count = DEFAULT_UI_COUNT;
        mks_lv_clean_ui();
        start_print();
    }
}

void event_handle_no(lv_obj_t* obj, lv_event_t event) {

    if(event == LV_EVENT_RELEASED) {
        lv_obj_del(com_p1.com_popup_src);
        mks_ui_page.mks_ui_page = MKS_UI_PAGE_LOADING; 
	    mks_ui_page.wait_count = DEFAULT_UI_COUNT;
        frame_ctrl.out = false;
        mks_lv_clean_ui();
        // mks_draw_craving();
        mks_draw_inFile(frame_ctrl.current_file_name);
    }
}


void mks_run_frame(char *parameter) {

    char frame_cmd[20];
    frame_ctrl.cancle_enable = true;
    frame_ctrl.out = true;

    if (sys.state != State::Idle && sys.state != State::Alarm) {    // 判断SD卡状态
        mks_lv_label_updata(frame_page.label_text, "SD is busy...");
        lv_refr_now(lv_refr_get_disp_refreshing());
        frame_ctrl.cancle_enable = true;
        return ;
    }

    if(frame_ctrl.file_name[0] != '/') frame_ctrl.file_name[0] = '/';
    grbl_sendf(CLIENT_SERIAL, "frame file:%s\n", frame_ctrl.file_name);
    mks_openSDFile(parameter);

    char fileLine[255];
    uint8_t point_last_num = 1;
    uint32_t point_count = 0;
    uint32_t get_current_line = sd_get_current_line_number(); // 获取当前
    char *b = NULL;
    bool need_fine = true;

    x1point_flag = false;
    x2point_flag = false;
    y1point_flag = false;
    y2point_flag = false;

    while ( (readFileLine(fileLine, 255))   && 
            (frame_ctrl.is_read_file)       && 
            (frame_ctrl.is_use_same_file == false)) {
        
        grbl_send(CLIENT_SERIAL ,"enter while \n");
        if(get_current_line < 10) {
            get_current_line = sd_get_current_line_number();
            b = strstr(fileLine, "Bounds");
            if(b != NULL) {
                grbl_send(CLIENT_SERIAL, "have Bounds\n");
                grbl_send(CLIENT_SERIAL, fileLine);
                lb_polocte_cmd(fileLine);
                frame_ctrl.is_read_file = false;
                frame_ctrl.is_use_lb = true;
                // frame_ctrl.is_use_same_file = true;
                break;
            }
        }
        polocte_cmd(fileLine);
    }

    frame_ctrl.is_use_same_file = true;

    grbl_sendf(CLIENT_SERIAL ,"(%.2f, %.2f), (%.2f, %.2f)", 
                                frame_ctrl.x_min, 
                                frame_ctrl.y_min, 
                                frame_ctrl.x_max, 
                                frame_ctrl.y_max);
    closeFile();
    mks_lv_label_updata(frame_page.label_text, "Running...");
    lv_refr_now(lv_refr_get_disp_refreshing()); 

    MKS_GRBL_CMD_SEND("M3 S5\n");

    sprintf(frame_cmd, "G0 X%f Y%f 300\n",frame_ctrl.x_min, frame_ctrl.y_min); // point 0
    MKS_GRBL_CMD_SEND(frame_cmd);
    grbl_send(CLIENT_SERIAL ,frame_cmd);

    sprintf(frame_cmd, "G1 Y%f F1000\n",frame_ctrl.y_max);  // point 1
    MKS_GRBL_CMD_SEND(frame_cmd);
    grbl_send(CLIENT_SERIAL ,frame_cmd);

    sprintf(frame_cmd, "G1 X%f F1000\n", frame_ctrl.x_max); // point 2
    MKS_GRBL_CMD_SEND(frame_cmd);
    grbl_send(CLIENT_SERIAL ,frame_cmd);

    sprintf(frame_cmd, "G1 Y%f F1000\n", frame_ctrl.y_min); // point 3
    MKS_GRBL_CMD_SEND(frame_cmd);
    grbl_send(CLIENT_SERIAL ,frame_cmd);

    sprintf(frame_cmd, "G1 X%f F1000\n", frame_ctrl.x_min); // point 4 
    MKS_GRBL_CMD_SEND(frame_cmd);
    grbl_send(CLIENT_SERIAL ,frame_cmd);
    
    MKS_GRBL_CMD_SEND("M5\n");
    MKS_GRBL_CMD_SEND("G0 X0 Y0 F300\n");

    // frame_run(frame_ctrl.is_use_lb);
    frame_ctrl.is_begin_run = true;
    frame_ctrl.cancle_enable = true;
    grbl_send(CLIENT_SERIAL ,"frame finsh\n");
    return ;
}

void frame_run(bool is_lb) {
    char frame_cmd[20];

    if(!is_lb) {
        MKS_GRBL_CMD_SEND("M3 S5\n");
        sprintf(frame_cmd, "G0 X%f Y%f 300\n",frame_ctrl.x_min, frame_ctrl.y_min); // point 0
        MKS_GRBL_CMD_SEND(frame_cmd);

        sprintf(frame_cmd, "G1 Y%f F1000\n",frame_ctrl.y_max);  // point 1
        MKS_GRBL_CMD_SEND(frame_cmd);

        sprintf(frame_cmd, "G1 X%f F1000\n", frame_ctrl.x_max); // point 2
        MKS_GRBL_CMD_SEND(frame_cmd);

        sprintf(frame_cmd, "G1 Y%f F1000\n", frame_ctrl.y_min); // point 3
        MKS_GRBL_CMD_SEND(frame_cmd);

        sprintf(frame_cmd, "G1 X%f F1000\n", frame_ctrl.x_min); // point 4 
        MKS_GRBL_CMD_SEND(frame_cmd);
        
        MKS_GRBL_CMD_SEND("M5\n");
        MKS_GRBL_CMD_SEND("G0 X0 Y0 F300\n");
    }else {
        MKS_GRBL_CMD_SEND("M3 S5\n");
        // MKS_GRBL_CMD_SEND("G91\n");
        sprintf(frame_cmd, "G0 X%f Y%f 300\n",frame_ctrl.x_min, frame_ctrl.y_min); // point 0
        MKS_GRBL_CMD_SEND(frame_cmd);

        // MKS_GRBL_CMD_SEND("G90\n");

        sprintf(frame_cmd, "G1 Y%f F1000\n", abs(frame_ctrl.y_max - frame_ctrl.y_min));  // point 1
        MKS_GRBL_CMD_SEND(frame_cmd);

        sprintf(frame_cmd, "G1 X%f F1000\n", abs(frame_ctrl.x_max - frame_ctrl.x_min)); // point 2
        MKS_GRBL_CMD_SEND(frame_cmd);

        sprintf(frame_cmd, "G1 Y%f F1000\n", -abs(frame_ctrl.y_max - frame_ctrl.y_min)); // point 3
        MKS_GRBL_CMD_SEND(frame_cmd);

        sprintf(frame_cmd, "G1 X%f F1000\n", -abs(frame_ctrl.x_max - frame_ctrl.x_min)); // point 4 
        MKS_GRBL_CMD_SEND(frame_cmd);
        
        MKS_GRBL_CMD_SEND("M5\n");
        MKS_GRBL_CMD_SEND("G0 X0 Y0 F300\n");
    }
}


static void event_handler_cave_yes(lv_obj_t* obj, lv_event_t event) {

	if (event == LV_EVENT_RELEASED) {

		mks_ui_page.mks_ui_page = MKS_UI_PAGE_LOADING;
		
		#if defined(USR_RELASE)
			lv_obj_clean(mks_src);
		#else 
			lv_obj_clean(mks_global.mks_src);
		#endif
			start_print();
	}
}

static void event_handler_cave_no(lv_obj_t* obj, lv_event_t event) {

	if (event == LV_EVENT_RELEASED) {

        char temp[128];
		
        cavre_popup_del();

        lv_obj_clean(mks_global.mks_src);

        memset(temp, 0, sizeof(temp));
        memcpy(temp, mks_file_list.filename_str[mks_file_list.file_choose], sizeof(temp));
        if(temp[0] == '/') temp[0] = ' ';
        mks_draw_inFile(temp);
        // mks_draw_inFile(mks_file_list.filename_str[mks_file_list.file_choose]);
	}
}


void frame_finsh_popup(void) {
    char temp[128];
    memset(temp, 0, sizeof(temp));
    memcpy(temp, mks_file_list.filename_str[mks_file_list.file_choose], sizeof(temp));
    if(temp[0] == '/') temp[0] = ' ';
    mks_draw_cavre_popup(temp, event_handler_cave_yes, event_handler_cave_no);
}

bool mks_get_frame_status(void) { 

    if(sys.state == State::Idle) {
        frame_ctrl.is_finsh_run = true;
    }else{
        frame_ctrl.is_finsh_run = false;
    }
    return frame_ctrl.is_finsh_run;
}




