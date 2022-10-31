#include "MKS_draw_ready.h"

MKS_PAGE_READY ready_src;
lv_style_t bkl_color;    // main

static void disp_imgbtn(void);
static void disp_label(void);
static void disp_img(void);

enum {
    ID_R_CONTRL,
    ID_R_SCULPTURE,
    ID_R_TOOL,
    ID_R_NONE,
};

static uint8_t get_event(lv_obj_t* obj) {
    if (obj == ready_src.ready_imgbtn_Control)         return ID_R_CONTRL;
    else if (obj == ready_src.ready_imgbtn_Sculpture)       return ID_R_SCULPTURE;
    else if (obj == ready_src.ready_imgbtn_Tool)            return ID_R_TOOL;
    else if (obj == ready_src.ready_imgbtn_wifi_status )    return ID_R_NONE;
}


static void event_handler_none(lv_obj_t* obj, lv_event_t event) { 
	
	if (event == LV_EVENT_RELEASED) {

	}
}

static void event_handler(lv_obj_t* obj, lv_event_t event) {

    uint8_t id = get_event(obj);

    if(event == LV_EVENT_PRESSED) { 
        // ts35_beep_on(); 
    }

    mks_ui_page.mks_ui_page = MKS_UI_PAGE_LOADING;
    if((event == LV_EVENT_RELEASED) || (event == LV_EVENT_PRESS_LOST))  {

        if((id != ID_R_NONE)) {
            mks_lv_clean_ui();
        }
        
        switch(id) {
            // case ID_R_ADJUST:        mks_draw_power(); break;
            case ID_R_CONTRL:         mks_draw_move();; break;
            case ID_R_SCULPTURE:     
                file_popup_select_flag = false;
                mks_draw_craving();;
            break;
            case ID_R_TOOL:     mks_draw_tool();  break;
#ifdef ENABLE_WIFI
            // case ID_R_WIFI:  
            //     mks_grbl.wifi_back_from = 0;
            //     mks_draw_wifi(); 
            // break;
#endif
            case ID_R_NONE: break;
        }
    }
}

lv_obj_t *logo;
uint32_t logo_count = 0;
void mks_draw_logo(void) {
    mks_ui_page.mks_ui_page = MKS_UI_Logo;      
    mks_global.mks_src = lv_obj_create(NULL, NULL);
    mks_global.mks_src = lv_scr_act();
    lv_obj_set_style(mks_global.mks_src ,&mks_global.mks_src_style);
    logo = mks_lvgl_img_set(mks_global.mks_src, logo, &mks_logo, 0 ,0);
}

void mks_draw_ready(void) {

    mks_ui_page.mks_ui_page = MKS_UI_PAGE_LOADING;

    mks_global.mks_src_1 = lv_obj_create(mks_global.mks_src, NULL);
    lv_obj_set_size(mks_global.mks_src_1, READY_src1_x_size, READY_src1_y_size);
    lv_obj_set_pos(mks_global.mks_src_1, READY_src1_x, READY_src1_y);
    lv_obj_set_style(mks_global.mks_src_1 ,&mks_global.mks_src_1_style);

    disp_imgbtn();
    disp_img();
    disp_label();

    mks_ui_page.mks_ui_page = MKS_UI_Ready;
}

static void disp_imgbtn(void) {

    if (mks_get_wifi_status() == false){ 
        ready_src.ready_imgbtn_wifi_status = lv_imgbtn_creat_n_mks(mks_global.mks_src ,ready_src.ready_imgbtn_wifi_status, &png_wifi_disconnect, &png_wifi_disconnect, 0, 0, event_handler);
    }
    else{
        ready_src.ready_imgbtn_wifi_status = lv_imgbtn_creat_n_mks(mks_global.mks_src ,ready_src.ready_imgbtn_wifi_status, &png_wifi_connect, &png_wifi_connect, 0, 0, event_handler);
    }

    ready_src.ready_imgbtn_Control = lv_imgbtn_creat_mks(mks_global.mks_src_1, ready_src.ready_imgbtn_Control, &png_ctrl_pre, &Control, LV_ALIGN_IN_TOP_LEFT,102, 10, event_handler);
    ready_src.ready_imgbtn_Sculpture = lv_imgbtn_creat_mks(mks_global.mks_src_1, ready_src.ready_imgbtn_Sculpture, &png_file_pre, &Sculpture, LV_ALIGN_IN_TOP_LEFT, 202, 10, event_handler);
    ready_src.ready_imgbtn_Tool = lv_imgbtn_creat_mks(mks_global.mks_src_1, ready_src.ready_imgbtn_Tool, &png_tool_pre, &Tool, LV_ALIGN_IN_TOP_LEFT, 302, 10, event_handler);
}

static void disp_img(void) {

    ready_src.ready_img_wpos = mks_lvgl_img_set_algin(mks_global.mks_src ,ready_src.ready_img_wpos, &png_w_pos, LV_ALIGN_IN_TOP_LEFT, 20, 40);
    ready_src.ready_img_mpos = mks_lvgl_img_set_algin(mks_global.mks_src ,ready_src.ready_img_mpos, &png_m_pos, LV_ALIGN_IN_TOP_LEFT, 250, 40);
}


static void disp_label(void) {

    label_for_imgbtn_name(mks_global.mks_src_1, ready_src.ready_label_Control, ready_src.ready_imgbtn_Control, 0, 0, mc_language.control);
    label_for_imgbtn_name(mks_global.mks_src_1, ready_src.ready_label_Sculpture, ready_src.ready_imgbtn_Sculpture, 0, 0, mc_language.sculpture);
    label_for_imgbtn_name(mks_global.mks_src_1, ready_src.ready_label_Tool, ready_src.ready_imgbtn_Tool, 0, 0, mc_language.tool);

    label_for_text(mks_global.mks_src, ready_src.ready_label_mpos, ready_src.ready_img_mpos, 0, 0, LV_ALIGN_OUT_RIGHT_MID, mc_language.Mpos);
    label_for_text(mks_global.mks_src, ready_src.ready_label_wpos, ready_src.ready_img_wpos, 0, 0, LV_ALIGN_OUT_RIGHT_MID, mc_language.Wpos);


    ready_src.ready_label_xpos = label_for_text(mks_global.mks_src,   ready_src.ready_label_xpos, NULL , 40, 86,  LV_ALIGN_IN_TOP_LEFT, "X:0");
    ready_src.ready_label_ypos = label_for_text(mks_global.mks_src,   ready_src.ready_label_ypos, NULL , 40, 126, LV_ALIGN_IN_TOP_LEFT, "Y:0");
    ready_src.ready_label_zpos = label_for_text(mks_global.mks_src,   ready_src.ready_label_zpos, NULL , 40, 166, LV_ALIGN_IN_TOP_LEFT, "Z:0");

    ready_src.ready_label_m_xpos = label_for_text(mks_global.mks_src,   ready_src.ready_label_m_xpos, NULL , 280, 86,  LV_ALIGN_IN_TOP_LEFT, "X:0");
    ready_src.ready_label_m_ypos = label_for_text(mks_global.mks_src,   ready_src.ready_label_m_ypos, NULL , 280, 126, LV_ALIGN_IN_TOP_LEFT, "Y:0");
    ready_src.ready_label_m_zpos = label_for_text(mks_global.mks_src,   ready_src.ready_label_m_zpos, NULL , 280, 166, LV_ALIGN_IN_TOP_LEFT, "Z:0");


    #if defined(ENABLE_WIFI)
        if (mks_get_wifi_status() == false){ 
            ready_src.ready_label_wifi_status = label_for_text(mks_global.mks_src, ready_src.ready_label_wifi_status, NULL, 34, 6, LV_ALIGN_IN_TOP_LEFT, mc_language.wifi_disconnect);
        }else {
            ready_src.ready_label_wifi_status = label_for_text(mks_global.mks_src, ready_src.ready_label_wifi_status, NULL, 34, 6, LV_ALIGN_IN_TOP_LEFT, mc_language.wifi_connect);
        }
    #else 
        // ready_src.ready_label_wifi_status = mks_lv_static_label(ready_src.ready_btn_wifi, ready_src.ready_label_wifi_status, 40, 0, "Disconnect", 110);
        ready_src.ready_label_wifi_status = label_for_btn_name(ready_src.ready_btn_wifi, ready_src.ready_label_wifi_status, 0, 0, "WIFI:Disconnect");
    #endif
}

char xpos_str[50] = "X:0.0";
char ypos_str[50] = "Y:0.0";
char zpos_str[50] = "Z:0.0";

char m_xpos_str[50] = "X:0.0";
char m_ypos_str[50] = "Y:0.0";
char m_zpos_str[50] = "Z:0.0";

char wifi_status_str[50];
char wifi_ip_str[100];

void mks_widi_show_ip(IPAddress ip, uint8_t p) { 
    if(p) {
        strcat(wifi_ip_str, ip.toString().c_str());
    }else {
    }
}


void ready_data_updata(void) {

    static uint8_t wifi_ref_count = 0;
    static float mks_print_position[MAX_N_AXIS];
    float* print_position = system_get_mpos();

    sprintf(xpos_str, "X:%.1f", print_position[0]);
    sprintf(ypos_str, "Y:%.1f", print_position[1]);
    sprintf(zpos_str, "Z:%.1f", print_position[2]);

    lv_label_set_static_text(ready_src.ready_label_m_xpos, xpos_str);
    lv_label_set_static_text(ready_src.ready_label_m_ypos, ypos_str);
    lv_label_set_static_text(ready_src.ready_label_m_zpos, zpos_str);

    mpos_to_wpos(print_position);
    sprintf(m_xpos_str, "X:%.1f", print_position[0]);
    sprintf(m_ypos_str, "Y:%.1f", print_position[1]);
    sprintf(m_zpos_str, "Z:%.1f", print_position[2]);

    lv_label_set_static_text(ready_src.ready_label_xpos, m_xpos_str);
    lv_label_set_static_text(ready_src.ready_label_ypos, m_ypos_str);
    lv_label_set_static_text(ready_src.ready_label_zpos, m_zpos_str);
    
    #if defined(ENABLE_WIFI)
    if (mks_get_wifi_status() == false){
        ready_src.ready_label_wifi_status = mks_lv_label_updata(ready_src.ready_label_wifi_status, "WIFI:Disconnect");
    }
    else {
        ready_src.ready_label_wifi_status = mks_lv_label_updata(ready_src.ready_label_wifi_status, "WIFI:Connect");
    }
    #endif
}




