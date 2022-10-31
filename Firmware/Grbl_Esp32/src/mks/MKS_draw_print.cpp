#include "MKS_draw_print.h"
#include "../System.h"

PWR_CTRL_t mks_pwr_ctrl;
SPEED_CTRL_t mks_speed_ctrl;
MKS_PRINT_PAGE_t print_src;
MKS_PRINT_DATA_UPDATA_t print_data_updata;
MKS_PRINT_SETTING_T print_setting;

uint32_t ddxd;

/* btn */
static lv_obj_t* btn_popup_cancle;
static lv_obj_t* btn_popup_sure;
static lv_obj_t* btn_finsh_popup_sure;

lv_obj_t* Label_print_file_name;

LV_IMG_DECLARE(M_Pause);  // 暂停
LV_IMG_DECLARE(M_start);  // 开始
LV_IMG_DECLARE(M_Stop);  // 停止
LV_IMG_DECLARE(M_PWRr);  // 功率
LV_IMG_DECLARE(M_SPEED);  // 速度
LV_IMG_DECLARE(back);  // 速度

LV_IMG_DECLARE(add);  // 加
LV_IMG_DECLARE(confirm);  // 确认
LV_IMG_DECLARE(s_return);  // 确认
LV_IMG_DECLARE(reduce);  // 减

LV_IMG_DECLARE(png_cave_pwr);
LV_IMG_DECLARE(png_cave_speed);
LV_IMG_DECLARE(png_cave_xpos);
LV_IMG_DECLARE(png_cave_ypos);
LV_IMG_DECLARE(png_cave_zpos);
LV_IMG_DECLARE(png_pause_pre);          // 暂停
LV_IMG_DECLARE(png_start_pre);          // 开始
LV_IMG_DECLARE(png_stop_pre);           // 停止
LV_IMG_DECLARE(png_cave_pwr_pre);       // 功率
LV_IMG_DECLARE(png_cave_speed_pre);     // 速度
LV_IMG_DECLARE(png_times);              // 雕刻次数

static void event_handler_suspend(lv_obj_t* obj, lv_event_t event) {

    if (event == LV_EVENT_RELEASED) {

        if(sys.state == State::Hold) {
            lv_imgbtn_set_src(print_src.print_imgbtn_suspend, LV_BTN_STATE_PR, &png_start_pre);
            lv_imgbtn_set_src(print_src.print_imgbtn_suspend, LV_BTN_STATE_REL, &png_start);
            lv_label_set_static_text(print_src.print_Label_p_suspend, "Star");
            MKS_GRBL_CMD_SEND("~");
            if(print_setting._need_to_start_write) {
                sys_rt_s_override = print_setting.cur_spindle_pwr;
            }
        }   
        else if(sys.state == State::Cycle)    {
            lv_imgbtn_set_src(print_src.print_imgbtn_suspend, LV_BTN_STATE_PR, &png_pause_pre);
            lv_imgbtn_set_src(print_src.print_imgbtn_suspend, LV_BTN_STATE_REL, &png_pause);
            lv_label_set_static_text(print_src.print_Label_p_suspend, "Pause");
            MKS_GRBL_CMD_SEND("!");
            // spindle->stop();
        } 
    }
}

static void event_handler_stop(lv_obj_t* obj, lv_event_t event) {
    if (event == LV_EVENT_RELEASED) {
        mks_draw_print_popup("Do you want to stop print?");
    }
}


static void event_handler_op(lv_obj_t* obj, lv_event_t event) {
    if (event == LV_EVENT_RELEASED) {
        mks_clear_print();
        mks_draw_operation();
    }
}

static void event_handler_adj(lv_obj_t* obj, lv_event_t event) {
    if (event == LV_EVENT_RELEASED) {
        set_print_click(false);
        draw_adj_popup();
    }
}

static void event_handler_none(lv_obj_t* obj, lv_event_t event) {
    if (event == LV_EVENT_RELEASED) {

    }
}




void mks_draw_print(void) {

    char print_file_name[128];
    
    print_setting.cur_spindle_pwr = sys_rt_s_override;
    print_setting.cur_spindle_speed = sys_rt_f_override;
    print_setting.cur_spindle_rapid = sys_rt_r_override;

    // mks fix
    print_setting.carve_staus = CAVRE_START;

    memcpy(print_file_name, file_print_send, sizeof(file_print_send));
    if(print_file_name[0] == '/') print_file_name[0] = ' ';

    mks_pwr_ctrl.pwr_len = PWR_1_PERSEN;
    mks_speed_ctrl.speed_len = SPEED_1_PERSEN;

    lv_style_copy(&print_src.print_file_name_style, &lv_style_plain_color);
    print_src.print_file_name_style.text.font = &lv_font_roboto_16;

    /* 进度条背景样式 */
    lv_style_copy(&print_src.print_bar_bg_style, &lv_style_plain_color);
    print_src.print_bar_bg_style.body.main_color = LV_COLOR_MAKE(0x3F,0x46,0x66);
    print_src.print_bar_bg_style.body.grad_color = LV_COLOR_MAKE(0x3F,0x46,0x66);
    print_src.print_bar_bg_style.body.radius = 5;

    /* 进度条显示样式 */
    lv_style_copy(&print_src.print_bar_indic_style,&lv_style_plain_color);
    print_src.print_bar_indic_style.body.main_color = LV_COLOR_MAKE(0x52,0xCC,0x82);
    print_src.print_bar_indic_style.body.grad_color = LV_COLOR_MAKE(0x52,0xCC,0x82);
    print_src.print_bar_indic_style.body.radius = 5;
    print_src.print_bar_indic_style.body.padding.left = 0;//让指示器跟背景边框之间没有距离
    print_src.print_bar_indic_style.body.padding.top = 0;
    print_src.print_bar_indic_style.body.padding.right = 0;
    print_src.print_bar_indic_style.body.padding.bottom = 0;

    print_src.print_imgbtn_suspend  = lv_imgbtn_creat_n_mks(mks_global.mks_src,  print_src.print_imgbtn_suspend, &png_start_pre, &png_start, 8, 250 ,event_handler_suspend);
    print_src.print_imgbtn_stop     = lv_imgbtn_creat_n_mks(mks_global.mks_src,  print_src.print_imgbtn_stop, &png_stop_pre, &png_stop, 165, 250 ,event_handler_stop);
    print_src.print_imgbtn_adj      = lv_imgbtn_creat_n_mks(mks_global.mks_src,  print_src.print_imgbtn_adj, &png_adj_pre, &png_adj, 322, 250, event_handler_adj);

    print_src.print_bar_print = mks_lv_bar_set(mks_global.mks_src, print_src.print_bar_print, 464, 50, print_bar_pic_x, print_bar_pic_y, 0);

    lv_bar_set_style(print_src.print_bar_print, LV_BAR_STYLE_BG , &print_src.print_bar_bg_style);
    lv_bar_set_style(print_src.print_bar_print, LV_BAR_STYLE_INDIC , &print_src.print_bar_indic_style);

    print_src.print_Label_p_suspend = label_for_imgbtn_name_mid(mks_global.mks_src, print_src.print_Label_p_suspend, print_src.print_imgbtn_suspend ,-35 ,0 ,"Pause");
    print_src.print_Label_p_stop = label_for_imgbtn_name_mid(mks_global.mks_src, print_src.print_Label_p_stop, print_src.print_imgbtn_stop ,-40 ,0 ,"Stop");
    print_src.print_Label_p_adj = label_for_imgbtn_name_mid(mks_global.mks_src, print_src.print_Label_p_adj, print_src.print_imgbtn_adj ,-20 ,0 ,"Adjustment");
    


    print_src.print_Label_power = label_for_text(mks_global.mks_src, print_src.print_Label_power, NULL, 194, 161, LV_ALIGN_IN_TOP_LEFT, "S:0%");  // 输出功率
    print_src.print_Label_caveSpeed =  label_for_text(mks_global.mks_src, print_src.print_Label_caveSpeed, NULL, 39, 161, LV_ALIGN_IN_TOP_LEFT, "F:0%");    // 雕刻速度
    print_src.print_Label_caveR =  label_for_text(mks_global.mks_src, print_src.print_Label_caveR, NULL, 356, 161, LV_ALIGN_IN_TOP_LEFT, "R:0%");  

    Label_print_file_name = label_for_text(mks_global.mks_src, Label_print_file_name, NULL, 30, 6, LV_ALIGN_IN_TOP_LEFT, print_file_name);
    lv_label_set_style(Label_print_file_name, LV_LABEL_STYLE_MAIN, &print_src.print_file_name_style);

    print_src.print_bar_print_percen = label_for_btn_name(print_src.print_bar_print, print_src.print_bar_print_percen, 190, 0, "0%");

    mks_ui_page.mks_ui_page = MKS_UI_Pring;  //进入雕刻界面
	mks_ui_page.wait_count = DEFAULT_UI_COUNT;
}


static void event_btn_cancle(lv_obj_t* obj, lv_event_t event) {
    if (event == LV_EVENT_RELEASED) {
        
        lv_obj_set_click(print_src.print_imgbtn_suspend, true);
        lv_obj_set_click(print_src.print_imgbtn_stop, true);
        lv_obj_set_click(print_src.print_imgbtn_adj, true);
        // lv_obj_set_click(print_src.print_imgbtn_pwr, true);
        // lv_obj_set_click(print_src.print_imgbtn_speed, true);
        lv_obj_del(print_src.print_stop_popup);
    }
}

static void event_btn_sure(lv_obj_t* obj, lv_event_t event) {
    uint16_t buf_cmd[]={0x18};
    if (event == LV_EVENT_RELEASED) {
        lv_obj_set_click(print_src.print_imgbtn_suspend, true);
        lv_obj_set_click(print_src.print_imgbtn_stop, true);
        lv_obj_set_click(print_src.print_imgbtn_adj, true);
        // lv_obj_set_click(print_src.print_imgbtn_pwr, true);
        // lv_obj_set_click(print_src.print_imgbtn_speed, true);
        closeFile();
        mks_ui_page.mks_ui_page = MKS_UI_PAGE_LOADING;
        mks_ui_page.wait_count = 1;
        mks_clear_print();
        MKS_GRBL_CMD_SEND("M3 S0\n");
        MKS_GRBL_CMD_SEND("G90X0Y0F800\n");
        MKS_GRBL_CMD_SEND(buf_cmd);
        mks_draw_ready();
    }
}

static void event_btn_printdon(lv_obj_t* obj, lv_event_t event) {
    if (event == LV_EVENT_RELEASED) {

        lv_obj_set_click(print_src.print_imgbtn_suspend, true);
        lv_obj_set_click(print_src.print_imgbtn_stop, true);
        lv_obj_set_click(print_src.print_imgbtn_adj, true);
        mks_ui_page.mks_ui_page = MKS_UI_PAGE_LOADING;
        
        lv_obj_del(print_src.print_finsh_popup);
        mks_clear_print();
        mks_draw_ready();
    }
}

// stop print popup
void mks_draw_print_popup(const char* text) {
    
    lv_obj_set_click(print_src.print_imgbtn_suspend, false);
    lv_obj_set_click(print_src.print_imgbtn_stop, false);
    lv_obj_set_click(print_src.print_imgbtn_adj, false);

    print_src.print_stop_popup = lv_obj_create(mks_global.mks_src, NULL);

    lv_obj_set_size(print_src.print_stop_popup, print_popup_size_x, print_popup_size_y);
    lv_obj_set_pos(print_src.print_stop_popup, print_popup_x, print_popup_y);

    lv_style_copy(&print_src.printf_popup_style, &lv_style_scr);
    print_src.printf_popup_style.body.main_color = LV_COLOR_MAKE(0xCE, 0xD6, 0xE5);
    print_src.printf_popup_style.body.grad_color = LV_COLOR_MAKE(0xCE, 0xD6, 0xE5);
    print_src.printf_popup_style.text.color = LV_COLOR_BLACK;
    print_src.printf_popup_style.body.radius = 17;
    lv_obj_set_style(print_src.print_stop_popup, &print_src.printf_popup_style);

    lv_style_copy(&print_src.print_popup_btn_style, &lv_style_scr);
    print_src.print_popup_btn_style.body.main_color = LV_COLOR_MAKE(0x3F, 0x46, 0x66);
    print_src.print_popup_btn_style.body.grad_color = LV_COLOR_MAKE(0x3F, 0x46, 0x66);
    print_src.print_popup_btn_style.body.opa = LV_OPA_COVER;//设置背景色完全不透明
    print_src.print_popup_btn_style.text.color = LV_COLOR_WHITE;
    print_src.print_popup_btn_style.body.radius = 10; 

    btn_popup_sure = mks_lv_btn_set(print_src.print_stop_popup, btn_popup_sure, 100,40,20,130,event_btn_sure);
	lv_btn_set_style(btn_popup_sure, LV_BTN_STYLE_REL, &print_src.print_popup_btn_style);
    lv_btn_set_style(btn_popup_sure,LV_BTN_STYLE_PR,&print_src.print_popup_btn_style);

    label_for_btn_name(btn_popup_sure, print_src.print_Label_popup_sure, 30, 0, "Yes");

	btn_popup_cancle = mks_lv_btn_set(print_src.print_stop_popup, btn_popup_cancle, 100,40,230,130,event_btn_cancle);
	lv_btn_set_style(btn_popup_cancle, LV_BTN_STYLE_REL, &print_src.print_popup_btn_style);
    lv_btn_set_style(btn_popup_cancle,LV_BTN_STYLE_PR,&print_src.print_popup_btn_style);

    label_for_btn_name(btn_popup_cancle, print_src.print_Label_popup_sure, 50, 0, "Cancel");
    mks_lvgl_long_sroll_label_with_wight_set(print_src.print_stop_popup, print_src.print_Label_popup, 80, 60, text, 200);
}

void mks_draw_finsh_pupop(void) { 

    lv_obj_set_click(print_src.print_imgbtn_suspend, false);
    lv_obj_set_click(print_src.print_imgbtn_stop, false);
    lv_obj_set_click(print_src.print_imgbtn_adj, false);
    

    print_src.print_finsh_popup = lv_obj_create(mks_global.mks_src, NULL);

    lv_obj_set_size(print_src.print_finsh_popup, 350, 200);
    lv_obj_set_pos(print_src.print_finsh_popup, 80, 50);

    lv_style_copy(&print_src.printf_popup_style, &lv_style_scr);
    print_src.printf_popup_style.body.main_color = LV_COLOR_MAKE(0xCE, 0xD6, 0xE5);
    print_src.printf_popup_style.body.grad_color = LV_COLOR_MAKE(0xCE, 0xD6, 0xE5);
    print_src.printf_popup_style.text.color = LV_COLOR_BLACK;
    print_src.printf_popup_style.body.radius = 17;
    lv_obj_set_style(print_src.print_finsh_popup, &print_src.printf_popup_style);

    lv_style_copy(&print_src.print_popup_btn_style, &lv_style_scr);
    print_src.print_popup_btn_style.body.main_color = LV_COLOR_MAKE(0x3F, 0x46, 0x66);
    print_src.print_popup_btn_style.body.grad_color = LV_COLOR_MAKE(0x3F, 0x46, 0x66);
    print_src.print_popup_btn_style.body.opa = LV_OPA_COVER;//设置背景色完全不透明
    print_src.print_popup_btn_style.text.color = LV_COLOR_WHITE;
    print_src.print_popup_btn_style.body.radius = 10; 

    btn_finsh_popup_sure = lv_btn_create(print_src.print_finsh_popup, NULL);
    lv_obj_set_size(btn_finsh_popup_sure,   100, 50);
    lv_obj_set_pos(btn_finsh_popup_sure,    120, 130);
    lv_obj_set_event_cb(btn_finsh_popup_sure, event_btn_printdon);
    lv_btn_set_style(btn_finsh_popup_sure, LV_BTN_STYLE_REL, &print_src.print_popup_btn_style);
    lv_btn_set_style(btn_finsh_popup_sure,LV_BTN_STYLE_PR,&print_src.print_popup_btn_style);

    label_for_btn_name(btn_finsh_popup_sure, print_src.print_Label_popup_sure, 0, 0, "Yes");
    label_for_screen(print_src.print_finsh_popup, print_src.print_Label_popup, 0, -20, "File is print done!");
}

char bar_percen_str[20];
void mks_print_bar_updata(void) {
    print_src.print_bar_print = mks_lv_bar_updata(print_src.print_bar_print, (uint16_t)sd_report_perc_complete());
    sprintf(bar_percen_str, "%d%%", (uint16_t)sd_report_perc_complete());
    print_src.print_bar_print_percen = mks_lv_label_updata(print_src.print_bar_print_percen, bar_percen_str);
}

/****************************************************************************************pwr_popup****************************************************************************************/

lv_obj_t *pwr_label_power;

char power_add_dec_buf[20];

static void event_pwr_setting_add(lv_obj_t* obj, lv_event_t event) {
    if (event == LV_EVENT_RELEASED) {
        
        if(mks_pwr_ctrl.pwr_len == PWR_1_PERSEN) {
            print_setting.cur_spindle_pwr += SpindleSpeedOverride::FineIncrement;
            if(print_setting.cur_spindle_pwr > SpindleSpeedOverride::Max) {
                print_setting.cur_spindle_pwr = SpindleSpeedOverride::Max;    
            }
        }else if(mks_pwr_ctrl.pwr_len == PWR_10_PERSEN) {
            print_setting.cur_spindle_pwr += SpindleSpeedOverride::CoarseIncrement;
            if(print_setting.cur_spindle_pwr > SpindleSpeedOverride::Max) {
                print_setting.cur_spindle_pwr = SpindleSpeedOverride::Max;    
            }
        }
        sprintf(power_add_dec_buf, "Power:%d%%", print_setting.cur_spindle_pwr);
        lv_label_set_static_text(pwr_label_power, power_add_dec_buf);
    }
}

static void event_pwr_setting_dec(lv_obj_t* obj, lv_event_t event) {

    uint16_t temp;

    if (event == LV_EVENT_RELEASED) {

        if(mks_pwr_ctrl.pwr_len == PWR_1_PERSEN) {
            print_setting.cur_spindle_pwr -= SpindleSpeedOverride::FineIncrement;
            if(print_setting.cur_spindle_pwr < SpindleSpeedOverride::Min) {
                print_setting.cur_spindle_pwr = SpindleSpeedOverride::Min;
            }
        }else if(mks_pwr_ctrl.pwr_len == PWR_10_PERSEN) {
            print_setting.cur_spindle_pwr -= SpindleSpeedOverride::CoarseIncrement;
            if(print_setting.cur_spindle_pwr < SpindleSpeedOverride::Min) {
                print_setting.cur_spindle_pwr = SpindleSpeedOverride::Min;
            }
        }
        sprintf(power_add_dec_buf, "Power:%d%%", print_setting.cur_spindle_pwr);
        lv_label_set_static_text(pwr_label_power, power_add_dec_buf);
    }
}

static void event_btn_pwr_1mm(lv_obj_t* obj, lv_event_t event) {
    if (event == LV_EVENT_RELEASED) {

        if(mks_pwr_ctrl.pwr_len == PWR_10_PERSEN) {
            mks_pwr_ctrl.pwr_len = PWR_1_PERSEN;
            lv_label_set_text(print_src.print_label_1_mm, "#ffffff 1% #");
            lv_label_set_text(print_src.print_label_10_mm, "#000000 10% #");

            print_src.print_mm_btn1_style.body.main_color = LV_COLOR_MAKE(0x3F, 0x46, 0x66);
            print_src.print_mm_btn1_style.body.grad_color = LV_COLOR_MAKE(0x3F, 0x46, 0x66);
            lv_btn_set_style(print_src.print_btn_1_mm, LV_BTN_STYLE_REL, &print_src.print_mm_btn1_style);
            lv_btn_set_style(print_src.print_btn_1_mm,LV_BTN_STYLE_PR,&print_src.print_mm_btn1_style);

            print_src.print_mm_btn2_style.body.main_color = LV_COLOR_WHITE;
            print_src.print_mm_btn2_style.body.grad_color = LV_COLOR_WHITE;
            lv_btn_set_style(print_src.print_btn_10_mm, LV_BTN_STYLE_REL, &print_src.print_mm_btn2_style);
            lv_btn_set_style(print_src.print_btn_10_mm,LV_BTN_STYLE_PR,&print_src.print_mm_btn2_style); 
        }
    }
}

static void event_btn_pwr_10mm(lv_obj_t* obj, lv_event_t event) {
    if (event == LV_EVENT_RELEASED) {

        if(mks_pwr_ctrl.pwr_len == PWR_1_PERSEN) {
            mks_pwr_ctrl.pwr_len = PWR_10_PERSEN;
            lv_label_set_text(print_src.print_label_1_mm, "#000000 1% #");
            lv_label_set_text(print_src.print_label_10_mm, "#ffffff 10% #");

            print_src.print_mm_btn1_style.body.main_color = LV_COLOR_MAKE(0x3F, 0x46, 0x66);
            print_src.print_mm_btn1_style.body.grad_color = LV_COLOR_MAKE(0x3F, 0x46, 0x66);
            lv_btn_set_style(print_src.print_btn_10_mm, LV_BTN_STYLE_REL, &print_src.print_mm_btn1_style);
            lv_btn_set_style(print_src.print_btn_10_mm,LV_BTN_STYLE_PR,&print_src.print_mm_btn1_style);

            print_src.print_mm_btn2_style.body.main_color = LV_COLOR_WHITE;
            print_src.print_mm_btn2_style.body.grad_color = LV_COLOR_WHITE;
            lv_btn_set_style(print_src.print_btn_1_mm, LV_BTN_STYLE_REL, &print_src.print_mm_btn2_style);
            lv_btn_set_style(print_src.print_btn_1_mm,LV_BTN_STYLE_PR,&print_src.print_mm_btn2_style);
        }
    }
}

static void event_pwr_setting_confirm(lv_obj_t* obj, lv_event_t event) {
    if (event == LV_EVENT_RELEASED) {

        if(sys.state == State::Hold) {
            print_setting._need_to_start_write = true;
        }else{
            sys_rt_s_override = print_setting.cur_spindle_pwr;
        }

        lv_obj_set_click(print_src.print_imgbtn_suspend, true);
        lv_obj_set_click(print_src.print_imgbtn_stop, true);
        lv_obj_set_click(print_src.print_imgbtn_adj, true);
        lv_obj_del(print_src.print_pwr_speed_src);
    }
}

static void event_pwr_setting_return(lv_obj_t* obj, lv_event_t event) {

    if (event == LV_EVENT_RELEASED) {

        print_setting.cur_spindle_pwr = sys_rt_s_override;

        lv_obj_set_click(print_src.print_imgbtn_suspend, true);
        lv_obj_set_click(print_src.print_imgbtn_stop, true);
        lv_obj_set_click(print_src.print_imgbtn_adj, true);
        lv_obj_del(print_src.print_pwr_speed_src);
    }
}


void mks_print_pwr_set(void) { 

    char buf[20]; 

    lv_obj_set_click(print_src.print_imgbtn_suspend, false);
    lv_obj_set_click(print_src.print_imgbtn_stop, false);

#if defined(USR_RELASE)
    print_src.print_pwr_speed_src = lv_obj_create(mks_src, NULL);
#else
    print_src.print_pwr_speed_src = lv_obj_create(mks_global.mks_src, NULL);
#endif
    lv_obj_set_size(print_src.print_pwr_speed_src, 350, 200);
    lv_obj_set_pos(print_src.print_pwr_speed_src, 75, 50);

    lv_style_copy(&print_src.printf_popup_style, &lv_style_scr);
    print_src.printf_popup_style.body.main_color = LV_COLOR_MAKE(0xCE, 0xD6, 0xE5);
    print_src.printf_popup_style.body.grad_color = LV_COLOR_MAKE(0xCE, 0xD6, 0xE5);
    print_src.printf_popup_style.text.color = LV_COLOR_BLACK;
    print_src.printf_popup_style.body.radius = 17;
    lv_obj_set_style(print_src.print_pwr_speed_src, &print_src.printf_popup_style);

    lv_style_copy(&print_src.print_mm_btn1_style, &lv_style_scr);
    print_src.print_mm_btn1_style.body.main_color = LV_COLOR_MAKE(0x3F, 0x46, 0x66);
    print_src.print_mm_btn1_style.body.grad_color = LV_COLOR_MAKE(0x3F, 0x46, 0x66);
    print_src.print_mm_btn1_style.body.opa = LV_OPA_COVER;//设置背景色完全不透明
    print_src.print_mm_btn1_style.text.color = LV_COLOR_WHITE;
    print_src.print_mm_btn1_style.body.radius = 10; 

    lv_style_copy(&print_src.print_mm_btn2_style, &lv_style_scr);
    print_src.print_mm_btn2_style.body.main_color = LV_COLOR_MAKE(0x3F, 0x46, 0x66);
    print_src.print_mm_btn2_style.body.grad_color = LV_COLOR_MAKE(0x3F, 0x46, 0x66);
    print_src.print_mm_btn2_style.body.opa = LV_OPA_COVER;//设置背景色完全不透明
    print_src.print_mm_btn2_style.text.color = LV_COLOR_WHITE;
    print_src.print_mm_btn2_style.body.radius = 10; 

    print_src.print_btn_1_mm = mks_lv_btn_set(print_src.print_pwr_speed_src, print_src.print_btn_1_mm, 100, 40, 60, 80, event_btn_pwr_1mm);
    print_src.print_btn_10_mm = mks_lv_btn_set(print_src.print_pwr_speed_src, print_src.print_btn_10_mm, 100, 40, 180, 80, event_btn_pwr_10mm);

    if(mks_pwr_ctrl.pwr_len == PWR_1_PERSEN) { 
        print_src.print_mm_btn1_style.body.main_color = LV_COLOR_MAKE(0x3F, 0x46, 0x66);
        print_src.print_mm_btn1_style.body.grad_color = LV_COLOR_MAKE(0x3F, 0x46, 0x66);
        lv_btn_set_style(print_src.print_btn_1_mm, LV_BTN_STYLE_REL, &print_src.print_mm_btn1_style);
        lv_btn_set_style(print_src.print_btn_1_mm,LV_BTN_STYLE_PR,&print_src.print_mm_btn1_style);

        print_src.print_mm_btn2_style.body.main_color = LV_COLOR_WHITE;
        print_src.print_mm_btn2_style.body.grad_color = LV_COLOR_WHITE;
        lv_btn_set_style(print_src.print_btn_10_mm, LV_BTN_STYLE_REL, &print_src.print_mm_btn2_style);
        lv_btn_set_style(print_src.print_btn_10_mm,LV_BTN_STYLE_PR,&print_src.print_mm_btn2_style); 
    }else {
        print_src.print_mm_btn1_style.body.main_color = LV_COLOR_MAKE(0x3F, 0x46, 0x66);
        print_src.print_mm_btn1_style.body.grad_color = LV_COLOR_MAKE(0x3F, 0x46, 0x66);
        lv_btn_set_style(print_src.print_btn_10_mm, LV_BTN_STYLE_REL, &print_src.print_mm_btn1_style);
        lv_btn_set_style(print_src.print_btn_10_mm,LV_BTN_STYLE_PR,&print_src.print_mm_btn1_style);

        print_src.print_mm_btn2_style.body.main_color = LV_COLOR_WHITE;
        print_src.print_mm_btn2_style.body.grad_color = LV_COLOR_WHITE;
        lv_btn_set_style(print_src.print_btn_1_mm, LV_BTN_STYLE_REL, &print_src.print_mm_btn2_style);
        lv_btn_set_style(print_src.print_btn_1_mm,LV_BTN_STYLE_PR,&print_src.print_mm_btn2_style); 
    }

    sprintf(buf, "Power:%d%%", sys_rt_s_override);

    pwr_label_power = label_for_screen(print_src.print_pwr_speed_src, pwr_label_power, 0, -60, buf);

    if(mks_pwr_ctrl.pwr_len == PWR_1_PERSEN) {
        print_src.print_label_1_mm = mks_lvgl_long_sroll_label_with_wight_set_center(print_src.print_btn_1_mm, print_src.print_label_1_mm, 0, 0, "#ffffff 1%#", 50);
        print_src.print_label_10_mm = mks_lvgl_long_sroll_label_with_wight_set_center(print_src.print_btn_10_mm, print_src.print_label_10_mm, 0, 0, "#000000 10% #", 50);
    }else{
        print_src.print_label_1_mm = mks_lvgl_long_sroll_label_with_wight_set_center(print_src.print_btn_1_mm, print_src.print_label_1_mm, 0, 0, "#000000 1%#", 50);
        print_src.print_label_10_mm = mks_lvgl_long_sroll_label_with_wight_set_center(print_src.print_btn_10_mm, print_src.print_label_10_mm, 0, 0, "#ffffff 10% #", 50);
    }
    
    print_src.print_sp_imgbtn_add = lv_imgbtn_creat_mks(print_src.print_pwr_speed_src, print_src.print_sp_imgbtn_add, &add, &add, LV_ALIGN_IN_LEFT_MID, print_pwr_popup_add_btn_x,print_pwr_popup_add_btn_y, event_pwr_setting_add);
    print_src.print_sp_imgbtn_dec = lv_imgbtn_creat_mks(print_src.print_pwr_speed_src, print_src.print_sp_imgbtn_dec, &reduce, &reduce, LV_ALIGN_IN_LEFT_MID, print_pwr_popup_add_btn_x+80,print_pwr_popup_add_btn_y, event_pwr_setting_dec);
    print_src.print_sp_btn_sure = lv_imgbtn_creat_mks(print_src.print_pwr_speed_src, print_src.print_sp_imgbtn_dec, &confirm, &confirm, LV_ALIGN_IN_LEFT_MID, print_pwr_popup_add_btn_x+160,print_pwr_popup_add_btn_y, event_pwr_setting_confirm);

    print_src.print_sp_btn_return = lv_imgbtn_creat_mks(print_src.print_pwr_speed_src, 
                                                        print_src.print_sp_btn_return, 
                                                        &s_return, 
                                                        &s_return, 
                                                        LV_ALIGN_IN_LEFT_MID, 
                                                        print_pwr_popup_add_btn_x+240,
                                                        print_pwr_popup_add_btn_y, event_pwr_setting_return);
}


/****************************************************************************************speed_popup****************************************************************************************/
lv_obj_t *pwr_label_speed;
char speed_add_dec_buf[20];

static void event_speed_setting_add(lv_obj_t* obj, lv_event_t event) {
    if (event == LV_EVENT_RELEASED) {
        
        if(mks_speed_ctrl.speed_len == SPEED_1_PERSEN) {
            print_setting.cur_spindle_speed += FeedOverride::FineIncrement;
            if(print_setting.cur_spindle_speed > FeedOverride::Max) {
                print_setting.cur_spindle_speed = FeedOverride::Max;
            }

        }else if(mks_speed_ctrl.speed_len == SPEED_10_PERSEN) {
            print_setting.cur_spindle_speed += FeedOverride::CoarseIncrement;
            if(print_setting.cur_spindle_speed > FeedOverride::Max) {
                print_setting.cur_spindle_speed = FeedOverride::Max;
            }
        }
        sprintf(speed_add_dec_buf, "Speed:%d%%", print_setting.cur_spindle_speed);
        lv_label_set_static_text(pwr_label_speed, speed_add_dec_buf);
    }
}

static void event_speed_setting_dec(lv_obj_t* obj, lv_event_t event) {
    if (event == LV_EVENT_RELEASED) {
        
        if(mks_speed_ctrl.speed_len == SPEED_1_PERSEN) {
            print_setting.cur_spindle_speed -= FeedOverride::FineIncrement;
            if(print_setting.cur_spindle_speed < FeedOverride::Min) {
                print_setting.cur_spindle_speed = FeedOverride::Min;
            }
        }else if(mks_speed_ctrl.speed_len == SPEED_10_PERSEN) {
            print_setting.cur_spindle_speed -= FeedOverride::CoarseIncrement;
            if(print_setting.cur_spindle_speed < FeedOverride::Min) {
                print_setting.cur_spindle_speed = FeedOverride::Min;
            }
        }
        sprintf(speed_add_dec_buf, "Speed:%d%%", print_setting.cur_spindle_speed);
        lv_label_set_static_text(pwr_label_speed, speed_add_dec_buf);
    }
}

static void event_speed_setting_confirm(lv_obj_t* obj, lv_event_t event) {
    if (event == LV_EVENT_RELEASED) {

        sys_rt_f_override = print_setting.cur_spindle_speed;

        lv_obj_set_click(print_src.print_imgbtn_suspend, true);
        lv_obj_set_click(print_src.print_imgbtn_stop, true);
        lv_obj_set_click(print_src.print_imgbtn_adj, true);
        lv_obj_del(print_src.print_pwr_speed_src);
    }
}

static void event_speed_setting_return(lv_obj_t* obj, lv_event_t event) {
    if (event == LV_EVENT_RELEASED) {

        print_setting.cur_spindle_speed = sys_rt_f_override;

        lv_obj_set_click(print_src.print_imgbtn_suspend, true);
        lv_obj_set_click(print_src.print_imgbtn_stop, true);
        lv_obj_set_click(print_src.print_imgbtn_adj, true);
        lv_obj_del(print_src.print_pwr_speed_src);
    }
}


static void event_btn_speed_1mm(lv_obj_t* obj, lv_event_t event) {
    if (event == LV_EVENT_RELEASED) {
        if(mks_speed_ctrl.speed_len == SPEED_10_PERSEN) {
            mks_speed_ctrl.speed_len = SPEED_1_PERSEN;

            lv_label_set_text(print_src.print_label_1_mm, "#ffffff 1% #");  //000000
            lv_label_set_text(print_src.print_label_10_mm, "#000000 10% #"); //ffffff
            
            print_src.print_mm_btn1_style.body.main_color = LV_COLOR_MAKE(0x3F, 0x46, 0x66);
            print_src.print_mm_btn1_style.body.grad_color = LV_COLOR_MAKE(0x3F, 0x46, 0x66);
            lv_btn_set_style(print_src.print_btn_1_mm, LV_BTN_STYLE_REL, &print_src.print_mm_btn1_style);
            lv_btn_set_style(print_src.print_btn_1_mm,LV_BTN_STYLE_PR,&print_src.print_mm_btn1_style);

            print_src.print_mm_btn2_style.body.main_color = LV_COLOR_WHITE;
            print_src.print_mm_btn2_style.body.grad_color = LV_COLOR_WHITE;
            lv_btn_set_style(print_src.print_btn_10_mm, LV_BTN_STYLE_REL, &print_src.print_mm_btn2_style);
            lv_btn_set_style(print_src.print_btn_10_mm,LV_BTN_STYLE_PR,&print_src.print_mm_btn2_style);
        }
    }
}

static void event_btn_speed_10mm(lv_obj_t* obj, lv_event_t event) {
    if (event == LV_EVENT_RELEASED) {
        if(mks_speed_ctrl.speed_len == SPEED_1_PERSEN) {
            mks_speed_ctrl.speed_len = SPEED_10_PERSEN;
            lv_label_set_text(print_src.print_label_1_mm, "#000000 1% #");
            lv_label_set_text(print_src.print_label_10_mm, "#ffffff 10% #");

            print_src.print_mm_btn1_style.body.main_color = LV_COLOR_WHITE;
            print_src.print_mm_btn1_style.body.grad_color = LV_COLOR_WHITE;
            lv_btn_set_style(print_src.print_btn_1_mm, LV_BTN_STYLE_REL, &print_src.print_mm_btn1_style);
            lv_btn_set_style(print_src.print_btn_1_mm,LV_BTN_STYLE_PR,&print_src.print_mm_btn1_style);

            print_src.print_mm_btn2_style.body.main_color = LV_COLOR_MAKE(0x3F, 0x46, 0x66);
            print_src.print_mm_btn2_style.body.grad_color = LV_COLOR_MAKE(0x3F, 0x46, 0x66);
            lv_btn_set_style(print_src.print_btn_10_mm, LV_BTN_STYLE_REL, &print_src.print_mm_btn2_style);
            lv_btn_set_style(print_src.print_btn_10_mm,LV_BTN_STYLE_PR,&print_src.print_mm_btn2_style);
        }
    }
}

void mks_print_speed_set(void) { 

    char buf[20]; 

    lv_obj_set_click(print_src.print_imgbtn_suspend, false);
    lv_obj_set_click(print_src.print_imgbtn_stop, false);
    lv_obj_set_click(print_src.print_imgbtn_adj, false);

    print_src.print_pwr_speed_src = lv_obj_create(mks_global.mks_src, NULL);

    lv_obj_set_size(print_src.print_pwr_speed_src, 350, 200);
    lv_obj_set_pos(print_src.print_pwr_speed_src, 80, 50);

    lv_style_copy(&print_src.printf_popup_style, &lv_style_scr);
    print_src.printf_popup_style.body.main_color = LV_COLOR_MAKE(0xCE, 0xD6, 0xE5);
    print_src.printf_popup_style.body.grad_color = LV_COLOR_MAKE(0xCE, 0xD6, 0xE5);
    print_src.printf_popup_style.text.color = LV_COLOR_BLACK;
    print_src.printf_popup_style.body.radius = 17;
    lv_obj_set_style(print_src.print_pwr_speed_src, &print_src.printf_popup_style);

    lv_style_copy(&print_src.print_mm_btn1_style, &lv_style_scr);
    print_src.print_mm_btn1_style.body.main_color = LV_COLOR_MAKE(0x3F, 0x46, 0x66);
    print_src.print_mm_btn1_style.body.grad_color = LV_COLOR_MAKE(0x3F, 0x46, 0x66);
    print_src.print_mm_btn1_style.body.opa = LV_OPA_COVER;//设置背景色完全不透明
    print_src.print_mm_btn1_style.text.color = LV_COLOR_WHITE;
    print_src.print_mm_btn1_style.body.radius = 10; 

    lv_style_copy(&print_src.print_mm_btn2_style, &lv_style_scr);
    print_src.print_mm_btn2_style.body.main_color = LV_COLOR_MAKE(0x3F, 0x46, 0x66);
    print_src.print_mm_btn2_style.body.grad_color = LV_COLOR_MAKE(0x3F, 0x46, 0x66);
    print_src.print_mm_btn2_style.body.opa = LV_OPA_COVER;//设置背景色完全不透明
    print_src.print_mm_btn2_style.text.color = LV_COLOR_WHITE;
    print_src.print_mm_btn2_style.body.radius = 10; 

    print_src.print_btn_1_mm = mks_lv_btn_set(print_src.print_pwr_speed_src, print_src.print_btn_1_mm, 100, 40, 60, 80, event_btn_speed_1mm);
    print_src.print_btn_10_mm = mks_lv_btn_set(print_src.print_pwr_speed_src, print_src.print_btn_10_mm, 100, 40, 180, 80, event_btn_speed_10mm);

    if(mks_speed_ctrl.speed_len == SPEED_1_PERSEN) {
        print_src.print_mm_btn1_style.body.main_color = LV_COLOR_MAKE(0x3F, 0x46, 0x66);
        print_src.print_mm_btn1_style.body.grad_color = LV_COLOR_MAKE(0x3F, 0x46, 0x66);
        lv_btn_set_style(print_src.print_btn_1_mm, LV_BTN_STYLE_REL, &print_src.print_mm_btn1_style);
        lv_btn_set_style(print_src.print_btn_1_mm,LV_BTN_STYLE_PR,&print_src.print_mm_btn1_style);
        
        print_src.print_mm_btn2_style.body.main_color = LV_COLOR_WHITE;
        print_src.print_mm_btn2_style.body.grad_color = LV_COLOR_WHITE;
        lv_btn_set_style(print_src.print_btn_10_mm, LV_BTN_STYLE_REL, &print_src.print_mm_btn2_style);
        lv_btn_set_style(print_src.print_btn_10_mm,LV_BTN_STYLE_PR,&print_src.print_mm_btn2_style);

    }else if(mks_speed_ctrl.speed_len == SPEED_10_PERSEN){
        print_src.print_mm_btn1_style.body.main_color = LV_COLOR_WHITE;
        print_src.print_mm_btn1_style.body.grad_color = LV_COLOR_WHITE;
        lv_btn_set_style(print_src.print_btn_1_mm, LV_BTN_STYLE_REL, &print_src.print_mm_btn1_style);
        lv_btn_set_style(print_src.print_btn_1_mm,LV_BTN_STYLE_PR,&print_src.print_mm_btn1_style);

         print_src.print_mm_btn2_style.body.main_color = LV_COLOR_MAKE(0x3F, 0x46, 0x66);
         print_src.print_mm_btn2_style.body.grad_color = LV_COLOR_MAKE(0x3F, 0x46, 0x66);
        lv_btn_set_style(print_src.print_btn_10_mm, LV_BTN_STYLE_REL, & print_src.print_mm_btn2_style);
        lv_btn_set_style(print_src.print_btn_10_mm,LV_BTN_STYLE_PR,& print_src.print_mm_btn2_style);
    }
    else {
        print_src.print_popup_btn_style.body.main_color = LV_COLOR_MAKE(0x3F, 0x46, 0x66);
        print_src.print_popup_btn_style.body.grad_color = LV_COLOR_MAKE(0x3F, 0x46, 0x66);
        lv_btn_set_style(print_src.print_btn_1_mm, LV_BTN_STYLE_REL, &print_src.print_popup_btn_style);
        lv_btn_set_style(print_src.print_btn_1_mm,LV_BTN_STYLE_PR,&print_src.print_popup_btn_style);
        
        print_src.print_popup_btn_style.body.main_color = LV_COLOR_WHITE;
        print_src.print_popup_btn_style.body.grad_color = LV_COLOR_WHITE;
        lv_btn_set_style(print_src.print_btn_10_mm, LV_BTN_STYLE_REL, &print_src.print_popup_btn_style);
        lv_btn_set_style(print_src.print_btn_10_mm,LV_BTN_STYLE_PR,&print_src.print_popup_btn_style);
    }

    sprintf(buf, "Speed:%d%%", sys_rt_f_override);
    // pwr_label_speed = mks_lvgl_long_sroll_label_with_wight_set_center(print_src.print_pwr_speed_src, pwr_label_speed, 20, 50, buf, 100); 
    pwr_label_speed = label_for_screen(print_src.print_pwr_speed_src, pwr_label_speed, 0, -60, buf);

    if(mks_speed_ctrl.speed_len == SPEED_1_PERSEN) {
        print_src.print_label_1_mm = mks_lvgl_long_sroll_label_with_wight_set_center(print_src.print_btn_1_mm, print_src.print_label_1_mm, 0, 0, "#ffffff 1%#", 50);
        print_src.print_label_10_mm = mks_lvgl_long_sroll_label_with_wight_set_center(print_src.print_btn_10_mm, print_src.print_label_10_mm, 0, 0, "#000000 10% #", 50);

    }else if(mks_speed_ctrl.speed_len == SPEED_10_PERSEN) {
        print_src.print_label_1_mm = mks_lvgl_long_sroll_label_with_wight_set_center(print_src.print_btn_1_mm, print_src.print_label_1_mm, 0, 0, "#000000 1%#", 50);
        print_src.print_label_10_mm = mks_lvgl_long_sroll_label_with_wight_set_center(print_src.print_btn_10_mm, print_src.print_label_10_mm, 0, 0, "#ffffff 10% #", 50);
    }

    print_src.print_sp_imgbtn_add = lv_imgbtn_creat_mks(print_src.print_pwr_speed_src, print_src.print_sp_imgbtn_add, &add, &add, LV_ALIGN_IN_LEFT_MID, print_pwr_popup_add_btn_x,print_pwr_popup_add_btn_y, event_speed_setting_add);
    print_src.print_sp_imgbtn_dec = lv_imgbtn_creat_mks(print_src.print_pwr_speed_src, print_src.print_sp_imgbtn_dec, &reduce, &reduce, LV_ALIGN_IN_LEFT_MID, print_pwr_popup_add_btn_x+80,print_pwr_popup_add_btn_y, event_speed_setting_dec);
    print_src.print_sp_btn_sure = lv_imgbtn_creat_mks(print_src.print_pwr_speed_src, 
                                                        print_src.print_sp_btn_sure, 
                                                        &confirm, 
                                                        &confirm, 
                                                        LV_ALIGN_IN_LEFT_MID, 
                                                        print_pwr_popup_add_btn_x+160,
                                                        print_pwr_popup_add_btn_y, event_speed_setting_confirm);
    
    print_src.print_sp_btn_return = lv_imgbtn_creat_mks(print_src.print_pwr_speed_src, 
                                                        print_src.print_sp_btn_return, 
                                                        &s_return, 
                                                        &s_return, 
                                                        LV_ALIGN_IN_LEFT_MID, 
                                                        print_pwr_popup_add_btn_x+240,
                                                        print_pwr_popup_add_btn_y, event_speed_setting_return);
}


lv_obj_t *label_feed_rate, *label_spindle_speed, *label_rapid_speed;
lv_obj_t *label_back, *label_confirm, *label_add, *label_dec, *label_persen;
lv_obj_t *img_back, *img_confirm, *img_add, *img_dec;
uint8_t sp_select = 0; // default is 0, mean is select feed rate;
uint8_t sp_step = 1;    //default is  1, can select 10, 20, if sp_select = 2, only can select 25

char persen_dis_str[10];

char feed_rate_dis_str[20];
char spindle_speed_dis_str[20];
char rapid_dis_str[20];
enum {
    ID_SP_FEED_RATE,
    ID_SP_SPINDLE_SPEED,
    ID_SP_RAPID_SPEED,
    ID_SP_BACK,
    ID_SP_CONFIRM,
    ID_SP_ADD,
    ID_SP_DEC,
    ID_SP_PERSEM,
};

uint8_t get_sp_event_id(lv_obj_t* obj) {

    if(obj == print_src.print_sp_imgbtn_add) return ID_SP_ADD;
    else if(obj == print_src.print_sp_imgbtn_dec) return ID_SP_DEC;
    else if(obj == print_src.print_btn_1_mm) return ID_SP_PERSEM;
    else if(obj == print_src.print_sp_btn_sure) return ID_SP_CONFIRM;
    else if(obj == print_src.print_sp_btn_return) return ID_SP_BACK;
    else if(obj == print_src.print_imgbtn_pwr) return ID_SP_FEED_RATE;
    else if(obj == print_src.print_imgbtn_speed) return ID_SP_SPINDLE_SPEED;
    else if(obj == print_src.print_imgbtn_rapid) return ID_SP_RAPID_SPEED;
}


static void sp_img_set(uint8_t num, bool status) {

    switch(num) {
        case 0:
            if(status) lv_img_set_src(img_add, &png_sp_add);
            else lv_img_set_src(img_add, &png_sp_add_pre);
        break;

        case 1:
            if(status) lv_img_set_src(img_dec, &png_sp_dec);
            else lv_img_set_src(img_dec, &png_sp_dec_pre);
        break;

        case 2: 
            if(status) lv_img_set_src(img_confirm, &png_sp_comfirm);
            else lv_img_set_src(img_confirm, &png_sp_comfirm_pre);
        break;

        case 3: 
            if(status) lv_img_set_src(img_back, &png_sp_back);
            else lv_img_set_src(img_back, &png_sp_back_per);
        break;
    }
}

static void sp_list_select(uint8_t num) {

    switch(num) {
        case 0: 
            sp_select = 0;
            lv_btn_set_style(print_src.print_imgbtn_pwr, LV_BTN_STYLE_REL, &print_src.print_mm_btn2_style);
            lv_btn_set_style(print_src.print_imgbtn_pwr, LV_BTN_STYLE_PR, &print_src.print_mm_btn2_style);
            lv_btn_set_style(print_src.print_imgbtn_speed, LV_BTN_STYLE_REL, &print_src.print_mm_btn1_style);
            lv_btn_set_style(print_src.print_imgbtn_speed, LV_BTN_STYLE_PR, &print_src.print_mm_btn1_style);
            lv_btn_set_style(print_src.print_imgbtn_rapid, LV_BTN_STYLE_REL, &print_src.print_mm_btn1_style);
            lv_btn_set_style(print_src.print_imgbtn_rapid, LV_BTN_STYLE_PR, &print_src.print_mm_btn1_style);
        break;
        
        case 1: 
            sp_select = 1;
            lv_btn_set_style(print_src.print_imgbtn_pwr, LV_BTN_STYLE_REL, &print_src.print_mm_btn1_style);
            lv_btn_set_style(print_src.print_imgbtn_pwr, LV_BTN_STYLE_PR, &print_src.print_mm_btn1_style);
            lv_btn_set_style(print_src.print_imgbtn_speed, LV_BTN_STYLE_REL, &print_src.print_mm_btn2_style);
            lv_btn_set_style(print_src.print_imgbtn_speed, LV_BTN_STYLE_PR, &print_src.print_mm_btn2_style);
            lv_btn_set_style(print_src.print_imgbtn_rapid, LV_BTN_STYLE_REL, &print_src.print_mm_btn1_style);
            lv_btn_set_style(print_src.print_imgbtn_rapid, LV_BTN_STYLE_PR, &print_src.print_mm_btn1_style);
        break;

        case 2: 
            sp_select = 2;
            lv_btn_set_style(print_src.print_imgbtn_pwr, LV_BTN_STYLE_REL, &print_src.print_mm_btn1_style);
            lv_btn_set_style(print_src.print_imgbtn_pwr, LV_BTN_STYLE_PR, &print_src.print_mm_btn1_style);
            lv_btn_set_style(print_src.print_imgbtn_speed, LV_BTN_STYLE_REL, &print_src.print_mm_btn1_style);
            lv_btn_set_style(print_src.print_imgbtn_speed, LV_BTN_STYLE_PR, &print_src.print_mm_btn1_style);
            lv_btn_set_style(print_src.print_imgbtn_rapid, LV_BTN_STYLE_REL, &print_src.print_mm_btn2_style);
            lv_btn_set_style(print_src.print_imgbtn_rapid, LV_BTN_STYLE_PR, &print_src.print_mm_btn2_style);
        break;
    }
}

static void sp_step_select(void) {

    if(sp_select != 2) {
        if(sp_step == 1) sp_step = 10;
        else if(sp_step == 10) sp_step = 20;
        else if(sp_step == 20) sp_step = 1;
        else if(sp_step == 25) sp_step = 1;
    }else {
        sp_step = 25;
    }
    sprintf(persen_dis_str, "%d%%", sp_step);
    lv_label_set_text(label_persen, persen_dis_str);
}

static void sp_add_dec(uint8_t num, uint8_t step, bool dir) {

    int step_get;

    if(dir == true) step_get = step;
    else step_get = -step;

    if(num == 0) {  

        print_setting.cur_spindle_pwr  += step_get;

        if(step > print_setting.cur_spindle_pwr) print_setting.cur_spindle_pwr = SpindleSpeedOverride::Min;

        if(print_setting.cur_spindle_pwr >= SpindleSpeedOverride::Max) {
            print_setting.cur_spindle_pwr = SpindleSpeedOverride::Max;
        } 
        else if(print_setting.cur_spindle_pwr <= SpindleSpeedOverride::Min) {
            print_setting.cur_spindle_pwr = SpindleSpeedOverride::Min;
        }   

        sprintf(spindle_speed_dis_str, "Spindle speed: %d%%", print_setting.cur_spindle_pwr);
        lv_label_set_text(label_spindle_speed, spindle_speed_dis_str);
    }
    else if(num == 1) {

        print_setting.cur_spindle_speed  += step_get;

        if(step > print_setting.cur_spindle_speed) print_setting.cur_spindle_speed = FeedOverride::Min;

        if(print_setting.cur_spindle_speed >= FeedOverride::Max) {
            print_setting.cur_spindle_speed = FeedOverride::Max;
        } 
        else if(print_setting.cur_spindle_speed <= FeedOverride::Min) {
            print_setting.cur_spindle_speed = FeedOverride::Min;
        } 

        sprintf(feed_rate_dis_str, "Feed rate: %d%%", print_setting.cur_spindle_speed);
        lv_label_set_text(label_feed_rate, feed_rate_dis_str);
    }
    else if(num == 2) {

        print_setting.cur_spindle_rapid  += step_get;

        if(step > print_setting.cur_spindle_rapid) print_setting.cur_spindle_rapid = RapidOverride::Low;

        if(print_setting.cur_spindle_rapid >= RapidOverride::Default) {
            print_setting.cur_spindle_rapid = RapidOverride::Default;
        } 
        else if(print_setting.cur_spindle_rapid <= RapidOverride::Low) {
            print_setting.cur_spindle_rapid = RapidOverride::Low;
        } 

        sprintf(rapid_dis_str, "Rapid speed: %d%%", print_setting.cur_spindle_rapid);
        lv_label_set_text(label_rapid_speed, rapid_dis_str);
    }
}

static void set_comfirm(uint8_t num) {
    grbl_sendf(CLIENT_SERIAL, "Set num:%d\n", num);
    if(num == 0) sys_rt_s_override = print_setting.cur_spindle_pwr;
    else if(num == 1) sys_rt_f_override = print_setting.cur_spindle_speed;
    else if(num == 2) sys_rt_r_override = print_setting.cur_spindle_rapid;
}



static void event_handler_sp(lv_obj_t* obj, lv_event_t event) {

    uint8_t id = get_sp_event_id(obj);

    if(event == LV_EVENT_PRESSED) { 

        switch (id) {
            case ID_SP_ADD: sp_img_set(0, false); break;
            case ID_SP_DEC: sp_img_set(1, false); break;
            case ID_SP_CONFIRM: sp_img_set(2, false); break;
            case ID_SP_BACK: sp_img_set(3, false); break;
        }
    }

    if((event == LV_EVENT_RELEASED) || (event == LV_EVENT_PRESS_LOST))  {

        switch (id) {
            case ID_SP_ADD:
                sp_img_set(0, true); 
                sp_add_dec(sp_select, sp_step, true);
            break;
            case ID_SP_DEC: 
                sp_img_set(1, true); 
                sp_add_dec(sp_select, sp_step, false);
            break;
            case ID_SP_CONFIRM: 
                sp_img_set(2, true); 
                set_comfirm(sp_select);
                sp_popup_del();
            break;
            case ID_SP_BACK: 
                sp_popup_del();
            break;

            case ID_SP_FEED_RATE: sp_list_select(0); if(sp_step == 25) sp_step_select(); break;
            case ID_SP_SPINDLE_SPEED: sp_list_select(1); if(sp_step == 25) sp_step_select();  break;
            case ID_SP_RAPID_SPEED: sp_list_select(2); sp_step_select(); break;

            case ID_SP_PERSEM: sp_step_select(); break;
        }
    }
}

void draw_adj_popup(void) {

    sp_select = 0;
    sp_step = 1;

    sprintf(persen_dis_str, "%d%%", sp_step);

    print_src.print_pwr_speed_src = lv_obj_create(mks_global.mks_src, NULL);
    lv_obj_set_size(print_src.print_pwr_speed_src, 360, 220);
    lv_obj_set_pos(print_src.print_pwr_speed_src, 60, 50);

    lv_style_copy(&print_src.printf_popup_style, &lv_style_scr);
    print_src.printf_popup_style.body.main_color = LV_COLOR_MAKE(0xCE, 0xD6, 0xE5);
    print_src.printf_popup_style.body.grad_color = LV_COLOR_MAKE(0xCE, 0xD6, 0xE5);
    print_src.printf_popup_style.text.color = LV_COLOR_BLACK;
    print_src.printf_popup_style.body.radius = 17;
    lv_obj_set_style(print_src.print_pwr_speed_src, &print_src.printf_popup_style);

    lv_style_copy(&print_src.print_mm_btn1_style, &lv_style_scr);
    print_src.print_mm_btn1_style.body.main_color = LV_COLOR_MAKE(0xCE, 0xD6, 0xE5);
    print_src.print_mm_btn1_style.body.grad_color = LV_COLOR_MAKE(0xCE, 0xD6, 0xE5);
    print_src.print_mm_btn1_style.body.opa = LV_OPA_COVER;//设置背景色完全不透明
    print_src.print_mm_btn1_style.body.border.width = 1;
    print_src.print_mm_btn1_style.body.border.color = LV_COLOR_MAKE(0x3F, 0x46, 0x66);
    print_src.print_mm_btn1_style.text.color =  LV_COLOR_MAKE(0x3F, 0x46, 0x66);;
    print_src.print_mm_btn1_style.body.radius = 10;

    lv_style_copy(&print_src.print_mm_btn2_style, &lv_style_scr);
    print_src.print_mm_btn2_style.body.main_color = LV_COLOR_MAKE(0x3F, 0x46, 0x66);
    print_src.print_mm_btn2_style.body.grad_color = LV_COLOR_MAKE(0x3F, 0x46, 0x66);
    print_src.print_mm_btn2_style.body.opa = LV_OPA_COVER;//设置背景色完全不透明
    print_src.print_mm_btn2_style.text.color = LV_COLOR_WHITE;
    print_src.print_mm_btn2_style.body.radius = 10; 

    print_src.print_sp_imgbtn_add = mks_lv_btn_set(print_src.print_pwr_speed_src, print_src.print_sp_imgbtn_add, 60, 60, 290, 10, event_handler_sp);
    print_src.print_sp_imgbtn_dec = mks_lv_btn_set(print_src.print_pwr_speed_src, print_src.print_sp_imgbtn_dec, 60, 60, 290, 80, event_handler_sp);
    print_src.print_btn_1_mm = mks_lv_btn_set(print_src.print_pwr_speed_src, print_src.print_btn_1_mm, 60, 60, 290, 150, event_handler_sp);
    print_src.print_sp_btn_sure = mks_lv_btn_set(print_src.print_pwr_speed_src, print_src.print_sp_btn_sure, 100, 60, 180, 150, event_handler_sp);
    print_src.print_sp_btn_return = mks_lv_btn_set(print_src.print_pwr_speed_src, print_src.print_sp_btn_return, 100, 60, 10, 150, event_handler_sp);

    print_src.print_imgbtn_pwr = mks_lv_btn_set(print_src.print_pwr_speed_src, print_src.print_imgbtn_pwr, 270, 40, 10, 10, event_handler_sp);
    print_src.print_imgbtn_speed = mks_lv_btn_set(print_src.print_pwr_speed_src, print_src.print_imgbtn_speed, 270, 40, 10, 56, event_handler_sp);
    print_src.print_imgbtn_rapid = mks_lv_btn_set(print_src.print_pwr_speed_src, print_src.print_imgbtn_rapid, 270, 40, 10, 100, event_handler_sp);
    
    
    sprintf(feed_rate_dis_str, "Feed rate: %d%%",  print_setting.cur_spindle_speed);
    sprintf(spindle_speed_dis_str, "Spindle speed: %d%%", print_setting.cur_spindle_pwr);
    sprintf(rapid_dis_str, "Rapid speed: %d%%", print_setting.cur_spindle_rapid);
    
    label_feed_rate = label_for_text(print_src.print_pwr_speed_src, label_feed_rate, print_src.print_imgbtn_speed, 10, 0, LV_ALIGN_IN_LEFT_MID,  feed_rate_dis_str);
    label_spindle_speed = label_for_text(print_src.print_pwr_speed_src, label_spindle_speed, print_src.print_imgbtn_pwr, 10, 0, LV_ALIGN_IN_LEFT_MID,  spindle_speed_dis_str);
    label_rapid_speed = label_for_text(print_src.print_pwr_speed_src, label_rapid_speed, print_src.print_imgbtn_rapid, 10, 0, LV_ALIGN_IN_LEFT_MID,  rapid_dis_str);

    label_for_text(print_src.print_pwr_speed_src, label_back, print_src.print_sp_btn_return, 0, 0, LV_ALIGN_IN_BOTTOM_MID,     "Back");
    label_for_text(print_src.print_pwr_speed_src, label_confirm, print_src.print_sp_btn_sure, 0, 0, LV_ALIGN_IN_BOTTOM_MID,    "Confirm");
    label_for_text(print_src.print_pwr_speed_src, label_add, print_src.print_sp_imgbtn_add, 0, 0, LV_ALIGN_IN_BOTTOM_MID,      "Add");
    label_for_text(print_src.print_pwr_speed_src, label_dec, print_src.print_sp_imgbtn_dec, 0, 0, LV_ALIGN_IN_BOTTOM_MID,      "Reduce");
    label_persen = label_for_text(print_src.print_pwr_speed_src, label_persen, print_src.print_btn_1_mm, 0, 0, LV_ALIGN_CENTER, persen_dis_str);

    img_add = mks_lvgl_img_set_algin(print_src.print_pwr_speed_src ,img_add, &png_sp_add, LV_ALIGN_IN_TOP_LEFT, 308, 18);
    img_dec = mks_lvgl_img_set_algin(print_src.print_pwr_speed_src ,img_dec, &png_sp_dec, LV_ALIGN_IN_TOP_LEFT, 308, 88);
    img_confirm   = mks_lvgl_img_set_algin(print_src.print_pwr_speed_src ,img_confirm, &png_sp_comfirm, LV_ALIGN_IN_TOP_LEFT, 218, 160);
    img_back = mks_lvgl_img_set_algin(print_src.print_pwr_speed_src ,img_back, &png_sp_back, LV_ALIGN_IN_TOP_LEFT, 48, 160);


    lv_btn_set_style(print_src.print_sp_imgbtn_add, LV_BTN_STYLE_REL, &print_src.print_mm_btn1_style);
    lv_btn_set_style(print_src.print_sp_imgbtn_add, LV_BTN_STYLE_PR, &print_src.print_mm_btn2_style);

    lv_btn_set_style(print_src.print_sp_imgbtn_dec, LV_BTN_STYLE_REL, &print_src.print_mm_btn1_style);
    lv_btn_set_style(print_src.print_sp_imgbtn_dec, LV_BTN_STYLE_PR, &print_src.print_mm_btn2_style);

    lv_btn_set_style(print_src.print_btn_1_mm, LV_BTN_STYLE_REL, &print_src.print_mm_btn1_style);
    lv_btn_set_style(print_src.print_btn_1_mm, LV_BTN_STYLE_PR, &print_src.print_mm_btn2_style);

    lv_btn_set_style(print_src.print_sp_btn_sure, LV_BTN_STYLE_REL, &print_src.print_mm_btn1_style);
    lv_btn_set_style(print_src.print_sp_btn_sure, LV_BTN_STYLE_PR, &print_src.print_mm_btn2_style);

    lv_btn_set_style(print_src.print_sp_btn_return, LV_BTN_STYLE_REL, &print_src.print_mm_btn1_style);
    lv_btn_set_style(print_src.print_sp_btn_return, LV_BTN_STYLE_PR, &print_src.print_mm_btn2_style);

    lv_btn_set_style(print_src.print_imgbtn_pwr, LV_BTN_STYLE_REL, &print_src.print_mm_btn2_style);
    lv_btn_set_style(print_src.print_imgbtn_pwr, LV_BTN_STYLE_PR, &print_src.print_mm_btn2_style);

    lv_btn_set_style(print_src.print_imgbtn_speed, LV_BTN_STYLE_REL, &print_src.print_mm_btn1_style);
    lv_btn_set_style(print_src.print_imgbtn_speed, LV_BTN_STYLE_PR, &print_src.print_mm_btn1_style);

    lv_btn_set_style(print_src.print_imgbtn_rapid, LV_BTN_STYLE_REL, &print_src.print_mm_btn1_style);
    lv_btn_set_style(print_src.print_imgbtn_rapid, LV_BTN_STYLE_PR, &print_src.print_mm_btn1_style);
}

void sp_popup_del(void) {
    set_print_click(true);
    lv_obj_del(print_src.print_pwr_speed_src);
}


void set_print_click(bool status) {
    lv_obj_set_click(print_src.print_imgbtn_suspend, status);
    lv_obj_set_click(print_src.print_imgbtn_stop , status);
    lv_obj_set_click(print_src.print_imgbtn_adj , status);
}


char pl_info[128];
void mks_print_data_updata(void) {

    sprintf(print_data_updata.print_pwr_str, "S:%d%%", sys_rt_s_override);
    print_src.print_Label_power = mks_lv_label_updata(print_src.print_Label_power, print_data_updata.print_pwr_str);

    sprintf(print_data_updata.print_speed_str, "F:%2d%%", sys_rt_f_override);  
    print_src.print_Label_caveSpeed = mks_lv_label_updata(print_src.print_Label_caveSpeed, print_data_updata.print_speed_str);

    sprintf(print_data_updata.print_rapid_str, "R:%2d%%", sys_rt_r_override);
    print_src.print_Label_caveR = mks_lv_label_updata(print_src.print_Label_caveR, print_data_updata.print_rapid_str);

    if (SD_ready_next == false) {
        if (mks_grbl.is_mks_ts35_flag == true) {
            mks_print_bar_updata();
        }
    }
}

uint8_t get_print_speed(void) {
    
    return sys_rt_f_override;
}

uint8_t get_print_power(void) {

    return sys_rt_s_override;
}


void mks_clear_print(void) {
    lv_obj_clean(mks_global.mks_src);
}

void mks_del_obj(lv_obj_t *obj) { 
    lv_obj_del(obj);
}