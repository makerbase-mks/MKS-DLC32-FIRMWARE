#include "MKS_draw_move.h"

MKS_MOVE_PAGE move_page;
MKS_MOVE_CTRL_T ui_move_ctrl;
PROBE_RUN_T probe_run;

static void disp_imgbtn(void);

static void disp_imgbtn_1(void);
static void disp_imgbtn_1_del(void);
static void disp_btn(void);
static void disp_label(void);
static void disp_imgbtn_2(void);
static void disp_imgbtn_2_del(void);

enum {
	ID_M_UP,
	ID_M_DOWN,
	ID_M_LEFT,
	ID_M_RIGHT,
	ID_M_Z_UP,
	ID_M_Z_DOWN,
	ID_M_STEP,
	ID_M_SPEED,
	ID_M_UNLOCK,
	ID_M_HOME,
	ID_M_POS,
	ID_M_XY_HOME,
	ID_M_Z_HOME,
	ID_M_XY_POS,
	ID_M_Z_POS,
	ID_M_HHOME,
	ID_M_BACK,
	ID_M_CNC_SET,
	ID_L_NEXT,
	ID_L_UP,
	ID_M_KNIFE,
	ID_M_COOLING,
	ID_M_XYZ_POS,
};

static uint8_t get_event(lv_obj_t* obj) {

    if      (obj == move_page.y_n)  			return ID_M_UP;
    else if (obj == move_page.y_p)     			return ID_M_DOWN;
    else if (obj == move_page.x_n)				return ID_M_LEFT;
    else if (obj == move_page.x_p) 				return ID_M_RIGHT;
	else if (obj == move_page.z_n) 				return ID_M_Z_UP;
	else if (obj == move_page.z_p) 				return ID_M_Z_DOWN;
    else if (obj == move_page.m_unlock)     	return ID_M_UNLOCK;
	else if (obj == move_page.xy_home)     		return ID_M_XY_HOME;
	else if (obj == move_page.z_home)     		return ID_M_Z_HOME;
	else if (obj == move_page.Back)     		return ID_M_BACK;
	else if (obj == move_page.btn_len)     		return ID_M_STEP;
	else if (obj == move_page.btn_speed)		return ID_M_SPEED;
	else if (obj == move_page.next)				return ID_L_NEXT;
	else if (obj == move_page.up)				return ID_L_UP;
	else if (obj == move_page.xy_clear)			return ID_M_XY_POS;
	else if (obj == move_page.z_clear)			return ID_M_Z_POS;				
	else if (obj == move_page.knife)			return ID_M_KNIFE;
	else if (obj == move_page.cooling)			return ID_M_COOLING;
	else if (obj == move_page.btn_spindle)		return ID_M_CNC_SET;
}

static void event_henadle_pupup_com(lv_obj_t* obj, lv_event_t event) { 

	if (event == LV_EVENT_RELEASED) {
		set_click_status(true);
		common_popup_com_del();
	}
}

void set_move(char axis, float step, uint32_t speed) {

	if(step == 0) return;
	char str[30];
	sprintf(str, "$J=G91%c%.2fF%d\n", axis, step, speed);
	MKS_GRBL_CMD_SEND(str);
}

static void set_cooling(lv_obj_t* obj, lv_event_t event) {

	if (event == LV_EVENT_RELEASED) {
		CoolantState state;
		state = coolant_get_state();
		if(state.Flood == 1) {
			MKS_GRBL_CMD_SEND("M9\n");
		}
		else{
			MKS_GRBL_CMD_SEND("M8\n");
		}
	}
}

void move_ctrl(char axis, uint8_t dir) {
	float step;
	uint32_t speed;

	if(dir == 0) {
		if(mks_grbl.move_dis == M_0_1_MM) 			step = -0.1;
		else if(mks_grbl.move_dis == M_1_MM)		step = -1;
		else if(mks_grbl.move_dis == M_10_MM)		step = -10;
	}else {
		if(mks_grbl.move_dis == M_0_1_MM) 			step = 0.1;
		else if(mks_grbl.move_dis == M_1_MM)		step = 1;
		else if(mks_grbl.move_dis == M_10_MM)		step = 10;
	}
	
	if(mks_grbl.move_speed == LOW_SPEED) 		speed = 500;
	else if(mks_grbl.move_speed == MID_SPEED)	speed = 1000;
	else if(mks_grbl.move_speed == HIGHT_SPEED)	speed = 2000;

	set_move(axis, step, speed);
}

void mc_unlock(void) {
	MKS_GRBL_CMD_SEND("$X\n");
	set_click_status(false);
	mks_draw_common_popup_info_com("Info", "Unlock success!", "", event_henadle_pupup_com);
}

void set_move_speed(uint8_t speed_level) {
	if(speed_level == 0)	mks_grbl.move_speed = LOW_SPEED;
	else if(speed_level == 1)	mks_grbl.move_speed = MID_SPEED;
	else if(speed_level == 2)	mks_grbl.move_speed = HIGHT_SPEED;
	else mks_grbl.move_speed = MID_SPEED;
}

void set_move_step(uint8_t step_level) {
	if(step_level == 0)	mks_grbl.move_speed = LOW_SPEED;
	else if(step_level == 1)	mks_grbl.move_speed = MID_SPEED;
	else if(step_level == 2)	mks_grbl.move_speed = HIGHT_SPEED;
	else mks_grbl.move_speed = MID_SPEED;
}

static void event_handler_len(lv_obj_t* obj, lv_event_t event) {

	if (event == LV_EVENT_RELEASED) {
		if (mks_grbl.move_dis == M_0_1_MM) {
			mks_grbl.move_dis = M_1_MM;
			lv_label_set_text(move_page.Label_len, "1mm");
		}else if(mks_grbl.move_dis == M_1_MM) {
			mks_grbl.move_dis = M_10_MM;
			lv_label_set_text(move_page.Label_len, "10mm");
		}else if(mks_grbl.move_dis == M_10_MM) {
			mks_grbl.move_dis = M_0_1_MM;
			lv_label_set_text(move_page.Label_len, "0.1mm");
		}
	}
}

void set_home(void) {

	// 保证主轴不会打开
	MKS_GRBL_CMD_SEND("M5\n");
	mks_grbl.cnc_pwr = GRBL_CNC_OFF;

	if(mks_ui_page.mks_ui_page == MKS_UI_Control) {
		// lv_label_set_text(move_page.label_cnc_pwr, "OFF");
	}

	MKS_GRBL_CMD_SEND("$J=G90X0Y0F2000\n");
	set_click_status(false);
	mks_draw_common_pupup_info("Info", "Homing...", " ");
	ui_move_ctrl.soft_homing_status = HOMING_START;
}


void set_xy_home(void) {

	// 保证主轴不会打开
	MKS_GRBL_CMD_SEND("M5\n");
	mks_grbl.cnc_pwr = GRBL_CNC_OFF;
	
	if(mks_ui_page.mks_ui_page == MKS_UI_Control) {
		// lv_label_set_text(move_page.label_cnc_pwr, "OFF");
	}

	MKS_GRBL_CMD_SEND("$J=G90X0Y0F2000\n");
	set_click_status(false);
	mks_draw_common_pupup_info("Info", "Homing...", " ");
	ui_move_ctrl.soft_homing_status = HOMING_START;
}


void set_z_home(void) {

	// 保证激光不会打开
	MKS_GRBL_CMD_SEND("M5\n");
	mks_grbl.cnc_pwr = GRBL_CNC_OFF;

	if(mks_ui_page.mks_ui_page == MKS_UI_Control) {
		// lv_label_set_text(move_page.label_cnc_pwr, "OFF");
	}

	MKS_GRBL_CMD_SEND("$J=G90Z0F1000\n");
	set_click_status(false);\
	mks_draw_common_pupup_info("Info", "Homing...", " ");
	ui_move_ctrl.soft_homing_status = HOMING_START;
}

static void set_xy_pos(lv_obj_t* obj, lv_event_t event) {

	if(event != LV_EVENT_RELEASED) return;
	set_click_status(false);
	if(sys.state == State::Idle && mks_get_motor_status() ) {
		MKS_GRBL_CMD_SEND("G92X0Y0\n");
		mks_draw_common_popup_info_com("Info", "Positioning success", " ", event_henadle_pupup_com);
	}else {
		mks_draw_common_popup_info_com("Warning", "Please wait machine stop!", " ", event_henadle_pupup_com);
	}
}

static void set_z_pos(lv_obj_t* obj, lv_event_t event) {
	if(event != LV_EVENT_RELEASED) return;
	set_click_status(false);
	if(sys.state == State::Idle && mks_get_motor_status() ) {
		MKS_GRBL_CMD_SEND("G92Z0\n");
		mks_draw_common_popup_info_com("Info", "Positioning success", " ", event_henadle_pupup_com);
	}else {
		mks_draw_common_popup_info_com("Warning", "Please wait machine stop!", " ", event_henadle_pupup_com);
	}
}

static void set_xyz_pos(lv_obj_t* obj, lv_event_t event) {

	if(event == LV_EVENT_RELEASED ) {
		
		set_click_status(false);

		if(sys.state == State::Idle && mks_get_motor_status() ) {
			MKS_GRBL_CMD_SEND("G92X0Y0Z0\n");
			mks_draw_common_popup_info_com("Info", "Positioning success", " ", event_henadle_pupup_com);
		}else {
			mks_draw_common_popup_info_com("Warning", "Please wait machine stop!", " ", event_henadle_pupup_com);
		}
	}
}

static void set_knife() {

	if(probe_invert->get()) {
		MKS_GRBL_CMD_SEND("G21 G91 G38.2 Z-50 F80\n");
		mks_draw_common_pupup_info("Info", "Setting probe...", " ");
		probe_run.status = PROBE_STAR;

	}else {
		set_click_status(true);
		mks_draw_common_popup_info_com("Info", "Setting error!", "Please set $6=1!", event_henadle_pupup_com);
	}
}


static void set_hhome(void) {

	MKS_GRBL_CMD_SEND("M5\n");
	mks_grbl.power_persen = P_0_PERSEN;

	if(mks_ui_page.mks_ui_page == MKS_UI_Control) {
		
	}

	set_click_status(false);

	if(hard_limits->get() && homing_enable->get()) {
		MKS_GRBL_CMD_SEND("$H\n");
		ui_move_ctrl.hard_homing_status = HOMING_START;
		mks_draw_common_pupup_info("Info", "Homing...", " ");
	}
	else {
		mks_draw_common_popup_info_com("Warning", "No Enable Hard Homing...", " ", event_henadle_pupup_com);
	}
}

static void set_cnc_power(bool status, uint8_t persen) {

	char buf[96];

	if(status) {
		sprintf(buf, "M3 S%d\n", persen*100);
		MKS_GRBL_CMD_SEND(buf);
	}
	else {
		sprintf(buf, "M5\n");
		MKS_GRBL_CMD_SEND(buf);
	}
}

static void set_cnc_ctrl(void) {

	if(sys_rt_s_override == 0) {
		mks_grbl.cnc_pwr = GRBL_CNC_OFF;
	}

	if(mks_grbl.cnc_pwr == GRBL_CNC_OFF) {
		set_cnc_power(true, 50);
		mks_grbl.cnc_pwr = GRBL_CNC_50;
		lv_btn_set_style(move_page.btn_spindle, LV_BTN_STYLE_REL, &move_page.btn_color_press);
		lv_btn_set_style(move_page.btn_spindle, LV_BTN_STYLE_PR,&move_page.btn_color_press);
	}
	else {
		set_cnc_power(false, 0);
		mks_grbl.cnc_pwr = GRBL_CNC_OFF;
		lv_btn_set_style(move_page.btn_spindle, LV_BTN_STYLE_REL, &move_page.btn_color);
		lv_btn_set_style(move_page.btn_spindle, LV_BTN_STYLE_PR,&move_page.btn_color);
	}
}

void set_step_len(void) {
	if(mks_grbl.move_dis == M_0_1_MM) {
		mks_grbl.move_dis = M_1_MM;
		mks_lv_label_updata(move_page.label_len, "1mm");
	}else if(mks_grbl.move_dis == M_1_MM) {
		mks_grbl.move_dis = M_10_MM;
		mks_lv_label_updata(move_page.label_len, "10mm");
	}else if(mks_grbl.move_dis == M_10_MM) {
		mks_grbl.move_dis = M_0_1_MM;
		mks_lv_label_updata(move_page.label_len, "0.1mm");
	}
}

void set_speed(void) {
	if(mks_grbl.move_speed == LOW_SPEED) {
		mks_grbl.move_speed = MID_SPEED;
		mks_lv_label_updata(move_page.label_speed, "Mid Speed");
	}else if(mks_grbl.move_speed == MID_SPEED) {
		mks_grbl.move_speed = HIGHT_SPEED;
		mks_lv_label_updata(move_page.label_speed, "High Speed");
	}else if(mks_grbl.move_speed == HIGHT_SPEED) {
		mks_grbl.move_speed = LOW_SPEED;
		mks_lv_label_updata(move_page.label_speed, "Low Speed");
	}
}

void set_move_back(void) {
        mks_lv_clean_ui();
		mks_ui_page.mks_ui_page = MKS_UI_PAGE_LOADING;
        mks_ui_page.wait_count = DEFAULT_UI_COUNT;
        mks_draw_ready();
}

static void event_handler(lv_obj_t* obj, lv_event_t event) {

	if(event != LV_EVENT_RELEASED) return;

	uint8_t id = get_event(obj);

	switch(id) {
		case ID_M_UP	:	move_ctrl('Y', 1); 	break;
		case ID_M_DOWN	:	move_ctrl('Y', 0); 	break;
		case ID_M_LEFT	:	move_ctrl('X', 0); 	break;
		case ID_M_RIGHT	:	move_ctrl('X', 1); 	break;
		case ID_M_Z_UP	:	move_ctrl('Z', 1);	break;
		case ID_M_Z_DOWN:	move_ctrl('Z', 0);	break;
		case ID_M_STEP	:	set_step_len();		break;
		case ID_M_SPEED	:	set_speed();		break;
		case ID_M_UNLOCK:	mc_unlock();		break;
		case ID_M_HOME	:	set_home();			break;
		case ID_M_HHOME	:	set_hhome();		break;
		case ID_M_BACK	: 	set_move_back(); 	break;
		case ID_M_XY_HOME:	set_xy_home(); 		break;
		case ID_M_Z_HOME:	set_z_home();		break;
		// case ID_M_XY_POS:	set_xy_pos();		break;
		// case ID_M_Z_POS :	set_z_pos(); 		break;	
		case ID_M_CNC_SET:	set_cnc_ctrl();		break;
		// case ID_L_NEXT	:	disp_imgbtn_1_del(); disp_imgbtn_2(); break;
		// case ID_L_UP	: 	disp_imgbtn_2_del();  disp_imgbtn_1(); break;
		case ID_M_KNIFE: set_knife(); break;
		// case ID_M_COOLING: set_cooling();  break;
	}
}

void mks_draw_move(void) {
 
	mks_global.mks_src_1 = lv_obj_create(mks_global.mks_src, NULL);
	lv_obj_set_size(mks_global.mks_src_1, 460, 90);
    lv_obj_set_pos(mks_global.mks_src_1, 10, 10);

	mks_global.mks_src_2 = lv_obj_create(mks_global.mks_src, NULL);
	lv_obj_set_size(mks_global.mks_src_2, 320, 200);
    lv_obj_set_pos(mks_global.mks_src_2, 10, 110);

	mks_global.mks_src_3 = lv_obj_create(mks_global.mks_src, NULL);
	lv_obj_set_size(mks_global.mks_src_3, 130, 200);
    lv_obj_set_pos(mks_global.mks_src_3, 340, 110);

	/* 背景层样式 */
	lv_obj_set_style(mks_global.mks_src_1, &mks_global.mks_src_1_style);
	lv_obj_set_style(mks_global.mks_src_2, &mks_global.mks_src_2_style);
	lv_obj_set_style(mks_global.mks_src_3, &mks_global.mks_src_3_style);

	disp_imgbtn();
	disp_btn();
	disp_label();

	mks_ui_page.mks_ui_page = MKS_UI_Control;
    mks_ui_page.wait_count = DEFAULT_UI_COUNT;
}

static void disp_imgbtn(void) {

	move_page.Back = lv_imgbtn_creat_mks(mks_global.mks_src_1, move_page.Back, &png_back_pre, &back, LV_ALIGN_IN_TOP_LEFT, 10, 5 , event_handler);

	disp_imgbtn_1();

    move_page.y_n = lv_imgbtn_creat_mks(mks_global.mks_src_2, move_page.y_n, &png_up_pre, &png_up, LV_ALIGN_IN_TOP_LEFT, 88, 10, event_handler);
    move_page.y_p = lv_imgbtn_creat_mks(mks_global.mks_src_2, move_page.y_p, &png_down_pre, &png_down, LV_ALIGN_IN_TOP_LEFT, 88, 138, event_handler);
    move_page.x_n = lv_imgbtn_creat_mks(mks_global.mks_src_2, move_page.x_n, &png_left_pre, &png_left, LV_ALIGN_IN_TOP_LEFT, 10, 74, event_handler);
    move_page.x_p = lv_imgbtn_creat_mks(mks_global.mks_src_2, move_page.x_p, &png_right_pre, &png_right, LV_ALIGN_IN_TOP_LEFT, 166, 74, event_handler);
	move_page.z_n = lv_imgbtn_creat_mks(mks_global.mks_src_2, move_page.z_n, &png_z_up_pre, &png_z_up, LV_ALIGN_IN_TOP_LEFT, 244, 10, event_handler);
	move_page.z_p = lv_imgbtn_creat_mks(mks_global.mks_src_2, move_page.z_p, &png_z_down_pre, &png_z_down, LV_ALIGN_IN_TOP_LEFT, 244, 138, event_handler);

	move_page.xy_home = lv_imgbtn_creat_mks(mks_global.mks_src_2, move_page.xy_home, &png_xyhome_pre, &png_xyhome, LV_ALIGN_IN_TOP_LEFT, 88, 74, event_handler);
	move_page.z_home = lv_imgbtn_creat_mks(mks_global.mks_src_2, move_page.z_home, &png_z_home_pre, &png_z_home, LV_ALIGN_IN_TOP_LEFT, 244, 74, event_handler);
}

static void disp_imgbtn_1_del(void) {
	lv_obj_del(move_page.xy_clear);
	lv_obj_del(move_page.z_clear);
	lv_obj_del(move_page.knife);
	lv_obj_del(move_page.next);

	lv_obj_del(move_page.label_xy_clear);
	lv_obj_del(move_page.label_z_clear);
	lv_obj_del(move_page.label_knife);
	lv_obj_del(move_page.label_next);

	// lv_obj_del(move_page.label_xpos);
	// lv_obj_del(move_page.label_ypos);
	// lv_obj_del(move_page.label_zpos);
}

static void disp_up_set(lv_obj_t* obj, lv_event_t event) {

	if(event != LV_EVENT_RELEASED) return;
	disp_imgbtn_2_del();  
	disp_imgbtn_1();
}

static void disp_down_set(lv_obj_t* obj, lv_event_t event) {

	if(event != LV_EVENT_RELEASED) return;
	disp_imgbtn_1_del(); 
	disp_imgbtn_2();
}

static void disp_imgbtn_1(void) {

	move_page.xy_clear = lv_imgbtn_creat_mks(mks_global.mks_src_1, move_page.xy_clear, &png_xyclear_pre, &png_xyclear, LV_ALIGN_IN_TOP_LEFT, 170, 5, set_xy_pos);
	move_page.z_clear = lv_imgbtn_creat_mks(mks_global.mks_src_1, move_page.z_clear, &png_zclear_pre, &png_zclear, LV_ALIGN_IN_TOP_LEFT, 240, 5, set_z_pos);
	move_page.knife = lv_imgbtn_creat_mks(mks_global.mks_src_1, move_page.knife, &png_knife_pre, &png_knife, LV_ALIGN_IN_TOP_LEFT, 310, 5, event_handler);
	move_page.next = lv_imgbtn_creat_mks(mks_global.mks_src_1, move_page.next, &png_l_next_pre, &png_l_next, LV_ALIGN_IN_TOP_LEFT, 380, 5, disp_down_set);

	move_page.label_xy_clear = label_for_imgbtn_name(mks_global.mks_src_1, move_page.label_xy_clear, move_page.xy_clear, 0, 0, "XY Clear");
	move_page.label_z_clear = label_for_imgbtn_name(mks_global.mks_src_1, move_page.label_z_clear, move_page.z_clear, 0, 0, "Z Clear");
	move_page.label_knife = label_for_imgbtn_name(mks_global.mks_src_1, move_page.label_knife, move_page.knife, 0, 0, "Knife");
	move_page.label_next = label_for_imgbtn_name(mks_global.mks_src_1, move_page.label_next, move_page.next, 0, 0, "Next");
}

static void disp_imgbtn_2(void) {
	move_page.up = lv_imgbtn_creat_mks(mks_global.mks_src_1, move_page.up, &png_l_up_pre, &png_l_up, LV_ALIGN_IN_TOP_LEFT, 170, 5, disp_up_set);
	move_page.cooling = lv_imgbtn_creat_mks(mks_global.mks_src_1, move_page.cooling, &png_cooling_pre, &png_cooling, LV_ALIGN_IN_TOP_LEFT, 240, 5, set_cooling);
	move_page.position = lv_imgbtn_creat_mks(mks_global.mks_src_1, move_page.position, &png_position_pre, &png_position, LV_ALIGN_IN_TOP_LEFT, 310, 5, set_xyz_pos);

	move_page.label_cooling = label_for_imgbtn_name(mks_global.mks_src_1, move_page.label_cooling, move_page.cooling, 0, 0, "Cooling");
	move_page.label_position = label_for_imgbtn_name(mks_global.mks_src_1, move_page.label_position, move_page.position, 0, 0, "Position");
	move_page.label_up = label_for_imgbtn_name(mks_global.mks_src_1, move_page.label_up, move_page.up, 0, 0, "Up");
}

static void disp_imgbtn_2_del(void) {
	lv_obj_del(move_page.cooling);
	lv_obj_del(move_page.position);
	lv_obj_del(move_page.up);

	lv_obj_del(move_page.label_cooling);
	lv_obj_del(move_page.label_position);
	lv_obj_del(move_page.label_up);
}

void move_pos_update(void) {

	static float mks_print_position[MAX_N_AXIS];
    float* print_position = system_get_mpos();

	mpos_to_wpos(print_position);
	
	sprintf(xpos_str, "X:%.1f", print_position[0]);
    sprintf(ypos_str, "Y:%.1f", print_position[1]);
    sprintf(zpos_str, "Z:%.1f", print_position[2]);

	if( (move_page.label_xpos != NULL) && 
		(move_page.label_ypos != NULL) &&
		(move_page.label_zpos != NULL)
	) {

		lv_label_set_static_text(move_page.label_xpos, xpos_str);
		lv_label_set_static_text(move_page.label_ypos, ypos_str);
		lv_label_set_static_text(move_page.label_zpos, zpos_str);
	}
}


static void disp_btn(void) {
	/* 按键样式 */
	lv_style_copy(&move_page.btn_color, &lv_style_scr);
    move_page.btn_color.body.main_color = LV_COLOR_MAKE(0x17, 0x1A, 0x26);
    move_page.btn_color.body.grad_color = LV_COLOR_MAKE(0x17, 0x1A, 0x26);
    move_page.btn_color.body.opa = LV_OPA_COVER;//设置背景色完全不透明
    move_page.btn_color.text.color = LV_COLOR_WHITE;
	move_page.btn_color.body.radius = 10;

	lv_style_copy(&move_page.btn_color_press, &lv_style_scr);
	move_page.btn_color_press.body.main_color = LV_COLOR_MAKE(0x3F, 0x47, 0x66);
    move_page.btn_color_press.body.grad_color = LV_COLOR_MAKE(0x3F, 0x47, 0x66);
    move_page.btn_color_press.body.opa = LV_OPA_COVER;//设置背景色完全不透明
    move_page.btn_color_press.text.color = LV_COLOR_WHITE;
	move_page.btn_color_press.body.radius = 10;

	move_page.btn_len = mks_lv_btn_set_for_aglin_screen(mks_global.mks_src_3, move_page.btn_len, 110, 52, LV_ALIGN_IN_TOP_LEFT, 10, 10, event_handler);
	move_page.btn_speed = mks_lv_btn_set_for_aglin_screen(mks_global.mks_src_3, move_page.btn_speed, 110, 52, LV_ALIGN_IN_TOP_LEFT, 10, 74, event_handler);
	
	move_page.btn_spindle = mks_lv_btn_set_for_aglin_screen(mks_global.mks_src_3, move_page.btn_speed, 110, 52, LV_ALIGN_IN_TOP_LEFT, 10, 138, event_handler);

	lv_btn_set_style(move_page.btn_len, LV_BTN_STYLE_REL, &move_page.btn_color);
	lv_btn_set_style(move_page.btn_len, LV_BTN_STYLE_PR, &move_page.btn_color_press);

	lv_btn_set_style(move_page.btn_speed, LV_BTN_STYLE_REL, &move_page.btn_color);
	lv_btn_set_style(move_page.btn_speed,LV_BTN_STYLE_PR,&move_page.btn_color_press);

	if(mks_grbl.cnc_pwr == GRBL_CNC_OFF) {
		lv_btn_set_style(move_page.btn_spindle, LV_BTN_STYLE_REL, &move_page.btn_color);
		lv_btn_set_style(move_page.btn_spindle, LV_BTN_STYLE_PR,&move_page.btn_color);
	}else{
		lv_btn_set_style(move_page.btn_spindle, LV_BTN_STYLE_REL, &move_page.btn_color_press);
		lv_btn_set_style(move_page.btn_spindle, LV_BTN_STYLE_PR,&move_page.btn_color_press);
	}
}

static void disp_label(void) {

	label_for_imgbtn_name(mks_global.mks_src_1, move_page.Label_back, move_page.Back, 0, 0, "Back");

	move_page.label_xpos = label_for_text(mks_global.mks_src_1, move_page.label_xpos, NULL, 93, 5, LV_ALIGN_IN_TOP_LEFT,  	"X:0");
	move_page.label_ypos = label_for_text(mks_global.mks_src_1, move_page.label_ypos, NULL, 93, 36, LV_ALIGN_IN_TOP_LEFT,	"Y:0");
	move_page.label_zpos = label_for_text(mks_global.mks_src_1, move_page.label_zpos, NULL, 93, 66, LV_ALIGN_IN_TOP_LEFT,  	"Z:0");

	if(mks_grbl.move_dis == M_0_1_MM) {
		move_page.label_len = mks_lvgl_long_sroll_label_with_wight_set_center(move_page.btn_len, move_page.label_len, 0, 0, "0.1mm", 50);
	}else if(mks_grbl.move_dis == M_1_MM) {
		move_page.label_len = mks_lvgl_long_sroll_label_with_wight_set_center(move_page.btn_len, move_page.label_len, 0, 0, "1mm", 50);
	}else if(mks_grbl.move_dis == M_10_MM) {
		move_page.label_len = mks_lvgl_long_sroll_label_with_wight_set_center(move_page.btn_len, move_page.label_len, 0, 0, "10mm", 50);
	}
	
	if(mks_grbl.move_speed == LOW_SPEED) {
		move_page.label_speed = mks_lvgl_long_sroll_label_with_wight_set_center(move_page.btn_speed, move_page.label_speed, 0, 0, "Low Speed", 100); //l:500, m:1000, h:2000
	}else if(mks_grbl.move_speed == MID_SPEED) {
		move_page.label_speed = mks_lvgl_long_sroll_label_with_wight_set_center(move_page.btn_speed, move_page.label_speed, 0, 0, "Mid Speed", 100);
	}else if(mks_grbl.move_speed == HIGHT_SPEED) {
		move_page.label_speed = mks_lvgl_long_sroll_label_with_wight_set_center(move_page.btn_speed, move_page.label_speed, 0, 0, "High Speed", 100);
	}	
	
	move_page.label_spindle = mks_lvgl_long_sroll_label_with_wight_set_center(move_page.btn_spindle, move_page.label_spindle, 0, 0, "Spindle",100);
}

void set_click_status(bool status) {
	lv_obj_set_click(move_page.Back, status);
}

lv_obj_t* move_popup_scr;
lv_style_t move_popup_color;
lv_style_t move_popup_btn_style;
lv_obj_t* move_popup_btn_sure;
lv_obj_t* move_popup_label_sure;
lv_obj_t* move_popup_label_dis;

static void event_handler_popup_sure(lv_obj_t* obj, lv_event_t event) {

	if (event == LV_EVENT_RELEASED) {
		lv_obj_set_click(move_page.Back, true);
		// lv_obj_set_click(move_page.m_unlock, true);
		lv_obj_del(move_popup_scr);
	}
}

static void event_handler_popup_sure_1(lv_obj_t* obj, lv_event_t event) {

	if (event == LV_EVENT_RELEASED) {
		lv_obj_set_click(move_page.Back, true);
		// lv_obj_set_click(move_page.m_unlock, true);
		lv_obj_del(move_popup_scr);
	}
}

void hard_home_check(void) {
	switch(ui_move_ctrl.hard_homing_status) {

		case HOMING_NONE:
			
		break;

		case HOMING_START:
			if(sys.state == State::Homing) 		ui_move_ctrl.hard_homing_status = HOMING_RUNNING;
			else if(sys.state == State::Idle)	ui_move_ctrl.hard_homing_status = HOMING_SUCCEED;
			else if(sys.state == State::Alarm)	ui_move_ctrl.hard_homing_status = HOMING_FAIL;
			else								ui_move_ctrl.hard_homing_status = HOMING_FAIL;
		break;

		case HOMING_RUNNING:
			if(sys.state == State::Idle) 		ui_move_ctrl.hard_homing_status = HOMING_SUCCEED;
			else if(sys.state == State::Alarm)	ui_move_ctrl.hard_homing_status = HOMING_FAIL;
			else if(sys.state == State::Homing)	ui_move_ctrl.hard_homing_status = HOMING_RUNNING;
			else 								ui_move_ctrl.hard_homing_status = HOMING_FAIL;
		break;

		case HOMING_SUCCEED:
			ui_move_ctrl.hard_homing_status = HOMING_NONE;
			common_pupup_info_del();
			mks_draw_common_popup_info_com("Info", "Homing succeed!", " ", event_henadle_pupup_com);
		break;

		case HOMING_FAIL:
			ui_move_ctrl.hard_homing_status = HOMING_NONE;
			common_pupup_info_del();
			mks_draw_common_popup_info_com("Info", "Homing fail", "please unlock!", event_henadle_pupup_com);
		break;
	}
}

void soft_home_check(void) { 

	switch(ui_move_ctrl.soft_homing_status) {

		case HOMING_NONE:

		break;

		case HOMING_START:
			if(sys.state == State::Jog) 		ui_move_ctrl.soft_homing_status = HOMING_RUNNING;
			else if(sys.state == State::Idle)	ui_move_ctrl.soft_homing_status = HOMING_SUCCEED;
			else if(sys.state == State::Alarm)	ui_move_ctrl.soft_homing_status = HOMING_FAIL;
			else 								ui_move_ctrl.soft_homing_status = HOMING_FAIL;
		break;

		case HOMING_RUNNING:
			if(sys.state == State::Idle)		ui_move_ctrl.soft_homing_status = HOMING_SUCCEED;
			else if(sys.state == State::Alarm)	ui_move_ctrl.soft_homing_status = HOMING_FAIL;
			else if(sys.state == State::Jog)	ui_move_ctrl.soft_homing_status = HOMING_RUNNING;
			else 								ui_move_ctrl.soft_homing_status = HOMING_FAIL;
		break;

		case HOMING_SUCCEED:
			ui_move_ctrl.soft_homing_status = HOMING_NONE;
			common_pupup_info_del();
			mks_draw_common_popup_info_com("Info", "Homing succeed!", " ", event_henadle_pupup_com);
		break;

		case HOMING_FAIL:
			// lv_obj_del(move_popup_scr);
			ui_move_ctrl.soft_homing_status = HOMING_NONE;
			common_pupup_info_del();
			mks_draw_common_popup_info_com("Info", "Homing fail", " ", event_henadle_pupup_com);
		break;
	}
}

void probe_check() {

	switch(probe_run.status) 
	{
		case PROBE_NO: 
		break;

		case PROBE_STAR: 
			if(probe_run.flag == 1) {
				MKS_GRBL_CMD_SEND("G0Z0.5\n");
				probe_run.status = PROBE_FITST_SUCCEED;
			} 
			else if(probe_run.flag == 2) probe_run.status = PROBE_FITST_FAIL;
			else if(probe_run.flag == 0) { }// nothings to do..
			else probe_run.status = PROBE_FITST_FAIL;
			probe_run.flag = 0;
		break;

		case PROBE_FITST_SUCCEED: 
			// grbl_send(CLIENT_SERIAL, "enter first secceed\n");
			MKS_GRBL_CMD_SEND("G38.2Z-2F30\n");
			probe_run.status = PROBE_SECOND_STAR;
		break;

		case PROBE_FITST_FAIL: 
			// grbl_send(CLIENT_SERIAL, "enter first fail\n");
			probe_run.status = PROBE_NO;
		break;

		case PROBE_SECOND_STAR:
			if(probe_run.flag == 1) probe_run.status = PROBE_SECODN_SUCCEED;
			else if(probe_run.flag == 2) probe_run.status = PROBE_SECODN_FAIL;
		break;

		case PROBE_SECODN_SUCCEED: 
			MKS_GRBL_CMD_SEND("G92Z10\n");
			MKS_GRBL_CMD_SEND("G0Z10F10\n");
			common_pupup_info_del();
			mks_draw_common_popup_info_com("Info", "Probe succeed!", " ", event_henadle_pupup_com);
			probe_run.status = PROBE_NO;
			probe_run.flag = 0;
		break;
		
    	case PROBE_SECODN_FAIL:
			common_pupup_info_del();
			mks_draw_common_popup_info_com("Info", "Probe fail!", " ", event_henadle_pupup_com);
			probe_run.status = PROBE_NO;
			probe_run.flag = 0;
		break;
	}
}

bool mks_get_motor_status(void) { 
	return stepper_idle;
}

void mks_clear_move(void) {
	lv_obj_clean(mks_src);
}




