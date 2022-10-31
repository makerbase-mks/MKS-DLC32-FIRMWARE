#include "MKS_draw_tool.h"

lv_style_t about_src1_style;
lv_style_t btn_tool_style;
static lv_style_t style_line;

lv_obj_t *about_src1; 

lv_obj_t *tool_img_back; 
lv_obj_t *tool_img_wifi; 
lv_obj_t *tool_img_language; 

lv_obj_t *tool_label_line1; 
lv_obj_t *tool_label_line2; 
lv_obj_t *tool_label_line3; 
lv_obj_t *label_tool_back; 
lv_obj_t *label_tool_wifi; 
lv_obj_t *label_tool_language; 

lv_obj_t* label_board_version;
lv_obj_t* label_Firmware_version;
lv_obj_t* label_cpu_info;

lv_obj_t *tool_line1;
lv_obj_t *tool_line2;
lv_obj_t *tool_line3;

lv_point_t tool_line_points[3][2] = {

    { {about_first_line_x, about_first_line_y}, {about_first_line_x+460, about_first_line_y} },
    { {about_first_line_x, about_first_line_y+50}, {about_first_line_x+460, about_first_line_y+50} },
    { {about_first_line_x, about_first_line_y+100}, {about_first_line_x+460, about_first_line_y+100} },
};

LV_IMG_DECLARE(back);	
LV_IMG_DECLARE(wifi_tool);	
LV_IMG_DECLARE(png_back_pre);
LV_IMG_DECLARE(png_wifi_pre);

static void event_btn_tool_wifi(lv_obj_t* obj, lv_event_t event) {

    if (event == LV_EVENT_PRESSED) {
        mks_clear_tool();
        mks_grbl.wifi_back_from = 1;
        #if defined(ENABLE_WIFI)
            mks_draw_wifi();
        #endif
    }
}


static void event_btn_tool_back(lv_obj_t* obj, lv_event_t event) {

    if (event == LV_EVENT_PRESSED) {
        mks_clear_tool();
        mks_ui_page.mks_ui_page = MKS_UI_PAGE_LOADING;
        mks_ui_page.wait_count = DEFAULT_UI_COUNT;
        mks_draw_ready();
    }
}

static void event_btn_tool_language(lv_obj_t* obj, lv_event_t event) {

    if (event == LV_EVENT_PRESSED) {
        mks_clear_tool();
        mks_ui_page.mks_ui_page = MKS_UI_LANGUAGE;
        mks_ui_page.wait_count = DEFAULT_UI_COUNT;
        draw_language();
    }
}

void mks_draw_tool(void) {

    char cpu_info[128]="CPU:Freq:";
    mks_global.mks_src_1 = lv_obj_create(mks_global.mks_src, NULL);
	lv_obj_set_size(mks_global.mks_src_1, about_src1_x_size, about_src1_y_size);
    lv_obj_set_pos(mks_global.mks_src_1, about_src1_x, about_src1_y);
    lv_obj_set_style(mks_global.mks_src_1, &mks_global.mks_src_1_style);

    tool_img_back = lv_imgbtn_creat_mks(mks_global.mks_src_1, tool_img_back, &png_back_pre, &back, LV_ALIGN_IN_LEFT_MID, 10, -15, event_btn_tool_back);

#if defined(ENABLE_WIFI)
    tool_img_wifi = lv_imgbtn_creat_mks(mks_global.mks_src_1, tool_img_wifi, &png_wifi_pre, &wifi_tool, LV_ALIGN_IN_RIGHT_MID, -30, -15, event_btn_tool_wifi);
#endif

    tool_img_language = lv_imgbtn_creat_mks(mks_global.mks_src_1, tool_img_language, &png_language_pre, &png_language, LV_ALIGN_IN_RIGHT_MID, -100, -15, event_btn_tool_language);

    lv_style_copy(&style_line, &lv_style_plain);
    style_line.line.color = LV_COLOR_MAKE(0x00, 0x3b, 0x75);
    style_line.line.width = 1;
    style_line.line.rounded = 1;

    tool_line1 = mks_lv_set_line(mks_global.mks_src, tool_line1, tool_line_points[0]);
    lv_line_set_style(tool_line1, LV_LINE_STYLE_MAIN, &style_line);
    tool_line2 = mks_lv_set_line(mks_global.mks_src, tool_line2, tool_line_points[1]);
    lv_line_set_style(tool_line2, LV_LINE_STYLE_MAIN, &style_line);
    tool_line3 = mks_lv_set_line(mks_global.mks_src, tool_line3, tool_line_points[2]);
    lv_line_set_style(tool_line3, LV_LINE_STYLE_MAIN, &style_line);

    label_for_imgbtn_name(mks_global.mks_src_1, label_tool_back, tool_img_back, 0, 0, "Back");

#if defined(ENABLE_WIFI)
    label_for_imgbtn_name(mks_global.mks_src_1, label_tool_wifi, tool_img_wifi, 0, 0, "Wifi");
#endif

    label_for_imgbtn_name(mks_global.mks_src_1, label_tool_language, tool_img_language, 0, 0, "Language");
    

    mks_lvgl_long_sroll_label_with_wight_set_center(mks_global.mks_src, label_board_version, 10, 120, BOARD_NAME, 400);

    mks_lvgl_long_sroll_label_with_wight_set_center(mks_global.mks_src, label_Firmware_version, 10, 170, FW_NAME, 400);

    strcat(cpu_info, String(ESP.getCpuFreqMHz()).c_str());
    strcat(cpu_info, "MHz/ T:");
    strcat(cpu_info, String(temperatureRead(), 1).c_str());
    strcat(cpu_info, "C/ ID:");
    strcat(cpu_info, String((uint16_t)(ESP.getEfuseMac() >> 32)).c_str());
    mks_lvgl_long_sroll_label_with_wight_set_center(mks_global.mks_src, label_cpu_info, 10, 220, cpu_info, 400);
    mks_ui_page.mks_ui_page = MKS_UI_Tool; 
}

void mks_clear_tool(void) {
    lv_obj_clean(mks_global.mks_src);
}


