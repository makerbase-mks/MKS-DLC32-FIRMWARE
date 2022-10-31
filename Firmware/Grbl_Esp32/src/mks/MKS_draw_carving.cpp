#include "MKS_draw_carving.h"
#include "MKS_LVGL.h"
#include "FS.h"
#include "../SDCard.h"

 /* Screan Build */
// static lv_obj_t* scr;

MKS_FILE_LIST_t mks_file_list;

lv_obj_t* caving_src1;
static lv_obj_t* caving_Popup;
lv_obj_t* caving_read_file_src1;

/* style */
static lv_style_t popup_style;
static lv_style_t btn_style;
static lv_style_t caving_src1_style;
/* BTN */
static lv_obj_t* btn_popup_cancle;
static lv_obj_t* btn_popup_sure;
static lv_obj_t* btn_popup_frame;

bool file_popup_select_flag = false;


/* imgbtn */
lv_obj_t* up;
lv_obj_t* next;
lv_obj_t* Cback;

lv_obj_t* label_up;
lv_obj_t* label_next;
lv_obj_t* label_Cback;

lv_obj_t* file_0;
lv_obj_t* file_1;
lv_obj_t* file_2;
lv_obj_t* file_3;
lv_obj_t* file_4;
lv_obj_t* file_5;
lv_obj_t* file_6;
lv_obj_t* file_7;

lv_obj_t* file_list[8];
lv_obj_t* Label_file_list[8];

/* Label */
lv_obj_t* Label_file_0;
lv_obj_t* Label_file_1;
lv_obj_t* Label_file_2;
lv_obj_t* Label_file_3;
lv_obj_t* Label_file_4;
lv_obj_t* Label_file_5;
lv_obj_t* Label_file_6;
lv_obj_t* Label_file_7;

lv_obj_t* Label_NoFile;
lv_obj_t* Label_popup_cancel;
lv_obj_t* Label_popup_sure;
lv_obj_t* Label_popup;
lv_obj_t* Label_popup_file_name;

#define USE_TW_DRAW

LV_IMG_DECLARE(Previous);		//先申明此图片
LV_IMG_DECLARE(Next);			//先申明此图片
LV_IMG_DECLARE(back);			//先申明此图片
LV_IMG_DECLARE(file);			//先申明此图片
LV_IMG_DECLARE(FileDir);		//先申明此图片

LV_IMG_DECLARE(png_previous_pre);		
LV_IMG_DECLARE(png_next_pre);
LV_IMG_DECLARE(png_back_pre);
char file_print_send[128];

#define FILE_NUM		8
#define FIEL_NAME		128
char filename[FILE_NUM][FIEL_NAME];

static void event_handler_up(lv_obj_t* obj, lv_event_t event) {

	if (event == LV_EVENT_RELEASED) {

		char p[30];

		if(file_popup_select_flag == true) return;

		if(mks_readSD_Status() == SDState::NotPresent)  // check sdcard is work
		{

		}
		else{

			if(mks_file_list.file_page == 1) {

			}else {
			mks_file_list.file_count = 0;
				mks_file_list.file_page--;
				// mks_draw_file_loadig();
				// lv_refr_now(lv_refr_get_disp_refreshing());
				// mks_del_file_obj_1(mks_file_list.file_begin_num);
				mks_del_file_obj();
				mks_file_list.file_begin_num = 0;
				mks_listDir(SD, "/",MKS_FILE_DEEP);
				// draw_file_btmimg();
				// draw_file_btmimg_1(mks_file_list.file_begin_num);
				// lv_obj_del(caving_read_file_src1);
				SD.end();
			}
		}
	}
}

static void event_handler_next(lv_obj_t* obj, lv_event_t event) {

		if (event == LV_EVENT_RELEASED) {

			if(file_popup_select_flag == true) return;

			if(mks_readSD_Status() == SDState::NotPresent)  // check sdcard is work
			{ 
				
			}
			else {
			if (event == LV_EVENT_RELEASED) {
				if(mks_file_list.file_begin_num >= MKS_FILE_NUM) {
					mks_file_list.file_count = 0;
					mks_file_list.file_page++;
					// mks_draw_file_loadig();
					// lv_refr_now(lv_refr_get_disp_refreshing());
					mks_del_file_obj();
					mks_file_list.file_begin_num = 0;
					mks_listDir(SD, "/",MKS_FILE_DEEP);
					// draw_file_btmimg();
					// lv_obj_del(caving_read_file_src1);
					SD.end();
				}
			}
		}
	}
}

static void event_handler_cback(lv_obj_t* obj, lv_event_t event) {
	if (event == LV_EVENT_RELEASED) {
		mks_clear_craving();
		mks_ui_page.mks_ui_page = MKS_UI_PAGE_LOADING;
        mks_ui_page.wait_count = DEFAULT_UI_COUNT;
		mks_draw_ready();
	}
}

static void event_handler_file0(lv_obj_t* obj, lv_event_t event) {
	if (event == LV_EVENT_RELEASED) {
		// grbl_send(CLIENT_SERIAL, "file0\n");
		mks_file_list.file_choose = 0;
		
#if defined(USE_TW_DRAW)
	mks_ui_page.mks_ui_page = MKS_UI_PAGE_LOADING;
	mks_lv_clean_ui();
	get_print_file_name(mks_file_list.filename_str[0]);
	frame_ctrl.is_use_same_file = false;
	mks_draw_inFile(mks_file_list.filename_str[0]);
#else
	mks_draw_caving_popup(mks_file_list.file_begin_num, mks_file_list.filename_str[0]);
#endif
		

		
	}
}

static void event_handler_file1(lv_obj_t* obj, lv_event_t event) {
	if (event == LV_EVENT_RELEASED) {
		// grbl_send(CLIENT_SERIAL, "file1\n");
		mks_file_list.file_choose = 1;
#if defined(USE_TW_DRAW)
		mks_ui_page.mks_ui_page = MKS_UI_PAGE_LOADING;
		mks_lv_clean_ui();
		get_print_file_name(mks_file_list.filename_str[1]);
		frame_ctrl.is_use_same_file = false;
		mks_draw_inFile(mks_file_list.filename_str[1]);
#else
		mks_draw_caving_popup(mks_file_list.file_begin_num, mks_file_list.filename_str[1]);
#endif	
		
	}
}

static void event_handler_file2(lv_obj_t* obj, lv_event_t event) {
	if (event == LV_EVENT_RELEASED) {
		// grbl_send(CLIENT_SERIAL, "file2\n");
		mks_file_list.file_choose = 2;
		
#if defined(USE_TW_DRAW)
		mks_ui_page.mks_ui_page = MKS_UI_PAGE_LOADING;
		mks_lv_clean_ui();
		get_print_file_name(mks_file_list.filename_str[2]);
		frame_ctrl.is_use_same_file = false;
		mks_draw_inFile(mks_file_list.filename_str[2]);
#else
		mks_draw_caving_popup(mks_file_list.file_begin_num, mks_file_list.filename_str[2]);
#endif
	}
}

static void event_handler_file3(lv_obj_t* obj, lv_event_t event) {
	if (event == LV_EVENT_RELEASED) {
		// grbl_send(CLIENT_SERIAL, "file3\n");
		mks_file_list.file_choose = 3;
#if defined(USE_TW_DRAW)
		mks_ui_page.mks_ui_page = MKS_UI_PAGE_LOADING;
		mks_lv_clean_ui();
		get_print_file_name(mks_file_list.filename_str[3]);
		frame_ctrl.is_use_same_file = false;
		mks_draw_inFile(mks_file_list.filename_str[3]);
#else
		mks_draw_caving_popup(mks_file_list.file_begin_num, mks_file_list.filename_str[3]);
#endif
		
	}
}

static void event_handler_file4(lv_obj_t* obj, lv_event_t event) {
	if (event == LV_EVENT_RELEASED) {
		// grbl_send(CLIENT_SERIAL, "file4\n");
		mks_file_list.file_choose = 4;
		
#if defined(USE_TW_DRAW)
		mks_ui_page.mks_ui_page = MKS_UI_PAGE_LOADING;
		mks_lv_clean_ui();
		get_print_file_name(mks_file_list.filename_str[4]);
		frame_ctrl.is_use_same_file = false;
		mks_draw_inFile(mks_file_list.filename_str[4]);
#else
		mks_draw_caving_popup(mks_file_list.file_begin_num, mks_file_list.filename_str[4]);
#endif
	}
}

static void event_handler_file5(lv_obj_t* obj, lv_event_t event) {
	if (event == LV_EVENT_RELEASED) {
		// grbl_send(CLIENT_SERIAL, "file5\n");
		mks_file_list.file_choose = 5;
		
#if defined(USE_TW_DRAW)
		mks_ui_page.mks_ui_page = MKS_UI_PAGE_LOADING;
		mks_lv_clean_ui();
		get_print_file_name(mks_file_list.filename_str[5]);
		frame_ctrl.is_use_same_file = false;
		mks_draw_inFile(mks_file_list.filename_str[5]);
#else
		mks_draw_caving_popup(mks_file_list.file_begin_num, mks_file_list.filename_str[5]);
#endif
	}
}

static void event_handler_file6(lv_obj_t* obj, lv_event_t event) {
	if (event == LV_EVENT_RELEASED) {
		// grbl_send(CLIENT_SERIAL, "file6\n");
		mks_file_list.file_choose = 6;
		
#if defined(USE_TW_DRAW)
		mks_ui_page.mks_ui_page = MKS_UI_PAGE_LOADING;
		mks_lv_clean_ui();
		get_print_file_name(mks_file_list.filename_str[6]);
		frame_ctrl.is_use_same_file = false;
		mks_draw_inFile(mks_file_list.filename_str[6]);
#else 
		mks_draw_caving_popup(mks_file_list.file_begin_num, mks_file_list.filename_str[6]);
#endif
	}
}

static void event_handler_file7(lv_obj_t* obj, lv_event_t event) {
	if (event == LV_EVENT_RELEASED) {
		// grbl_send(CLIENT_SERIAL, "file7\n");
		mks_file_list.file_choose = 7;
		
#if defined(USE_TW_DRAW)
		mks_ui_page.mks_ui_page = MKS_UI_PAGE_LOADING;
		mks_lv_clean_ui();
		get_print_file_name(mks_file_list.filename_str[7]);
		frame_ctrl.is_use_same_file = false;
		mks_draw_inFile(mks_file_list.filename_str[7]);
#else
		mks_draw_caving_popup(mks_file_list.file_begin_num, mks_file_list.filename_str[7]);
#endif
	}
}


void mks_draw_craving(void) {

	SDState state = get_sd_state(true);

	mks_global.mks_src_1 = lv_obj_create(mks_global.mks_src, NULL);
	lv_obj_set_size(mks_global.mks_src_1 ,caving_src1_size_x, caving_src1_size_y);
	lv_obj_set_pos(mks_global.mks_src_1, caving_src1_x, caving_src1_y);
	lv_obj_set_style(mks_global.mks_src_1, &mks_global.mks_src_1_style);

	up = lv_imgbtn_creat_mks(mks_global.mks_src_1, up, &png_previous_pre, &Previous, LV_ALIGN_IN_RIGHT_MID, caving_up_x, caving_up_y, event_handler_up);
	next = lv_imgbtn_creat_mks(mks_global.mks_src_1, next, &png_next_pre, &Next, LV_ALIGN_IN_RIGHT_MID, caving_next_x, caving_next_y, event_handler_next);
	Cback = lv_imgbtn_creat_mks(mks_global.mks_src_1, Cback, &png_back_pre, &back, LV_ALIGN_IN_LEFT_MID, 10, -15, event_handler_cback);

	label_for_imgbtn_name(mks_global.mks_src_1, label_up, up, 0, 0, "Up");
	label_for_imgbtn_name(mks_global.mks_src_1, label_next, next, 0, 0, "Next");
	label_for_imgbtn_name(mks_global.mks_src_1, label_Cback, Cback, 0, 0, "Back");

	// label_Cback = mks_lvgl_long_sroll_label_with_wight_set_center(mks_global.mks_src_1, label_Cback, caving_back_x,caving_back_y, "Back", 60);
	// label_up = mks_lvgl_long_sroll_label_with_wight_set_center(mks_global.mks_src_1, label_up, caving_up_x+20, caving_up_y+70, "UP", 60);
	// label_next = mks_lvgl_long_sroll_label_with_wight_set_center(mks_global.mks_src_1, label_next, caving_next_x, caving_next_y+70, "Next", 60);

	// if(mks_readSD_Status() == SDState::NotPresent)  // check sdcard is work
	if(state == SDState::NotPresent)
	{
		mks_grbl.mks_sd_status = 0;	// no sd insert
		label_for_screen(mks_global.mks_src, Label_NoFile, 0, 0, "No SD Card");
	}else {

		
		mks_file_list.file_begin_num = 0;
		mks_file_list.file_count = 0;
		mks_file_list.file_page = 1;
		mks_grbl.mks_sd_status = 1; // sd had inserted

		mks_listDir(SD, "/",MKS_FILE_DEEP);
		SD.end();
	}	
	mks_ui_page.mks_ui_page = MKS_UI_Caving;
    mks_ui_page.wait_count = DEFAULT_UI_COUNT;
}

void draw_filexx(uint8_t num, char *name) {

	if(name[0] == '/') name[0] = ' ';

	switch(num) {
		case 0:
		file_0 = lv_imgbtn_creat_mks(mks_global.mks_src, file_0, &file, &file, LV_ALIGN_CENTER, caving_first_file_x, caving_first_file_y, event_handler_file0);
		Label_file_0 = label_for_file(mks_global.mks_src, Label_file_0, 
																	caving_first_file_label_x, 
																	caving_first_file_label_y, 
																	name,
																	caving_file_name_show);
		break;

		case 1:
		file_1 = lv_imgbtn_creat_mks(mks_global.mks_src, file_1, &file, &file, LV_ALIGN_CENTER, caving_first_file_x + 120, caving_first_file_y, event_handler_file1);
		Label_file_1 = label_for_file(mks_global.mks_src, 
										Label_file_1, 
										caving_first_file_label_x + 120, 
										caving_first_file_label_y, 
										name,
										caving_file_name_show);
		break;
		
		case 2:
		file_2 = lv_imgbtn_creat_mks(mks_global.mks_src, file_0, &file, &file, LV_ALIGN_CENTER, caving_first_file_x + 240, caving_first_file_y, event_handler_file2);
		Label_file_2 = label_for_file(mks_global.mks_src, 
										Label_file_2, 
										caving_first_file_label_x + 240, 
										caving_first_file_label_y, 
										name,
										caving_file_name_show);
		break;

		case 3:
		file_3 = lv_imgbtn_creat_mks(mks_global.mks_src, file_3, &file, &file, LV_ALIGN_CENTER, caving_first_file_x + 360, caving_first_file_y, event_handler_file3);
		Label_file_3 = label_for_file(mks_global.mks_src, 
			Label_file_3, 
			caving_first_file_label_x + 360, 
			caving_first_file_label_y, 
			name,
			caving_file_name_show);
		break;

		case 4:
		file_4 = lv_imgbtn_creat_mks(mks_global.mks_src, file_4, &file, &file, LV_ALIGN_CENTER, caving_first_file_x, caving_first_file_y + 105, event_handler_file4);
		Label_file_4 = label_for_file(mks_global.mks_src, 
			Label_file_4, 
			caving_first_file_label_x, 
			caving_first_file_label_y + 105, 
			name,
			caving_file_name_show);
		break;

		case 5:
		file_5 = lv_imgbtn_creat_mks(mks_global.mks_src, file_5, &file, &file, LV_ALIGN_CENTER, caving_first_file_x + 120, caving_first_file_y + 105, event_handler_file5);
		Label_file_5 = label_for_file(mks_global.mks_src, 
				Label_file_5, 
				caving_first_file_label_x + 120, 
				caving_first_file_label_y + 105, 
				// mks_file_list.filename_str[5], 
				name,
				caving_file_name_show);
		break;

		case 6:
		file_6 = lv_imgbtn_creat_mks(mks_global.mks_src, file_6, &file, &file, LV_ALIGN_CENTER, caving_first_file_x + 240, caving_first_file_y + 105, event_handler_file6);
		Label_file_6 = label_for_file(mks_global.mks_src, 
				Label_file_6, 
				caving_first_file_label_x + 240, 
				caving_first_file_label_y + 105, 
				// mks_file_list.filename_str[6], 
				name,
				caving_file_name_show);
		break;

		case 7:
		file_7 = lv_imgbtn_creat_mks(mks_global.mks_src, file_7, &file, &file, LV_ALIGN_CENTER, caving_first_file_x + 360, caving_first_file_y + 105, event_handler_file7);
		Label_file_7 = label_for_file(mks_global.mks_src, 
				Label_file_7, 
				caving_first_file_label_x + 360, 
				caving_first_file_label_y + 105, 
				// mks_file_list.filename_str[7], 
				name,
				caving_file_name_show);
		break;
	}

	lv_refr_now(lv_refr_get_disp_refreshing());
}





void draw_file_btmimg(void) {

	char filename_dis_str[MKS_FILE_NUM][MKS_FILE_NAME_LENGTH];

	for(uint8_t i=0; i<mks_file_list.file_begin_num; i++) {
		strcpy(filename_dis_str[i], mks_file_list.filename_str[i]);
		if(filename_dis_str[i][0] == '/') filename_dis_str[i][0] = ' ';
	}
	
	if(mks_file_list.file_begin_num == 1) {
		file_0 = lv_imgbtn_creat_mks(mks_global.mks_src, file_0, &file, &file, LV_ALIGN_CENTER, caving_first_file_x, caving_first_file_y, event_handler_file0);
		Label_file_0 = label_for_file(mks_global.mks_src, Label_file_0, 
																		caving_first_file_label_x, 
																		caving_first_file_label_y, 
																		// mks_file_list.filename_str[0], 
																		filename_dis_str[0],
																		caving_file_name_show);
	}
	else if(mks_file_list.file_begin_num == 2) {
		file_0 = lv_imgbtn_creat_mks(mks_global.mks_src, 
									file_0, 
									&file, 
									&file, 
									LV_ALIGN_CENTER, 
									caving_first_file_x, 
									caving_first_file_y, 
									event_handler_file0);
		Label_file_0 = label_for_file(mks_global.mks_src, Label_file_0, 
																		caving_first_file_label_x, 
																		caving_first_file_label_y, 
																		// mks_file_list.filename_str[0],
																		filename_dis_str[0],
																		caving_file_name_show);

		file_1 = lv_imgbtn_creat_mks(mks_global.mks_src, file_1, &file, &file, LV_ALIGN_CENTER, caving_first_file_x + 120, caving_first_file_y, event_handler_file1);
		Label_file_1 = label_for_file(mks_global.mks_src, 
																		Label_file_1, 
																		caving_first_file_label_x + 120, 
																		caving_first_file_label_y, 
																		// mks_file_list.filename_str[1], 
																		filename_dis_str[1],
																		caving_file_name_show);	
	}
	else if(mks_file_list.file_begin_num == 3) {

		file_0 = lv_imgbtn_creat_mks(mks_global.mks_src, file_0, &file, &file, LV_ALIGN_CENTER, caving_first_file_x, caving_first_file_y, event_handler_file0);
		Label_file_0 = label_for_file(mks_global.mks_src, 
			Label_file_0, 
			caving_first_file_label_x, 
			caving_first_file_label_y, 
			// mks_file_list.filename_str[0],
			filename_dis_str[0], 
			caving_file_name_show);

		file_1 = lv_imgbtn_creat_mks(mks_global.mks_src, file_1, &file, &file, LV_ALIGN_CENTER, caving_first_file_x + 120, caving_first_file_y, event_handler_file1);
		Label_file_1 = label_for_file(mks_global.mks_src, 
			Label_file_1, 
			caving_first_file_label_x + 120, 
			caving_first_file_label_y, 
			// mks_file_list.filename_str[1], 
			filename_dis_str[1],
			caving_file_name_show);

		file_2 = lv_imgbtn_creat_mks(mks_global.mks_src, file_0, &file, &file, LV_ALIGN_CENTER, caving_first_file_x + 240, caving_first_file_y, event_handler_file2);
		Label_file_2 = label_for_file(mks_global.mks_src, 
			Label_file_2, 
			caving_first_file_label_x + 240, 
			caving_first_file_label_y, 
			// mks_file_list.filename_str[2], 
			filename_dis_str[2],
			caving_file_name_show);
	}
	else if(mks_file_list.file_begin_num == 4) {

		file_0 = lv_imgbtn_creat_mks(mks_global.mks_src, file_0, &file, &file, LV_ALIGN_CENTER, caving_first_file_x, caving_first_file_y, event_handler_file0);
		Label_file_0 = label_for_file(mks_global.mks_src, 
			Label_file_0, 
			caving_first_file_label_x, 
			caving_first_file_label_y, 
			// mks_file_list.filename_str[0], 
			filename_dis_str[0],
			caving_file_name_show);

		file_1 = lv_imgbtn_creat_mks(mks_global.mks_src, file_1, &file, &file, LV_ALIGN_CENTER, caving_first_file_x + 120, caving_first_file_y, event_handler_file1);
		Label_file_1 = label_for_file(mks_global.mks_src, 
			Label_file_1, 
			caving_first_file_label_x + 120, 
			caving_first_file_label_y, 
			// mks_file_list.filename_str[1],
			filename_dis_str[1], 
			caving_file_name_show);

		file_2 = lv_imgbtn_creat_mks(mks_global.mks_src, file_0, &file, &file, LV_ALIGN_CENTER, caving_first_file_x + 240, caving_first_file_y, event_handler_file2);
		Label_file_2 = label_for_file(mks_global.mks_src, 
			Label_file_2, 
			caving_first_file_label_x + 240, 
			caving_first_file_label_y, 
			// mks_file_list.filename_str[2], 
			filename_dis_str[2],
			caving_file_name_show);

		file_3 = lv_imgbtn_creat_mks(mks_global.mks_src, file_3, &file, &file, LV_ALIGN_CENTER, caving_first_file_x + 360, caving_first_file_y, event_handler_file3);
		Label_file_3 = label_for_file(mks_global.mks_src, 
			Label_file_3, 
			caving_first_file_label_x + 360, 
			caving_first_file_label_y, 
			// mks_file_list.filename_str[3], 
			filename_dis_str[3],
			caving_file_name_show);
	}
	else if(mks_file_list.file_begin_num == 5) {
		file_0 = lv_imgbtn_creat_mks(mks_global.mks_src, file_0, &file, &file, LV_ALIGN_CENTER, caving_first_file_x, caving_first_file_y, event_handler_file0);
		Label_file_0 = label_for_file(mks_global.mks_src, 
			Label_file_0, 
			caving_first_file_label_x, 
			caving_first_file_label_y, 
			// mks_file_list.filename_str[0],
			filename_dis_str[0], 
			caving_file_name_show);

		file_1 = lv_imgbtn_creat_mks(mks_global.mks_src, file_1, &file, &file, LV_ALIGN_CENTER, caving_first_file_x + 120, caving_first_file_y, event_handler_file1);
		Label_file_1 = label_for_file(mks_global.mks_src, 
			Label_file_1, 
			caving_first_file_label_x + 120, 
			caving_first_file_label_y, 
			// mks_file_list.filename_str[1], 
			filename_dis_str[1],
			caving_file_name_show);

		file_2 = lv_imgbtn_creat_mks(mks_global.mks_src, file_0, &file, &file, LV_ALIGN_CENTER, caving_first_file_x + 240, caving_first_file_y, event_handler_file2);
		Label_file_2 = label_for_file(mks_global.mks_src, 
			Label_file_2, 
			caving_first_file_label_x + 240, 
			caving_first_file_label_y, 
			// mks_file_list.filename_str[2], 
			filename_dis_str[2],
			caving_file_name_show);

		file_3 = lv_imgbtn_creat_mks(mks_global.mks_src, file_3, &file, &file, LV_ALIGN_CENTER, caving_first_file_x + 360, caving_first_file_y, event_handler_file3);
		Label_file_3 = label_for_file(mks_global.mks_src, 
			Label_file_3, 
			caving_first_file_label_x + 360, 
			caving_first_file_label_y, 
			// mks_file_list.filename_str[3], 
			filename_dis_str[3],
			caving_file_name_show);

		file_4 = lv_imgbtn_creat_mks(mks_global.mks_src, file_4, &file, &file, LV_ALIGN_CENTER, caving_first_file_x, caving_first_file_y + 105, event_handler_file4);
		Label_file_4 = label_for_file(mks_global.mks_src, 
			Label_file_4, 
			caving_first_file_label_x, 
			caving_first_file_label_y + 105, 
			// mks_file_list.filename_str[4], 
			filename_dis_str[4],
			caving_file_name_show);
	}
	else if(mks_file_list.file_begin_num == 6) {

		file_0 = lv_imgbtn_creat_mks(mks_global.mks_src, file_0, &file, &file, LV_ALIGN_CENTER, caving_first_file_x, caving_first_file_y, event_handler_file0);
		Label_file_0 = label_for_file(mks_global.mks_src, 
			Label_file_0, 
			caving_first_file_label_x, 
			caving_first_file_label_y, 
			// mks_file_list.filename_str[0], 
			filename_dis_str[0],
			caving_file_name_show);

		file_1 = lv_imgbtn_creat_mks(mks_global.mks_src, file_1, &file, &file, LV_ALIGN_CENTER, caving_first_file_x + 120, caving_first_file_y, event_handler_file1);
		Label_file_1 = label_for_file(mks_global.mks_src, 
			Label_file_1, 
			caving_first_file_label_x + 120, 
			caving_first_file_label_y, 
			// mks_file_list.filename_str[1], 
			filename_dis_str[1],
			caving_file_name_show);

		file_2 = lv_imgbtn_creat_mks(mks_global.mks_src, file_0, &file, &file, LV_ALIGN_CENTER, caving_first_file_x + 240, caving_first_file_y, event_handler_file2);
		Label_file_2 = label_for_file(mks_global.mks_src, 
			Label_file_2, 
			caving_first_file_label_x + 240, 
			caving_first_file_label_y, 
			// mks_file_list.filename_str[2], 
			filename_dis_str[2],
			caving_file_name_show);

		file_3 = lv_imgbtn_creat_mks(mks_global.mks_src, file_3, &file, &file, LV_ALIGN_CENTER, caving_first_file_x + 360, caving_first_file_y, event_handler_file3);
		Label_file_3 = label_for_file(mks_global.mks_src, 
			Label_file_3, 
			caving_first_file_label_x + 360, 
			caving_first_file_label_y, 
			// mks_file_list.filename_str[3], 
			filename_dis_str[3],
			caving_file_name_show);

		file_4 = lv_imgbtn_creat_mks(mks_global.mks_src, file_4, &file, &file, LV_ALIGN_CENTER, caving_first_file_x, caving_first_file_y + 105, event_handler_file4);
		Label_file_4 = label_for_file(mks_global.mks_src, 
			Label_file_4, 
			caving_first_file_label_x, 
			caving_first_file_label_y + 105, 
			// mks_file_list.filename_str[4], 
			filename_dis_str[4],
			caving_file_name_show);

		file_5 = lv_imgbtn_creat_mks(mks_global.mks_src, file_5, &file, &file, LV_ALIGN_CENTER, caving_first_file_x + 120, caving_first_file_y + 105, event_handler_file5);
		Label_file_5 = label_for_file(mks_global.mks_src, 
			Label_file_5, 
			caving_first_file_label_x + 120, 
			caving_first_file_label_y + 105, 
			// mks_file_list.filename_str[5], 
			filename_dis_str[5],
			caving_file_name_show);
	}
	else if(mks_file_list.file_begin_num == 7) {
		file_0 = lv_imgbtn_creat_mks(mks_global.mks_src, file_0, &file, &file, LV_ALIGN_CENTER, caving_first_file_x, caving_first_file_y, event_handler_file0);
		Label_file_0 = label_for_file(mks_global.mks_src, 
			Label_file_0, 
			caving_first_file_label_x, 
			caving_first_file_label_y, 
			// mks_file_list.filename_str[0],
			filename_dis_str[0],
			caving_file_name_show);

		file_1 = lv_imgbtn_creat_mks(mks_global.mks_src, file_1, &file, &file, LV_ALIGN_CENTER, caving_first_file_x + 120, caving_first_file_y, event_handler_file1);
		Label_file_1 = label_for_file(mks_global.mks_src, 
			Label_file_1, 
			caving_first_file_label_x + 120, 
			caving_first_file_label_y, 
			// mks_file_list.filename_str[1], 
			filename_dis_str[1],
			caving_file_name_show);

		file_2 = lv_imgbtn_creat_mks(mks_global.mks_src, file_0, &file, &file, LV_ALIGN_CENTER, caving_first_file_x + 240, caving_first_file_y, event_handler_file2);
		Label_file_2 = label_for_file(mks_global.mks_src, 
			Label_file_2, 
			caving_first_file_label_x + 240, 
			caving_first_file_label_y, 
			// mks_file_list.filename_str[2], 
			filename_dis_str[2],
			caving_file_name_show);

		file_3 = lv_imgbtn_creat_mks(mks_global.mks_src, file_3, &file, &file, LV_ALIGN_CENTER, caving_first_file_x + 360, caving_first_file_y, event_handler_file3);
		Label_file_3 = label_for_file(mks_global.mks_src, 
			Label_file_3, 
			caving_first_file_label_x + 360, 
			caving_first_file_label_y, 
			// mks_file_list.filename_str[3], 
			filename_dis_str[3],
			caving_file_name_show);

		file_4 = lv_imgbtn_creat_mks(mks_global.mks_src, file_4, &file, &file, LV_ALIGN_CENTER, caving_first_file_x, caving_first_file_y + 105, event_handler_file4);
		Label_file_4 = label_for_file(mks_global.mks_src, 
			Label_file_4, 
			caving_first_file_label_x, 
			caving_first_file_label_y + 105, 
			// mks_file_list.filename_str[4], 
			filename_dis_str[4],
			caving_file_name_show);

		file_5 = lv_imgbtn_creat_mks(mks_global.mks_src, file_5, &file, &file, LV_ALIGN_CENTER, caving_first_file_x + 120, caving_first_file_y + 105, event_handler_file5);
		Label_file_5 = label_for_file(mks_global.mks_src, 
			Label_file_5, 
			caving_first_file_label_x + 120, 
			caving_first_file_label_y + 105, 
			// mks_file_list.filename_str[5], 
			filename_dis_str[5],
			caving_file_name_show);

		file_6 = lv_imgbtn_creat_mks(mks_global.mks_src, file_6, &file, &file, LV_ALIGN_CENTER, caving_first_file_x + 240, caving_first_file_y + 105, event_handler_file6);
		Label_file_6 = label_for_file(mks_global.mks_src, 
			Label_file_6, 
			caving_first_file_label_x + 240, 
			caving_first_file_label_y + 105, 
			// mks_file_list.filename_str[6], 
			filename_dis_str[6],
			caving_file_name_show);
	}
	else if(mks_file_list.file_begin_num == 8) {

		file_0 = lv_imgbtn_creat_mks(mks_global.mks_src, file_0, &file, &file, LV_ALIGN_CENTER, caving_first_file_x, caving_first_file_y, event_handler_file0);
		Label_file_0 = label_for_file(mks_global.mks_src, Label_file_0, caving_first_file_x, caving_first_file_y+30, filename_dis_str[0], caving_file_name_show);

	
		file_1 = lv_imgbtn_creat_mks(mks_global.mks_src, file_1, &file, &file, LV_ALIGN_CENTER, caving_first_file_x + 120, caving_first_file_y, event_handler_file1);
		Label_file_1 = label_for_file(mks_global.mks_src, 
				Label_file_1, 
				caving_first_file_label_x + 120, 
				caving_first_file_label_y, 
				// mks_file_list.filename_str[1], 
				filename_dis_str[1],
				caving_file_name_show);

		file_2 = lv_imgbtn_creat_mks(mks_global.mks_src, file_0, &file, &file, LV_ALIGN_CENTER, caving_first_file_x + 240, caving_first_file_y, event_handler_file2);
		Label_file_2 = label_for_file(mks_global.mks_src, 
				Label_file_2, 
				caving_first_file_label_x + 240, 
				caving_first_file_label_y, 
				// mks_file_list.filename_str[2], 
				filename_dis_str[2],
				caving_file_name_show);

		file_3 = lv_imgbtn_creat_mks(mks_global.mks_src, file_3, &file, &file, LV_ALIGN_CENTER, caving_first_file_x + 360, caving_first_file_y, event_handler_file3);
		Label_file_3 = label_for_file(mks_global.mks_src, 
				Label_file_3, 
				caving_first_file_label_x + 360, 
				caving_first_file_label_y, 
				// mks_file_list.filename_str[3], 
				filename_dis_str[3],
				caving_file_name_show);

		file_4 = lv_imgbtn_creat_mks(mks_global.mks_src, file_4, &file, &file, LV_ALIGN_CENTER, caving_first_file_x, caving_first_file_y + 105, event_handler_file4);
		Label_file_4 = label_for_file(mks_global.mks_src, 
				Label_file_4, 
				caving_first_file_label_x, 
				caving_first_file_label_y + 105, 
				// mks_file_list.filename_str[4], 
				filename_dis_str[4],
				caving_file_name_show);

		file_5 = lv_imgbtn_creat_mks(mks_global.mks_src, file_5, &file, &file, LV_ALIGN_CENTER, caving_first_file_x + 120, caving_first_file_y + 105, event_handler_file5);
		Label_file_5 = label_for_file(mks_global.mks_src, 
				Label_file_5, 
				caving_first_file_label_x + 120, 
				caving_first_file_label_y + 105, 
				// mks_file_list.filename_str[5], 
				filename_dis_str[5],
				caving_file_name_show);

		file_6 = lv_imgbtn_creat_mks(mks_global.mks_src, file_6, &file, &file, LV_ALIGN_CENTER, caving_first_file_x + 240, caving_first_file_y + 105, event_handler_file6);
		Label_file_6 = label_for_file(mks_global.mks_src, 
				Label_file_6, 
				caving_first_file_label_x + 240, 
				caving_first_file_label_y + 105, 
				// mks_file_list.filename_str[6], 
				filename_dis_str[6],
				caving_file_name_show);

		file_7 = lv_imgbtn_creat_mks(mks_global.mks_src, file_7, &file, &file, LV_ALIGN_CENTER, caving_first_file_x + 360, caving_first_file_y + 105, event_handler_file7);
		Label_file_7 = label_for_file(mks_global.mks_src, 
				Label_file_7, 
				caving_first_file_label_x + 360, 
				caving_first_file_label_y + 105, 
				// mks_file_list.filename_str[7], 
				filename_dis_str[7],
				caving_file_name_show);
	}
	
}

void mks_del_file_obj(void) {

	if(mks_file_list.file_begin_num == 1) {
		lv_obj_del(file_0);
		lv_obj_del(Label_file_0);
	}
	else if(mks_file_list.file_begin_num == 2){
		lv_obj_del(file_0);
		lv_obj_del(file_1);
		lv_obj_del(Label_file_0);
		lv_obj_del(Label_file_1);
	}
	else if(mks_file_list.file_begin_num == 3){
		lv_obj_del(file_0);
		lv_obj_del(file_1);
		lv_obj_del(file_2);
		lv_obj_del(Label_file_0);
		lv_obj_del(Label_file_1);
		lv_obj_del(Label_file_2);
	}
	else if(mks_file_list.file_begin_num == 4){
		lv_obj_del(file_0);
		lv_obj_del(file_1);
		lv_obj_del(file_2);
		lv_obj_del(file_3);
		lv_obj_del(Label_file_0);
		lv_obj_del(Label_file_1);
		lv_obj_del(Label_file_2);
		lv_obj_del(Label_file_3);

	}
	else if(mks_file_list.file_begin_num == 5){
		lv_obj_del(file_0);
		lv_obj_del(file_1);
		lv_obj_del(file_2);
		lv_obj_del(file_3);
		lv_obj_del(file_4);
		lv_obj_del(Label_file_0);
		lv_obj_del(Label_file_1);
		lv_obj_del(Label_file_2);
		lv_obj_del(Label_file_3);
		lv_obj_del(Label_file_4);
	}
	else if(mks_file_list.file_begin_num == 6){
		lv_obj_del(file_0);
		lv_obj_del(file_1);
		lv_obj_del(file_2);
		lv_obj_del(file_3);
		lv_obj_del(file_4);
		lv_obj_del(file_5);
		lv_obj_del(Label_file_0);
		lv_obj_del(Label_file_1);
		lv_obj_del(Label_file_2);
		lv_obj_del(Label_file_3);
		lv_obj_del(Label_file_4);
		lv_obj_del(Label_file_5);
	}
	else if(mks_file_list.file_begin_num == 7){
		lv_obj_del(file_0);
		lv_obj_del(file_1);
		lv_obj_del(file_2);
		lv_obj_del(file_3);
		lv_obj_del(file_4);
		lv_obj_del(file_5);
		lv_obj_del(file_6);
		lv_obj_del(Label_file_0);
		lv_obj_del(Label_file_1);
		lv_obj_del(Label_file_2);
		lv_obj_del(Label_file_3);
		lv_obj_del(Label_file_4);
		lv_obj_del(Label_file_5);
		lv_obj_del(Label_file_6);
	}
	else if(mks_file_list.file_begin_num == 8){
		lv_obj_del(file_0);
		lv_obj_del(file_1);
		lv_obj_del(file_2);
		lv_obj_del(file_3);
		lv_obj_del(file_4);
		lv_obj_del(file_5);
		lv_obj_del(file_6);
		lv_obj_del(file_7); 

		lv_obj_del(Label_file_0);
		lv_obj_del(Label_file_1);
		lv_obj_del(Label_file_2);
		lv_obj_del(Label_file_3);
		lv_obj_del(Label_file_4);
		lv_obj_del(Label_file_5);
		lv_obj_del(Label_file_6);
		lv_obj_del(Label_file_7);
	}
}

void disable_file_click() {

	if(mks_file_list.file_begin_num == 1) {
		lv_obj_set_click(move_page.Back, true);
	}
	else if(mks_file_list.file_begin_num == 2) {

	}
}

static void event_btn_cancle(lv_obj_t* obj, lv_event_t event) {

    if (event == LV_EVENT_RELEASED) {
		file_popup_select_flag = false;
        lv_obj_del(caving_Popup);
	}
}

void start_print(void) { 

	char str_cmd[255] = "[ESP220]";
	char line_num[50];
	file_popup_select_flag = false;
	mks_grbl.is_mks_ts35_flag = true;

	mks_ui_page.mks_ui_page = MKS_UI_PAGE_LOADING; 
	mks_ui_page.wait_count = DEFAULT_UI_COUNT;
 
	strcat(str_cmd, file_print_send);
	strcat(str_cmd,"\n");
	MKS_GRBL_CMD_SEND(str_cmd);
	grbl_send(CLIENT_SERIAL, str_cmd);
	mks_draw_print();
}

static void event_btn_sure(lv_obj_t* obj, lv_event_t event) {
	char str_cmd[255] = "[ESP220]";
	char line_num[50];
    if (event == LV_EVENT_RELEASED) {

		file_popup_select_flag = false;

		if(sys.state != State::Idle) {
			lv_obj_del(caving_Popup);
			return ;
		}
		mks_grbl.is_mks_ts35_flag = true;

		mks_ui_page.mks_ui_page = MKS_UI_PAGE_LOADING; 
		mks_ui_page.wait_count = DEFAULT_UI_COUNT;
        lv_obj_del(caving_Popup);
		mks_clear_craving();

		ddxd = sd_get_current_line_number();
		sprintf(line_num ,"%d", ddxd);
        tf.writeFile("/PLA.txt", line_num);

		strcat(str_cmd, file_print_send);
		strcat(str_cmd,"\n");
		MKS_GRBL_CMD_SEND(str_cmd);
		grbl_send(CLIENT_SERIAL, str_cmd);
		mks_draw_print();
	}
}

static void event_btn_sure_alarm(lv_obj_t* obj, lv_event_t event) {

	if (event == LV_EVENT_RELEASED) {
		lv_obj_del(caving_Popup);
	}
}

static void event_fram_size_yes(lv_obj_t* obj, lv_event_t event) {

	if (event == LV_EVENT_RELEASED) {
		lv_obj_del(com_p1.com_popup_src);
		mks_ui_page.mks_ui_page = MKS_UI_PAGE_LOADING;
        mks_ui_page.wait_count = 1;
		mks_draw_frame();
		lv_refr_now(lv_refr_get_disp_refreshing());
		(frame_ctrl.file_name);
		file_popup_select_flag = false;
	}
}

static void event_fram_size_no(lv_obj_t* obj, lv_event_t event) {

	if (event == LV_EVENT_RELEASED) {
		file_popup_select_flag = false;
		lv_obj_del(com_p1.com_popup_src);
	}
}

static void event_btn_frame(lv_obj_t* obj, lv_event_t event) {

	uint32_t file_size = mks_file_list.file_size[mks_file_list.file_choose];   

	if (event == LV_EVENT_RELEASED) {

		lv_obj_del(caving_Popup);

		if(file_size >= 1024*1024) {
			mks_draw_common_popup("Warning!", 
								"File size is too big",
								"Do you want to connute?",
								event_fram_size_yes,
								event_fram_size_no);
		}else {
			
			mks_draw_frame();
			lv_refr_now(lv_refr_get_disp_refreshing());
			// mks_run_frame(frame_ctrl.file_name);
			file_popup_select_flag = false;
		}
	}
}

void mks_draw_caving_popup(uint8_t text, char *srt) {

	char file_name[128];

	if(file_popup_select_flag == true) return;

	file_popup_select_flag = true;

	caving_Popup = lv_obj_create(mks_global.mks_src, NULL);

	lv_obj_set_size(caving_Popup ,350, 200);
	lv_obj_set_pos(caving_Popup, 80,50);

	lv_style_copy(&popup_style, &lv_style_scr);
	popup_style.body.main_color = LV_COLOR_MAKE(0xCE, 0xD6, 0xE5); 
    popup_style.body.grad_color = LV_COLOR_MAKE(0xCE, 0xD6, 0xE5); 
	popup_style.text.color = LV_COLOR_BLACK;
	popup_style.body.radius = 17;
	lv_obj_set_style(caving_Popup, &popup_style);
	
	lv_style_copy(&btn_style, &lv_style_scr);
    btn_style.body.main_color = LV_COLOR_MAKE(0x3F, 0x46, 0x66);
    btn_style.body.grad_color = LV_COLOR_MAKE(0x3F, 0x46, 0x66);
	btn_style.body.radius = 10;
    btn_style.body.opa = LV_OPA_COVER; // 设置背景色完全不透明
    btn_style.text.color = LV_COLOR_WHITE;
	
	btn_popup_sure = mks_lv_btn_set(caving_Popup, btn_popup_sure, 100,40,10,130,event_btn_sure);
	lv_btn_set_style(btn_popup_sure, LV_BTN_STYLE_REL, &btn_style);
    lv_btn_set_style(btn_popup_sure,LV_BTN_STYLE_PR,&btn_style);
	mks_lvgl_long_sroll_label_with_wight_set_center(btn_popup_sure, Label_popup_sure, 50, 0, "Yes",50);


	btn_popup_frame = mks_lv_btn_set(caving_Popup, btn_popup_frame, 100,40,125,130,event_btn_frame);
	lv_btn_set_style(btn_popup_frame, LV_BTN_STYLE_REL, &btn_style);
    lv_btn_set_style(btn_popup_frame,LV_BTN_STYLE_PR,&btn_style);
	mks_lvgl_long_sroll_label_with_wight_set_center(btn_popup_frame, Label_popup_sure, 60, 0, "Frame",50);
	
	btn_popup_cancle = mks_lv_btn_set(caving_Popup, btn_popup_cancle, 100,40,240,130,event_btn_cancle);
	lv_btn_set_style(btn_popup_cancle, LV_BTN_STYLE_REL, &btn_style);
    lv_btn_set_style(btn_popup_cancle,LV_BTN_STYLE_PR,&btn_style);
	mks_lvgl_long_sroll_label_with_wight_set_center(btn_popup_cancle, Label_popup_sure, 50, 0, "Cancel",50);

	// memcpy(file_print_send, srt, MKS_FILE_NAME_LENGTH);
	memset(file_print_send, 0, sizeof(file_print_send));
	memset(frame_ctrl.file_name, 0, sizeof(frame_ctrl.file_name));
	
	strcpy(file_print_send, srt);
	strcpy(frame_ctrl.file_name, srt);
	strcpy(file_name, srt);


	if(file_name[0] == '/') file_name[0] = ' ';
	// mks_lvgl_long_sroll_label_with_wight_set(caving_Popup, Label_popup_file_name, 100, 40, file_name, 255);
	// mks_lvgl_long_sroll_label_with_wight_set(caving_Popup, Label_popup, 100, 60, "Is Caving this File?",255);
	label_for_screen(caving_Popup, Label_popup_file_name, 0, -20, file_name);
	label_for_screen(caving_Popup, Label_popup, 0, 0, "Is Caving this File?");
	
}


void get_print_file_name(char *srt) { 

	memset(file_print_send, 0, sizeof(file_print_send));
	memset(frame_ctrl.file_name, 0, sizeof(frame_ctrl.file_name));
	
	strcpy(file_print_send, srt);
	strcpy(frame_ctrl.file_name, srt);
}


void mks_draw_file_loadig(void) {

	caving_read_file_src1 = lv_obj_create(mks_global.mks_src, NULL);

	lv_obj_set_size(caving_read_file_src1 ,350, 200);
	lv_obj_set_pos(caving_read_file_src1, 80,50);

	lv_style_copy(&popup_style, &lv_style_scr);
	popup_style.body.main_color = LV_COLOR_MAKE(0xCE, 0xD6, 0xE5); 
    popup_style.body.grad_color = LV_COLOR_MAKE(0xCE, 0xD6, 0xE5); 
	popup_style.text.color = LV_COLOR_BLACK;
	popup_style.body.radius = 17;
	lv_obj_set_style(caving_read_file_src1, &popup_style);
	mks_lvgl_long_sroll_label_with_wight_set(caving_read_file_src1, Label_popup, 110, 80, "File is Loading",240);
}

void mks_clear_craving(void) {
	
	lv_obj_clean(mks_global.mks_src);

}
