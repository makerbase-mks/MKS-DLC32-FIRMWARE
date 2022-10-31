#include "MKS_draw_language.h"

mc_lg_muilt_t mc_language;
LANGUAGE_PAGE_T language_page;

void mc_language_init(void) {

	if(mks_grbl.language = SimpleChinese) {

		 
	}
	else if(mks_grbl.language = English) {
		mc_language.back = BACK_EN;
		mc_language.yes = YES_EN;
		mc_language.no = NO_EN;
		
		mc_language.control = CONTROL_EN;
		mc_language.sculpture = SCULPTURE_EN;
		mc_language.tool = TOOL_EN;
		mc_language.Mpos = MPOS_EN;
		mc_language.Wpos = WPOS_EN;
		mc_language.wifi_connect = WIFI_CONNECT_EN;
		mc_language.wifi_disconnect = WIFI_DISCONNECT_EN;
		
		mc_language.xy_clear = XY_CLEAR_EN;
		mc_language.z_clear = Z_CLEAR_EN;
		mc_language.knife = KNIFE_EN;
		mc_language.next = NEXT_EN;
		mc_language.up = UP_EN;
		mc_language.cooling = COOLING_EN;
		mc_language.position = POSITION_EN;
		mc_language.speed_high = SPEED_HIGH_EN;
		mc_language.speed_mid = SPEED_MID_EN; 
		mc_language.speed_low = SPEED_LOW_EN;
		mc_language.spindle = SPINDLE_EN;
		mc_language.carve = CARVE_EN;

		mc_language.dis_no_sd_card = DIS_NO_SDCARD_EN;
		
		mc_language.hold = HOLD_EN;
		mc_language.cycle = CYCLE_EN;
		mc_language.stop = STOP_EN;
		mc_language.adjust = ADJUST_EN;

		mc_language.dis_stop_print = DIS_STOP_CARVE_EN;
		mc_language.dis_homing = DIS_HOMEING_EN;
		mc_language.dis_no_hard_homing = DIS_NO_HARD_HOME_EN;
		mc_language.dis_homing_succeed = DIS_HOME_SUCCEED_EN;
		mc_language.dis_homing_fail = DIS_HOME_FAIL_EN;
		mc_language.dis_probe_set = DIS_PROBE_SET;
		mc_language.dis_probe_succeed = DIS_PROBE_SECCEED_EN;
		mc_language.dis_probe_fail = DIS_PROBE_FAIL_EN;
	}
}


enum {
	ID_L_BACK,
	ID_L_CN,
	ID_L_EN,
	ID_L_DE,
};

static uint8_t get_event(lv_obj_t* obj) {

    if(obj == language_page.imgbtn_back)                		return ID_L_BACK;
    else if(obj == language_page.imgbtn_simple_cn)           	return ID_L_CN;
	else if(obj == language_page.imgbtn_en)						return ID_L_EN;
	else if(obj == language_page.imgbtn_de)						return ID_L_DE;
}

/*
 *	language num:
 *	0------cn
 *	1------en
 *	2------de
 *	3...
*/
static void set_language(uint8_t language) {

	switch(language) {

		case 0:
			mks_grbl.language = SimpleChinese;
		break;

		case 1:
			mks_grbl.language = English;
		break;

		case 2:
			mks_grbl.language = Deutsch;
		break;
	}
	mc_language_init();
}

static void enent_handler_back(void) {

	mks_clear_language();
	mks_ui_page.mks_ui_page = MKS_UI_PAGE_LOADING;
	mks_draw_tool();
}

void set_language_btn_style(uint8_t language) {

	switch (language) {

		case 0: 
			lv_btn_set_style(language_page.imgbtn_simple_cn, LV_BTN_STYLE_REL, &mks_global.language_btn_pr_style);
			lv_btn_set_style(language_page.imgbtn_simple_cn,LV_BTN_STYLE_PR, &mks_global.language_btn_pr_style);

			lv_btn_set_style(language_page.imgbtn_en, LV_BTN_STYLE_REL, &mks_global.language_btn_rel_style);
			lv_btn_set_style(language_page.imgbtn_en,LV_BTN_STYLE_PR, &mks_global.language_btn_rel_style);
		break;

		case 1: 
			lv_btn_set_style(language_page.imgbtn_simple_cn, LV_BTN_STYLE_REL, &mks_global.language_btn_rel_style);
			lv_btn_set_style(language_page.imgbtn_simple_cn,LV_BTN_STYLE_PR, &mks_global.language_btn_rel_style);

			lv_btn_set_style(language_page.imgbtn_en, LV_BTN_STYLE_REL, &mks_global.language_btn_pr_style);
			lv_btn_set_style(language_page.imgbtn_en,LV_BTN_STYLE_PR, &mks_global.language_btn_pr_style);
		break;

		case 2: 
			lv_btn_set_style(language_page.imgbtn_simple_cn, LV_BTN_STYLE_REL, &mks_global.language_btn_rel_style);
			lv_btn_set_style(language_page.imgbtn_simple_cn,LV_BTN_STYLE_PR, &mks_global.language_btn_rel_style);

			lv_btn_set_style(language_page.imgbtn_en, LV_BTN_STYLE_REL, &mks_global.language_btn_rel_style);
			lv_btn_set_style(language_page.imgbtn_en,LV_BTN_STYLE_PR, &mks_global.language_btn_rel_style);
		break;
	}
}


char str_language_ch[10] = "$40=0\n";
char str_language_en[10] = "$40=1\n";
char str_language_de[10] = "$40=2\n"; 
static void event_handler(lv_obj_t* obj, lv_event_t event) {

	uint8_t id = get_event(obj);

    if(event == LV_EVENT_PRESSED) { ts35_beep_on(); }

    if((event == LV_EVENT_RELEASED) || (event == LV_EVENT_PRESS_LOST))  {

        ts35_beep_off();

        switch(id) {
			case ID_L_BACK: enent_handler_back(); break;
			case ID_L_CN: 
				set_language(0); 
				set_language_btn_style(0);
				MKS_GRBL_CMD_SEND(str_language_ch);
			break;
			case ID_L_EN: 
				set_language(1); 
				set_language_btn_style(1);
				MKS_GRBL_CMD_SEND(str_language_en);
			break;
			case ID_L_DE: 
				set_language(2); 
				set_language_btn_style(2);
				MKS_GRBL_CMD_SEND(str_language_de);
			break;
        }
    }
}

void draw_language(void) {

	uint8_t language_num = 0;

	mks_global.mks_src_1 = lv_obj_create(mks_global.mks_src, NULL);
	lv_obj_set_size(mks_global.mks_src_1, about_src1_x_size, about_src1_y_size);
    lv_obj_set_pos(mks_global.mks_src_1, about_src1_x, about_src1_y);
    lv_obj_set_style(mks_global.mks_src_1, &mks_global.mks_src_1_style);

	// tool_page.imgbtn_back = lv_imgbtn_creat_mks(mks_global.mks_src_1, tool_page.imgbtn_back, &png_back_pre, &back, LV_ALIGN_IN_LEFT_MID, 10, -15, event_handler);

	language_page.imgbtn_back = lv_imgbtn_creat_mks(mks_global.mks_src_1, language_page.imgbtn_back, &png_back_pre, &back, LV_ALIGN_IN_LEFT_MID, 10, -15, event_handler);
	language_page.imgbtn_simple_cn = mks_lv_btn_set(mks_global.mks_src, language_page.imgbtn_simple_cn, 130, 50, 20, 110, event_handler);
	language_page.imgbtn_en = mks_lv_btn_set(mks_global.mks_src, language_page.imgbtn_en, 130, 50, 20, 110 + 60, event_handler);

	language_num = language_select->get();

	set_language_btn_style(language_num);
	
	label_for_imgbtn_name(mks_global.mks_src, language_page.label_back, language_page.imgbtn_back, 0, 0, "back");
	label_for_btn_name(language_page.imgbtn_simple_cn, language_page.label_simple_cn , 0, 0, "中文");
	label_for_btn_name(language_page.imgbtn_en, language_page.label_en, 0, 0, "English");
	mks_ui_page.mks_ui_page = MKS_UI_LANGUAGE;
}

void mks_clear_language(void) {
    lv_obj_clean(mks_global.mks_src);
}

