#include "MKS_draw_power.h"
#include "MKS_LVGL.h"

POWER_PAGE_T power_page;

LV_IMG_DECLARE(SP_H_UP);			//强光
LV_IMG_DECLARE(SPD_H_PRE);			//强光

LV_IMG_DECLARE(SP_L_UP);			//弱光
LV_IMG_DECLARE(SP_L_PRE);			//弱光
LV_IMG_DECLARE(SP_ON);				//开
LV_IMG_DECLARE(SP_OFF);				//关

LV_IMG_DECLARE(Calibration);			//Z回零
LV_IMG_DECLARE(back);			//返回
LV_IMG_DECLARE(png_back_pre);

enum {

	ID_P_HIGH,
	ID_P_MID,
	ID_P_LOW,
	ID_P_OFF,
	ID_P_BACK,
};

static uint8_t get_event(lv_obj_t* obj) {

    if      (obj == power_page.pwr_high)  	return ID_P_HIGH;
    else if (obj == power_page.pwr_low)     return ID_P_LOW;
    else if (obj == power_page.pwr_off)     return ID_P_OFF;
    else if (obj == power_page.Back) 		return ID_P_BACK;
}

static void event_pwr_on_off(void) {

	if(sys.state != State::Idle) {
		draw_pwr_popup_1("Please wait machine idle!");
		return ;
	}

	mks_grbl.power_persen = P_0_PERSEN;
	lv_imgbtn_set_src(power_page.pwr_off, LV_BTN_STATE_PR, &SP_ON);
	lv_imgbtn_set_src(power_page.pwr_off, LV_BTN_STATE_REL, &SP_ON);
	lv_imgbtn_set_src(power_page.pwr_high, LV_BTN_STATE_PR, &SP_H_UP);
	lv_imgbtn_set_src(power_page.pwr_high, LV_BTN_STATE_REL, &SP_H_UP);
	lv_imgbtn_set_src(power_page.pwr_low, LV_BTN_STATE_PR, &SP_L_UP);
	lv_imgbtn_set_src(power_page.pwr_low, LV_BTN_STATE_REL, &SP_L_UP);
	// MKS_GRBL_CMD_SEND("M3 S0\n");
	// MKS_GRBL_CMD_SEND("G0\n");
	set_power_value(0);
}

static void event_pwr_l(void) {


	if(sys.state != State::Idle) {
		draw_pwr_popup_1("Please wait machine idle!");
		return ;
	}

	mks_grbl.power_persen = P_1_PERSEN;
	lv_imgbtn_set_src(power_page.pwr_high, LV_BTN_STATE_PR, &SP_H_UP);
	lv_imgbtn_set_src(power_page.pwr_high, LV_BTN_STATE_REL, &SP_H_UP);
	lv_imgbtn_set_src(power_page.pwr_low, LV_BTN_STATE_PR, &SP_L_PRE);
	lv_imgbtn_set_src(power_page.pwr_low, LV_BTN_STATE_REL, &SP_L_PRE);
	lv_imgbtn_set_src(power_page.pwr_off, LV_BTN_STATE_PR, &SP_OFF);
	lv_imgbtn_set_src(power_page.pwr_off, LV_BTN_STATE_REL, &SP_OFF);
	// MKS_GRBL_CMD_SEND("M3 S50\n");
	// MKS_GRBL_CMD_SEND("G1 F1000\n");
	set_power_value(50);
}

static void event_pwr_h(void) {

	if(sys.state != State::Idle) {
		draw_pwr_popup_1("Please wait machine idle!");
		return ;
	}

	mks_grbl.power_persen = P_10_PERSEN;
	lv_imgbtn_set_src(power_page.pwr_high, LV_BTN_STATE_PR, &SPD_H_PRE);
	lv_imgbtn_set_src(power_page.pwr_high, LV_BTN_STATE_REL, &SPD_H_PRE);
	lv_imgbtn_set_src(power_page.pwr_low, LV_BTN_STATE_PR, &SP_L_UP);
	lv_imgbtn_set_src(power_page.pwr_low, LV_BTN_STATE_REL, &SP_L_UP);
	lv_imgbtn_set_src(power_page.pwr_off, LV_BTN_STATE_PR, &SP_OFF);
	lv_imgbtn_set_src(power_page.pwr_off, LV_BTN_STATE_REL, &SP_OFF);
	// MKS_GRBL_CMD_SEND("M3 S500\n");
	// MKS_GRBL_CMD_SEND("G1 F1000\n");
	set_power_value(500);
}

void set_power_value(uint16_t value) {

	uint8_t str[30];
	if(value != 0) sprintf((char *)str, "M3 S%d\n", value);
	else sprintf((char *)str, "M5\n");

	MKS_GRBL_CMD_SEND(str);
	MKS_GRBL_CMD_SEND("G1 F1000\n");
}

static void event_handler_cailb(lv_obj_t* obj, lv_event_t event) {

	if (event == LV_EVENT_RELEASED) {
		MKS_GRBL_CMD_SEND("$J=G91Z20F346\n");
	}
}

static void event_handler(lv_obj_t* obj, lv_event_t event) {

	if(event != LV_EVENT_RELEASED) return ;
    uint8_t id = get_event(obj);

	if(id == ID_P_BACK) {
		mks_ui_page.mks_ui_page = MKS_UI_PAGE_LOADING;
		mks_lv_clean_ui();
	}

	switch(id) {
		case ID_P_HIGH:event_pwr_h();	break;
		case ID_P_LOW: event_pwr_l();	break;
		case ID_P_OFF: event_pwr_on_off();	break;
		case ID_P_BACK: mks_draw_ready();	break;
	}
}


void mks_draw_power(void) {

	mks_global.mks_src_1 = lv_obj_create(mks_global.mks_src, NULL);
	lv_obj_set_size(mks_global.mks_src_1, 460, 90);
    lv_obj_set_pos(mks_global.mks_src_1, 10, 10);
		
	mks_global.mks_src_2 = lv_obj_create(mks_global.mks_src, NULL);
	lv_obj_set_size(mks_global.mks_src_2, 460, 200);
    lv_obj_set_pos(mks_global.mks_src_2, 10, 110);

	lv_obj_set_style(mks_global.mks_src_1, &mks_global.mks_src_1_style);
	lv_obj_set_style(mks_global.mks_src_2, &mks_global.mks_src_2_style);

	power_page.Back = lv_imgbtn_creat_mks(mks_global.mks_src_1, power_page.Back, &png_back_pre, &back, LV_ALIGN_IN_LEFT_MID, 10, -10, event_handler);

#if defined(USE_BL_TOUCH)
	cailb = lv_imgbtn_creat_mks(p_scr1, cailb, &Calibration, &Calibration, LV_ALIGN_CENTER,150, -10, event_handler_cailb);
#endif

	if(mks_grbl.power_persen == P_10_PERSEN) {
		power_page.pwr_high = lv_imgbtn_creat_n_mks(mks_global.mks_src_2, power_page.pwr_high, &SPD_H_PRE, &SPD_H_PRE,PWR_IMGBTN_HIGH_X_POS, PWR_IMGBTN_HIGH_Y_POS, event_handler);
		power_page.pwr_low = lv_imgbtn_creat_n_mks(mks_global.mks_src_2, power_page.pwr_low, &SP_L_UP, &SP_L_UP,PWR_IMGBTN_HIGH_X_POS+PWR_IMGBTN_OFFSET_X, PWR_IMGBTN_HIGH_Y_POS, event_handler);
		power_page.pwr_off = lv_imgbtn_creat_n_mks(mks_global.mks_src_2, power_page.pwr_low, &SP_OFF, &SP_OFF, PWR_IMGBTN_HIGH_X_POS+PWR_IMGBTN_OFFSET_X*2, PWR_IMGBTN_HIGH_Y_POS, event_handler);
	}
	else if(mks_grbl.power_persen == P_1_PERSEN) {
		power_page.pwr_high = lv_imgbtn_creat_n_mks(mks_global.mks_src_2, power_page.pwr_high, &SP_H_UP, &SP_H_UP,PWR_IMGBTN_HIGH_X_POS, PWR_IMGBTN_HIGH_Y_POS, event_handler);
		power_page.pwr_low = lv_imgbtn_creat_n_mks(mks_global.mks_src_2, power_page.pwr_low, &SP_L_PRE, &SP_L_PRE,PWR_IMGBTN_HIGH_X_POS+PWR_IMGBTN_OFFSET_X, PWR_IMGBTN_HIGH_Y_POS, event_handler);
		power_page.pwr_off = lv_imgbtn_creat_n_mks(mks_global.mks_src_2, power_page.pwr_low, &SP_OFF, &SP_OFF, PWR_IMGBTN_HIGH_X_POS+PWR_IMGBTN_OFFSET_X*2, PWR_IMGBTN_HIGH_Y_POS, event_handler);
	}else if(mks_grbl.power_persen == P_0_PERSEN) {
		power_page.pwr_high = lv_imgbtn_creat_n_mks(mks_global.mks_src_2, power_page.pwr_high, &SP_H_UP, &SP_H_UP,PWR_IMGBTN_HIGH_X_POS, PWR_IMGBTN_HIGH_Y_POS, event_handler);
		power_page.pwr_low = lv_imgbtn_creat_n_mks(mks_global.mks_src_2, power_page.pwr_low, &SP_L_UP, &SP_L_UP,PWR_IMGBTN_HIGH_X_POS+PWR_IMGBTN_OFFSET_X, PWR_IMGBTN_HIGH_Y_POS, event_handler);
		power_page.pwr_off = lv_imgbtn_creat_n_mks(mks_global.mks_src_2, power_page.pwr_low, &SP_ON, &SP_ON, PWR_IMGBTN_HIGH_X_POS+PWR_IMGBTN_OFFSET_X*2, PWR_IMGBTN_HIGH_Y_POS, event_handler);
	}
	power_page.label_Back = mks_lvgl_long_sroll_label_with_wight_set_center(mks_global.mks_src_1, power_page.label_Back, 20,60, "Back", 50);
	
#if defined(USE_BL_TOUCH)
	label_cailb = mks_lvgl_long_sroll_label_with_wight_set_center(p_scr1, label_cailb, 350, 60, "Z Home", 60);
#endif

	mks_ui_page.mks_ui_page = MKS_UI_Adjust;
} 


static void event_handler_popup_sure_1(lv_obj_t* obj, lv_event_t event) { 

	if(event == LV_EVENT_RELEASED) {
#if defined(USR_RELASE)
		lv_obj_del(power_page.p_popup);
#else
	common_popup_com_del();
#endif
	}
}


void draw_pwr_popup_1(const char *text) {

#if defined(USR_RELASE)
	// lv_obj_set_click(move_page.Back, false);
	// lv_obj_set_click(move_page.m_unlock, false);
	// lv_obj_set_click(move_page.home, false);
	// lv_obj_set_click(move_page.postivs, false);
	// lv_obj_set_click(move_page.hhome, false);

	power_page.p_popup = lv_obj_create(mks_src, NULL);
	lv_obj_set_size(power_page.p_popup, move_popup_size_x, move_popup_size_y);
    lv_obj_set_pos(power_page.p_popup, move_popup_x, move_popup_y);

	lv_style_copy(&power_page.p_popup_color, &lv_style_scr);
	power_page.p_popup_color.body.main_color = LV_COLOR_MAKE(0xCE, 0xD6, 0xE5); 
    power_page.p_popup_color.body.grad_color = LV_COLOR_MAKE(0xCE, 0xD6, 0xE5); 
    power_page.p_popup_color.text.color = LV_COLOR_WHITE;
    power_page.p_popup_color.body.radius = 17;
	lv_obj_set_style(power_page.p_popup, &power_page.p_popup_color);

	lv_style_copy(&power_page.p_popup_btn_color, &lv_style_scr);
	power_page.p_popup_btn_color.body.main_color = LV_COLOR_MAKE(0x3F, 0x46, 0x66);
    power_page.p_popup_btn_color.body.grad_color = LV_COLOR_MAKE(0x3F, 0x46, 0x66);
    power_page.p_popup_btn_color.body.opa = LV_OPA_COVER;//设置背景色完全不透明
    power_page.p_popup_btn_color.text.color = LV_COLOR_WHITE;
	power_page.p_popup_btn_color.body.radius = 10;

	power_page.btn_sure = mks_lv_btn_set(power_page.p_popup, 
										power_page.btn_sure, 
										move_popup_btn_size_x, 
										move_popup_btn_size_y, 
										move_popup_btn_x,
										move_popup_btn_y, 
										event_handler_popup_sure_1);
	lv_btn_set_style(power_page.btn_sure, LV_BTN_STYLE_REL, &power_page.p_popup_btn_color);
	lv_btn_set_style(power_page.btn_sure,LV_BTN_STYLE_PR, &power_page.p_popup_btn_color);

	power_page.label_sure = mks_lvgl_long_sroll_label_with_wight_set_center(power_page.p_popup, power_page.label_sure, 90, 50, text, 200);
	power_page.label_popup = mks_lvgl_long_sroll_label_with_wight_set_center(power_page.btn_sure, power_page.label_popup, 50, 0, "Yes",50);

#else
	mks_draw_common_popup_info_com("Warning", (char *)text, " ", event_handler_popup_sure_1);
#endif
}










lv_obj_t *bl_src;
lv_obj_t *bl_dis_label;
lv_style_t bl_style;

void mks_draw_bl_touch(void) { 

	bl_src = lv_obj_create(mks_src, NULL);
	lv_obj_set_size(bl_src, 460, 90);
    lv_obj_set_pos(bl_src, 10, 10);

	bl_style.body.main_color = LV_COLOR_MAKE(0xCE, 0xD6, 0xE5); 
    bl_style.body.grad_color = LV_COLOR_MAKE(0xCE, 0xD6, 0xE5); 
	bl_style.text.color = LV_COLOR_BLACK;
	bl_style.body.radius = 17;
	lv_obj_set_style(bl_src, &bl_style);

	bl_dis_label = mks_lv_static_label(bl_src, bl_dis_label, 30, 30, "BLtouch is Working!", 100);
}

void bl_touch_run(void) {
	mks_draw_bl_touch();
	MKS_GRBL_CMD_SEND("G91Z10F100\n");  //Z抬升10
	BLTOUCH_push_down();					//弹出探针
	MKS_GRBL_CMD_SEND("G91Z-30F100\n"); //Z下降30
	mks_grbl.bl_status = BL_ADJ_ING;
}

void bl_touch_check(void) {

	if(mks_grbl.bl_status == BL_ADJ_ING) {

		if(stepper_idle) {   //检测到没有电机动了

			if(mks_grbl.bl_status == BL_ADJ_ING) { //再次确认bltouch

			}
		}
	}
}

void mks_clear_power(void) {
	lv_obj_clean(mks_src);
}