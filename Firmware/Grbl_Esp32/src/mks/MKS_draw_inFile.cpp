#include "MKS_draw_inFile.h"

inFILE_PAGE_T infile_page;

LV_IMG_DECLARE(X_N);			
LV_IMG_DECLARE(X_P);			
LV_IMG_DECLARE(Y_N);			
LV_IMG_DECLARE(Y_P);
LV_IMG_DECLARE(back);

LV_IMG_DECLARE(png_infile_cave);
LV_IMG_DECLARE(png_infile_pos);
LV_IMG_DECLARE(png_infile_frame);

LV_IMG_DECLARE(Positionting);
LV_IMG_DECLARE(png_pos_pre);
	
LV_IMG_DECLARE(png_m_up);
LV_IMG_DECLARE(png_m_right);		
LV_IMG_DECLARE(png_m_left);		
LV_IMG_DECLARE(png_m_down);

LV_IMG_DECLARE(png_infile_pos_pre);
LV_IMG_DECLARE(png_infile_frame_pre);
LV_IMG_DECLARE(png_infile_cave_pre);

LV_IMG_DECLARE(png_m_z_n);			
LV_IMG_DECLARE(png_m_z_n_pre);		
LV_IMG_DECLARE(png_m_z_p);			
LV_IMG_DECLARE(png_m_z_p_pre);
LV_IMG_DECLARE(png_home_pre);
LV_IMG_DECLARE(png_hhome_pre);		
LV_IMG_DECLARE(png_unlock_pre);		
LV_IMG_DECLARE(png_pos_pre);	
LV_IMG_DECLARE(png_m_up);
LV_IMG_DECLARE(png_m_right);		
LV_IMG_DECLARE(png_m_left);		
LV_IMG_DECLARE(png_m_down);
LV_IMG_DECLARE(png_back_pre);	




enum {

	ID_INF_UP,
	ID_INF_DOWN,
	ID_INF_LEFT,
	ID_INF_RIGHT,
	ID_INF_Z_UP,
	ID_INF_Z_DOWN,
	ID_INF_STEP,
	ID_INF_SPEED,
	ID_INF_XY_POS,
	ID_INF_Z_POS,
	ID_INF_CARVE,
	ID_INF_BACK,
	ID_INF_XY_HOME,
	ID_INF_Z_HOME,
	ID_INF_L_NEXT,
	ID_INF_L_UP,
	ID_INF_KNIFE,
};

static void disp_imgbtn(void);
static void disp_imgbtn_1(void);
static void disp_imgbtn_1_del(void);
static void disp_imgbtn_2(void);
static void disp_imgbtn_2_del(void);
static void disp_label(void);
static void disp_btn(void);
static uint8_t get_id(lv_obj_t* obj) {

    if      (obj == move_page.y_n)  			return ID_INF_UP;
    else if (obj == move_page.y_p)     			return ID_INF_DOWN;
    else if (obj == move_page.x_n)				return ID_INF_LEFT;
    else if (obj == move_page.x_p) 				return ID_INF_RIGHT;
	else if (obj == move_page.z_n) 				return ID_INF_Z_UP;
	else if (obj == move_page.z_p) 				return ID_INF_Z_DOWN;
	else if (obj == move_page.xy_home)     		return ID_INF_XY_HOME;
	else if (obj == move_page.z_home)     		return ID_INF_Z_HOME;
	else if (obj == infile_page.btn_cancle)   	return ID_INF_BACK;
	else if (obj == move_page.btn_len)     		return ID_INF_STEP;
	else if (obj == move_page.btn_speed)		return ID_INF_SPEED;
	else if (obj == infile_page.btn_sculpture)	return ID_INF_CARVE; 
	else if (obj == move_page.next)				return ID_INF_L_NEXT;
	else if (obj == move_page.up)				return ID_INF_L_UP;
	else if (obj == move_page.xy_clear)			return ID_INF_XY_POS;
	else if (obj == move_page.z_clear)			return ID_INF_Z_POS;
	else if (obj == move_page.knife)			return ID_INF_KNIFE;
}

static void event_handler_cave_yes(lv_obj_t* obj, lv_event_t event) {

	if (event == LV_EVENT_RELEASED) {
		mks_ui_page.mks_ui_page = MKS_UI_PAGE_LOADING;
		cavre_popup_del();
		lv_obj_clean(mks_global.mks_src);
		start_print();
	}
}

static void event_handler_cave_no(lv_obj_t* obj, lv_event_t event) {

	if (event == LV_EVENT_RELEASED) {
		cavre_popup_del();
	}
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

static void event_handler_back(void) {

	mks_ui_page.mks_ui_page = MKS_UI_PAGE_LOADING;
	lv_obj_clean(mks_global.mks_src);
	mks_draw_ready();
}

static void event_handler_sure(void) {
	char temp[128];
	memset(temp, 0, sizeof(temp));
	memcpy(temp, mks_file_list.filename_str[mks_file_list.file_choose], sizeof(temp));
	if(temp[0]=='/') temp[0] = ' ';
	mks_draw_cavre_popup(temp, event_handler_cave_yes, event_handler_cave_no);
}


static void event_handler_com_info(lv_obj_t* obj, lv_event_t event){

	if (event == LV_EVENT_RELEASED) {
		common_popup_com_del();
	}
}


static void event_handler_carve_set(lv_obj_t* obj, lv_event_t event){

	if (event == LV_EVENT_RELEASED) {
		infile_clean_obj(mks_global.mks_src_3);
		mks_draw_freaure();
	}
}

static void event_henadle_pupup_com(lv_obj_t* obj, lv_event_t event) { 

	if (event == LV_EVENT_RELEASED) {
		common_popup_com_del();
	}
}

static void set_xy_pos(lv_obj_t* obj, lv_event_t event) {
	if(event != LV_EVENT_RELEASED) return;
	if(sys.state == State::Idle && mks_get_motor_status() ) {
		MKS_GRBL_CMD_SEND("G92X0Y0\n");
		mks_draw_common_popup_info_com("Info", "Positioning success", " ", event_henadle_pupup_com);
	}else {
		mks_draw_common_popup_info_com("Warning", "Please wait machine stop!", " ", event_henadle_pupup_com);
	}
}

static void set_z_pos(lv_obj_t* obj, lv_event_t event) {
	if(event != LV_EVENT_RELEASED) return;
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

static void event_handler_len_set(void){

	if (mks_grbl.move_dis == M_0_1_MM) {
		mks_grbl.move_dis = M_1_MM;
		lv_label_set_text(move_page.label_len, "1mm");
	}else if(mks_grbl.move_dis == M_1_MM) {
		mks_grbl.move_dis = M_10_MM;
		lv_label_set_text(move_page.label_len, "10mm");
	}else if(mks_grbl.move_dis == M_10_MM) {
		mks_grbl.move_dis = M_0_1_MM;
		lv_label_set_text(move_page.label_len, "0.1mm");
	}
}

static void event_handler_speed(void){

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


static void event_handler(lv_obj_t* obj, lv_event_t event) {

	uint8_t id = get_id(obj);

	if(event == LV_EVENT_PRESSED) { 

    }
	
	if((event == LV_EVENT_RELEASED) || (event == LV_EVENT_PRESS_LOST)) {

		switch(id) {
			case ID_INF_UP:	move_ctrl('Y', 1); break;
			case ID_INF_DOWN: move_ctrl('Y', 0); break;
			case ID_INF_LEFT: move_ctrl('X', 0); break;
			case ID_INF_RIGHT: move_ctrl('X', 1); break;
			case ID_INF_Z_UP: move_ctrl('Z', 1); break;
			case ID_INF_Z_DOWN: move_ctrl('Z', 0); break;
			// case ID_INF_XY_POS: set_xy_pos(); break;
			// case ID_INF_Z_POS: set_z_pos(); break;
			case ID_INF_BACK: grbl_send(CLIENT_SERIAL, "into back\n"); event_handler_back(); break;
			case ID_INF_STEP: event_handler_len_set(); break;
			case ID_INF_SPEED: event_handler_speed(); break;
			case ID_INF_CARVE: event_handler_sure(); break; 
			case ID_INF_L_NEXT	:	disp_imgbtn_1_del(); disp_imgbtn_2(); break;
			case ID_INF_L_UP	: 	disp_imgbtn_2_del();  disp_imgbtn_1(); break;
			// case ID_INF_XY_POS:	set_xy_pos();		break;
			// case ID_INF_Z_POS :	set_z_pos(); 		break;
			case ID_INF_KNIFE: set_knife(); break;
		}
	}
}

void mks_draw_inFile(char *fn) {

	/* 背景层 */
	mks_global.mks_src_1 = lv_obj_create(mks_global.mks_src, NULL);
	lv_obj_set_size(mks_global.mks_src_1, 460, 90);
    lv_obj_set_pos(mks_global.mks_src_1, 10, 10);

	mks_global.mks_src_2 = lv_obj_create(mks_global.mks_src, NULL);
	lv_obj_set_size(mks_global.mks_src_2, 320, 200);
    lv_obj_set_pos(mks_global.mks_src_2, 10, 110);

	mks_global.mks_src_3 = lv_obj_create(mks_global.mks_src, NULL);
	lv_obj_set_size(mks_global.mks_src_3, 130, 200);
    lv_obj_set_pos(mks_global.mks_src_3, 340, 110);

	lv_obj_set_style(mks_global.mks_src_1, &mks_global.mks_src_1_style);
	lv_obj_set_style(mks_global.mks_src_2, &mks_global.mks_src_2_style);
	lv_obj_set_style(mks_global.mks_src_3, &mks_global.mks_src_3_style);

	disp_imgbtn();
	disp_btn();

	//	记录文件名
	memset(frame_ctrl.file_name, 0, sizeof(frame_ctrl.file_name));
	memcpy(frame_ctrl.file_name, fn, 128);

	//	记录文件大小
	frame_ctrl.file_size = mks_file_list.file_size[mks_file_list.file_choose]; 

	// 显示label
	// if(fn[0] == '/') fn[0] = ' ';
	// label_for_infile_name(mks_global.mks_src_1, infile_page.label_file_name, -120, 0, fn);
	
	disp_label();
	
	mks_ui_page.mks_ui_page = MKS_UI_inFile;
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

	// infile_page.btn_sure_print = lv_imgbtn_creat_mks(mks_global.mks_src_1, infile_page.btn_sure_print, &png_infile_cave_pre, &png_infile_cave, LV_ALIGN_IN_LEFT_MID, 370,-15, event_handler);
	infile_page.btn_cancle = lv_imgbtn_creat_mks(mks_global.mks_src_1, infile_page.btn_cancle, &back, &back, LV_ALIGN_IN_LEFT_MID,10, -15 , event_handler);
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

static void disp_imgbtn_1_del(void) {
	lv_obj_del(move_page.xy_clear);
	lv_obj_del(move_page.z_clear);
	lv_obj_del(move_page.knife);
	lv_obj_del(move_page.next);

	lv_obj_del(move_page.label_xy_clear);
	lv_obj_del(move_page.label_z_clear);
	lv_obj_del(move_page.label_knife);
	lv_obj_del(move_page.label_next);
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

static void disp_btn(void) {

	/* 按键样式 */
	lv_style_copy(&infile_page.btn_color, &lv_style_scr);
    infile_page.btn_color.body.main_color = LV_COLOR_MAKE(0x17, 0x1A, 0x26);
    infile_page.btn_color.body.grad_color = LV_COLOR_MAKE(0x17, 0x1A, 0x26);
    infile_page.btn_color.body.opa = LV_OPA_COVER;//设置背景色完全不透明
    infile_page.btn_color.text.color = LV_COLOR_WHITE;
	infile_page.btn_color.body.radius = 10;

	lv_style_copy(&infile_page.btn_press_color, &lv_style_scr);
    infile_page.btn_press_color.body.main_color = LV_COLOR_MAKE(0x3F, 0x47, 0x66);
    infile_page.btn_press_color.body.grad_color = LV_COLOR_MAKE(0x3F, 0x47, 0x66);
    infile_page.btn_press_color.body.opa = LV_OPA_COVER;//设置背景色完全不透明
    infile_page.btn_press_color.text.color = LV_COLOR_WHITE;
	infile_page.btn_press_color.body.radius = 10;

	move_page.btn_len = mks_lv_btn_set_for_aglin_screen(mks_global.mks_src_3, move_page.btn_len, 110, 52, LV_ALIGN_IN_TOP_LEFT, 10, 10, event_handler);
	move_page.btn_speed = mks_lv_btn_set_for_aglin_screen(mks_global.mks_src_3, move_page.btn_speed, 110, 52, LV_ALIGN_IN_TOP_LEFT, 10, 74, event_handler);
	infile_page.btn_sculpture = mks_lv_btn_set_for_aglin_screen(mks_global.mks_src_3, infile_page.btn_sculpture, 110, 52, LV_ALIGN_IN_TOP_LEFT, 10, 138, event_handler);		

	lv_btn_set_style(move_page.btn_len, LV_BTN_STYLE_REL, &infile_page.btn_color);
	lv_btn_set_style(move_page.btn_len, LV_BTN_STYLE_PR, &infile_page.btn_press_color);

	lv_btn_set_style(move_page.btn_speed, LV_BTN_STYLE_REL, &infile_page.btn_color);
	lv_btn_set_style(move_page.btn_speed,LV_BTN_STYLE_PR,&infile_page.btn_press_color);

	lv_btn_set_style(infile_page.btn_sculpture, LV_BTN_STYLE_REL, &infile_page.btn_color);
	lv_btn_set_style(infile_page.btn_sculpture,LV_BTN_STYLE_PR,&infile_page.btn_press_color);
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

	infile_page.label_sculpture = mks_lvgl_long_sroll_label_with_wight_set_center(infile_page.btn_sculpture, infile_page.label_sculpture, 0, 0, "Sculpture", 100);
}

void infile_clean_obj(lv_obj_t *obj_src) {
	lv_obj_clean(obj_src);
}


