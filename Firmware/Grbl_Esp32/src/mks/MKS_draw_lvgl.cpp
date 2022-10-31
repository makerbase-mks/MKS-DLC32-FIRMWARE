#include "MKS_draw_lvgl.h"


GLOBAL_OBJ_T mks_global;

COMMON_POPUP_T com_p1;
COMMON_POPUP_T com_p2;
COMMON_POPUP_T com_p_info;
COMMON_POPUP_T com_p_info_com;
COMMON_POPUP_T cavre_pupop;

LV_IMG_DECLARE(add);  // 加
LV_IMG_DECLARE(confirm);  // 确认
LV_IMG_DECLARE(s_return);  // 确认
LV_IMG_DECLARE(reduce);  // 减

/* 
 * Author   :MKS
 * Describe :Set gradient background (support LVGL V6, can not use v7)
 * Data     :2021/01/30
*/
lv_obj_t* mks_lvgl_draw_bk(lv_obj_t * scr, lv_style_t * dest_style, const lv_style_t * src_style, lv_color_t gcolor, lv_color_t mcolor) {
    lv_style_copy(dest_style, src_style);
	dest_style->body.grad_color = gcolor;
	dest_style->body.main_color = mcolor;
	lv_obj_set_style(scr ,dest_style);
    return scr;
}

/* 
 * Author   :MKS
 * Describe :Set monochrome background
 * Data     :2021/01/30
*/
lv_obj_t* mks_lvgl_draw_bk_both(lv_obj_t * scr, lv_style_t * dest_style, const lv_style_t * src_style, lv_color_t color) {
    lv_style_copy(dest_style, src_style);
	dest_style->body.grad_color = color;
	dest_style->body.main_color = color;
	lv_obj_set_style(scr ,dest_style);
    return scr;
}

/* 
 * Author   :MKS
 * Describe :Set wrap label
 * Data     :2021/01/30
*/
lv_obj_t* mks_lvgl_label_set(lv_obj_t *scr, lv_obj_t *lab, lv_coord_t x, lv_coord_t y, const char *text) {
    lab = lv_label_create(scr, NULL);                                                          
    lv_label_set_long_mode(lab, LV_LABEL_LONG_BREAK);                                    
    lv_obj_set_width(lab, 80);
    lv_obj_set_height(lab,20);
    lv_obj_set_pos(lab, x, y);
    lv_label_set_recolor(lab, true);                                                  
    lv_label_set_text(lab, text);
    return lab;
}


/* 
 * Author   :MKS
 * Describe :Set the label to scroll back and forth
 * Data     :2021/01/30
*/
lv_obj_t* mks_lvgl_long_sroll_label_with_wight_set(lv_obj_t* scr, lv_obj_t* lab, lv_coord_t x, lv_coord_t y, const char* text, lv_coord_t w) {
    lab = lv_label_create(scr, NULL);
    lv_label_set_long_mode(lab, LV_LABEL_LONG_BREAK);
    lv_obj_set_width(lab, w);
    lv_obj_set_height(lab, 20);
    lv_obj_set_pos(lab, x, y);
    lv_label_set_recolor(lab, true);
    lv_label_set_text(lab, text);
    return lab;
}

/* 
 * Author   :MKS
 * Describe :Set the label to scroll back and forth
 * Data     :2021/01/30
*/
lv_obj_t* mks_lvgl_long_sroll_label_with_wight_set_center(lv_obj_t* scr, lv_obj_t* lab, lv_coord_t x, lv_coord_t y, const char* text, lv_coord_t w) {
    lab = lv_label_create(scr, NULL);
    lv_label_set_long_mode(lab, LV_LABEL_LONG_EXPAND);
    // lv_obj_set_width(lab, w);
    lv_label_set_align(lab, LV_ALIGN_CENTER);
    // lv_obj_set_height(lab, 20);
    lv_obj_set_pos(lab, x, y);
    lv_label_set_recolor(lab, true);
    lv_label_set_text(lab, text);
    return lab;
}

/*
 * 用于显示文件名
*/
lv_obj_t* label_for_file(lv_obj_t* scr, lv_obj_t* lab, lv_coord_t x, lv_coord_t y, const char* text, lv_coord_t w) {

    lab = lv_label_create(scr, NULL);
    lv_label_set_long_mode(lab, LV_LABEL_LONG_SROLL_CIRC);
    lv_obj_set_width(lab, w);
    lv_label_set_recolor(lab, true);
    lv_label_set_text(lab, text);
    lv_label_set_align(lab ,LV_LABEL_ALIGN_CENTER);
    lv_obj_align(lab, NULL, LV_ALIGN_CENTER, x, y);
    return lab;
}

/*
 * 用于按键里面的文本
*/
lv_obj_t* label_for_btn_name(lv_obj_t* scr, lv_obj_t* lab,lv_coord_t x, lv_coord_t y, const char* text) {
    lab = lv_label_create(scr, NULL);
    lv_label_set_long_mode(lab, LV_LABEL_LONG_EXPAND);
    lv_label_set_recolor(lab, true);
    lv_label_set_text(lab, text);
    lv_label_set_align(lab ,LV_LABEL_ALIGN_CENTER);
    lv_obj_align(lab, NULL, LV_ALIGN_CENTER, x, y);
    return lab;
}

/*
 * 用于图片按键的文本
*/
lv_obj_t* label_for_imgbtn_name(lv_obj_t* scr, lv_obj_t* lab, lv_obj_t* base, lv_coord_t x, lv_coord_t y, const char* text) {
    lab = lv_label_create(scr, NULL);
    lv_label_set_long_mode(lab, LV_LABEL_LONG_EXPAND);
    lv_label_set_recolor(lab, true);
    lv_label_set_text(lab, text);
    // lv_label_set_align(lab ,LV_LABEL_ALIGN_CENTER);
    lv_obj_align(lab, base, LV_ALIGN_OUT_BOTTOM_MID, x, y);
    return lab;
}

/*
 * 用于图片按键的文本
*/
lv_obj_t* label_for_imgbtn_name_mid(lv_obj_t* scr, lv_obj_t* lab, lv_obj_t* base, lv_coord_t x, lv_coord_t y, const char* text) {
    lab = lv_label_create(scr, NULL);
    lv_label_set_long_mode(lab, LV_LABEL_LONG_EXPAND);
    lv_label_set_recolor(lab, true);
    lv_label_set_text(lab, text);
    // lv_label_set_align(lab ,LV_LABEL_ALIGN_CENTER);
    lv_obj_align(lab, base, LV_ALIGN_IN_RIGHT_MID, x, y);
    return lab;
}

/*
 * 用于提示框显示
*/
lv_obj_t* label_for_screen(lv_obj_t* scr, lv_obj_t* lab, lv_coord_t x, lv_coord_t y, const char* text) {

    lab = lv_label_create(scr, NULL);
    lv_label_set_long_mode(lab, LV_LABEL_LONG_EXPAND);
    lv_label_set_recolor(lab, true);
    lv_label_set_text(lab, text);
	lv_label_set_align(lab, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(lab, scr, LV_ALIGN_CENTER, x, y);
    return lab;
}

/*
 * 用于提示框显示
*/
lv_obj_t* label_for_infile_name(lv_obj_t* scr, lv_obj_t* lab, lv_coord_t x, lv_coord_t y, const char* text) {

    lab = lv_label_create(scr, NULL);
    lv_label_set_long_mode(lab, LV_LABEL_LONG_SROLL_CIRC);
    lv_label_set_recolor(lab, true);
    lv_label_set_text(lab, text);
	lv_label_set_align(lab, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(lab, scr, LV_ALIGN_CENTER, x, y);
    lv_obj_set_size(lab, 100, 20);
    return lab;
}

/*
 * 用于信息显示
 * 提供对齐选项
*/
lv_obj_t* label_for_text(lv_obj_t* scr, lv_obj_t* lab, lv_obj_t * base, lv_coord_t x, lv_coord_t y, lv_align_t align,  const char* text) {
    lab = lv_label_create(scr, NULL);
    lv_label_set_long_mode(lab, LV_LABEL_LONG_EXPAND);
    lv_label_set_recolor(lab, true);
    lv_label_set_text(lab, text);
	lv_label_set_align(lab, LV_LABEL_ALIGN_CENTER);
    lv_obj_align(lab, base, align, x, y);
    return lab;
}


lv_obj_t* mks_lv_static_label(lv_obj_t* scr, lv_obj_t* lab, lv_coord_t x, lv_coord_t y, const char* text, lv_coord_t w) {

    lab = lv_label_create(scr, NULL);
    lv_label_set_long_mode(lab, LV_LABEL_LONG_BREAK);
    lv_obj_set_width(lab, w);
    lv_obj_set_height(lab, 20);
    lv_obj_set_pos(lab, x, y);
    lv_label_set_recolor(lab, true);
    // lv_label_set_text(lab, text);
    lv_label_set_static_text(lab, text);
    return lab;
}

/* 
 * Author   :MKS
 * Describe :Set up Img display
 * Data     :2021/01/30
*/
lv_obj_t* mks_lvgl_img_set(lv_obj_t *scr ,lv_obj_t *img, const void * src_img, lv_coord_t x_mod, lv_coord_t y_mod) {
    img = lv_img_create(scr, NULL);
    lv_img_set_src(img, src_img);
    return img;
}

lv_obj_t* mks_lvgl_img_set_algin(lv_obj_t *scr ,lv_obj_t *img, const void * src_img, lv_align_t align, lv_coord_t x_mod, lv_coord_t y_mod) {
    img = lv_img_create(scr, NULL);
    lv_img_set_src(img, src_img);
    lv_obj_align(img, scr, align, x_mod, y_mod);
    return img;
}

/* 
 * Author   :MKS
 * Describe :Create Img button
 * Data     :2021/01/30
*/
lv_obj_t* lv_imgbtn_creat_mks(lv_obj_t *scr ,lv_obj_t *imgbtn, const void * img_pr, const void * img_rel, lv_align_t align, lv_coord_t x_mod, lv_coord_t y_mod, lv_event_cb_t event_cb) {
    imgbtn = lv_imgbtn_create(scr, NULL);
    lv_imgbtn_set_src(imgbtn, LV_BTN_STATE_PR, img_pr);
    lv_imgbtn_set_src(imgbtn, LV_BTN_STATE_REL, img_rel);
    lv_imgbtn_set_state(imgbtn, LV_BTN_STATE_REL);
    lv_obj_align(imgbtn, scr, align, x_mod, y_mod);
    lv_obj_set_event_cb(imgbtn, event_cb);
    return imgbtn;
}

lv_obj_t* lv_imgbtn_creat_mks1(lv_obj_t *scr ,lv_obj_t *imgbtn, const void * img_pr, const void * img_rel, lv_align_t align, lv_coord_t x_mod, lv_coord_t y_mod, lv_event_cb_t event_cb) {
    imgbtn = lv_imgbtn_create(scr, NULL);
    lv_imgbtn_set_src(imgbtn, LV_BTN_STATE_PR, img_pr);
    lv_imgbtn_set_src(imgbtn, LV_BTN_STATE_REL, img_rel);
    lv_imgbtn_set_state(imgbtn, LV_BTN_STATE_REL);
    lv_obj_set_size(imgbtn,100, 95);
    lv_obj_align(imgbtn, NULL, align,x_mod, y_mod);
    lv_obj_set_event_cb(imgbtn, event_cb);
    return imgbtn;
}

lv_obj_t* lv_imgbtn_creat_n_mks(lv_obj_t *scr ,lv_obj_t *imgbtn, const void * img_pr, const void * img_rel, lv_coord_t x_mod, lv_coord_t y_mod, lv_event_cb_t event_cb) {
    imgbtn = lv_imgbtn_create(scr, NULL);
    lv_imgbtn_set_src(imgbtn, LV_BTN_STATE_PR, img_pr);
    lv_imgbtn_set_src(imgbtn, LV_BTN_STATE_REL, img_rel);
    lv_imgbtn_set_state(imgbtn, LV_BTN_STATE_REL);
    lv_obj_set_pos(imgbtn, x_mod, y_mod);
    lv_obj_set_event_cb(imgbtn, event_cb);
    return imgbtn;
}

/* 
 * Author   :MKS
 * Describe :Create Img button
 * Data     :2021/03/16
*/
lv_obj_t* lv_imgbtn_img_set(lv_obj_t *imgbtn, const void * img_pr, const void * img_rel) {

    lv_imgbtn_set_src(imgbtn, LV_BTN_STATE_PR, img_pr);
    lv_imgbtn_set_src(imgbtn, LV_BTN_STATE_REL, img_rel);
    return imgbtn;
}


/* 
 * Author   :MKS
 * Describe :Create Img button
 * Data     :2021/02/23
*/
lv_obj_t* mks_lv_btn_set(lv_obj_t* scr, lv_obj_t* btn, lv_coord_t btn_w, lv_coord_t btn_h, lv_coord_t x, lv_coord_t y, lv_event_cb_t event_cb) {

    btn = lv_btn_create(scr, NULL);
    lv_obj_set_size(btn, btn_w, btn_h);
    lv_obj_set_pos(btn, x, y);
    lv_obj_set_event_cb(btn, event_cb);
    return btn;
}

lv_obj_t* mks_lv_btn_set_for_screen(lv_obj_t* scr, lv_obj_t* btn, lv_coord_t btn_w, lv_coord_t btn_h, lv_coord_t x, lv_coord_t y, lv_event_cb_t event_cb) {

    btn = lv_btn_create(scr, NULL);
    lv_obj_set_size(btn, btn_w, btn_h);
    lv_obj_align(btn, NULL, LV_ALIGN_CENTER, x, y);
    lv_obj_set_event_cb(btn, event_cb);
    return btn;
}

lv_obj_t* mks_lv_btn_set_for_aglin_screen(lv_obj_t* scr, lv_obj_t* btn, lv_coord_t btn_w, lv_coord_t btn_h, lv_align_t align, lv_coord_t x, lv_coord_t y, lv_event_cb_t event_cb) {

    btn = lv_btn_create(scr, NULL);
    lv_obj_set_size(btn, btn_w, btn_h);
    lv_obj_align(btn, NULL, align, x, y);
    lv_obj_set_event_cb(btn, event_cb);
    return btn;
}

/* 
 * Author   :MKS
 * Describe :Create Bar
 * Data     :2021/02/23
*/
lv_obj_t* mks_lv_bar_set(lv_obj_t* scr, lv_obj_t* bar, lv_coord_t bar_w, lv_coord_t bar_h, lv_coord_t x, lv_coord_t y, uint8_t val) {

    bar = lv_bar_create(scr, NULL);
    lv_obj_set_size(bar, bar_w, bar_h);
    // lv_obj_align(bar, NULL, LV_ALIGN_CENTER, x, y);
    lv_obj_set_pos(bar, x, y);
    //lv_bar_set_anim_time(bar, 1000);
    lv_bar_set_value(bar, val, LV_ANIM_ON);
    return bar;
}

/* 
 * Author   :MKS
 * Describe :Updata bar value
 * Data     :2021/03/11
*/
lv_obj_t* mks_lv_bar_updata(lv_obj_t* bar, int16_t value) { 
    lv_bar_set_value(bar, value, LV_ANIM_ON);
    return bar;
}

lv_obj_t* mks_lv_label_updata(lv_obj_t* lab, const char *str) {
    lv_label_set_static_text(lab ,str);
    return lab;
}

/* 
 * Author   :MKS
 * Describe :Updata bar value
 * Data     :2021/03/17
*/
lv_obj_t* mks_lv_set_kb(lv_obj_t* scr, lv_obj_t *kb, lv_event_cb_t event_cb) { 

    static lv_style_t rel_style, pr_style;

    lv_style_copy(&rel_style, &lv_style_btn_rel);
    rel_style.body.radius = 0;
    rel_style.body.border.width = 1;

    lv_style_copy(&pr_style, &lv_style_btn_pr);
    pr_style.body.radius = 0;
    pr_style.body.border.width = 1;

    kb = lv_kb_create(scr, NULL);
    lv_kb_set_cursor_manage(kb, true);
    lv_kb_set_style(kb, LV_KB_STYLE_BG, &lv_style_transp_tight);
    lv_kb_set_style(kb, LV_KB_STYLE_BTN_REL, &rel_style);
    lv_kb_set_style(kb, LV_KB_STYLE_BTN_PR, &pr_style);
    lv_obj_set_event_cb(kb, event_cb);

    return kb;
}

lv_obj_t* mks_lv_set_ta(lv_obj_t* scr, lv_obj_t *ta, lv_obj_t *kb, lv_coord_t w, lv_coord_t h, lv_coord_t x, lv_coord_t y) { 

    ta = lv_ta_create(scr, NULL); 
    lv_obj_set_pos(ta, x, y);
    lv_obj_set_size(ta, w, h);
    lv_ta_set_text(ta, "");
    lv_kb_set_ta(kb, ta);
    return ta;
}

lv_obj_t* mks_lv_set_line(lv_obj_t* scr, lv_obj_t * line, lv_point_t *line_points) { 

    /*Copy the previous line and apply the new style*/
    line = lv_line_create(scr, NULL);
    lv_line_set_points(line, line_points, 2);     /*Set the points*/
    
    // lv_obj_align(line, NULL, LV_ALIGN_CENTER, 0, 0);

    return line;
}

void mks_lv_clean_ui(void) { 
    mks_grbl.popup_1_flag = false;
    lv_obj_clean(mks_global.mks_src);
}

static void event_handler_globel_popup_sure(lv_obj_t* obj, lv_event_t event) { 

    if (event == LV_EVENT_RELEASED) { 
        lv_obj_del(com_p2.com_popup_src);

        if(mks_ui_page.mks_ui_page == MKS_UI_Frame) {
            frame_ctrl.out = false;
            mks_lv_clean_ui();
            mks_ui_page.mks_ui_page = MKS_UI_PAGE_LOADING;
            mks_draw_ready();
        }
        mks_grbl.popup_1_flag = false;
        frame_ctrl.is_begin_run = false;
    }
}

void draw_global_popup(const char *text) {

    if(mks_grbl.popup_1_flag == true) return;
    mks_grbl.popup_1_flag = true;

    com_p2.com_popup_src = lv_obj_create(mks_global.mks_src, NULL);

	lv_obj_set_size(com_p2.com_popup_src, move_popup_size_x, move_popup_size_y);
    lv_obj_set_pos(com_p2.com_popup_src, move_popup_x, move_popup_y);

	lv_style_copy(&com_p2.com_popup_sytle, &lv_style_scr);
	com_p2.com_popup_sytle.body.main_color = LV_COLOR_MAKE(0xCE, 0xD6, 0xE5); 
    com_p2.com_popup_sytle.body.grad_color = LV_COLOR_MAKE(0xCE, 0xD6, 0xE5); 
    com_p2.com_popup_sytle.text.color = LV_COLOR_BLACK;
    com_p2.com_popup_sytle.body.radius = 17;
	lv_obj_set_style(com_p2.com_popup_src, &com_p2.com_popup_sytle);

	lv_style_copy(&com_p2.com_btn_sytle, &lv_style_scr);
	com_p2.com_btn_sytle.body.main_color = LV_COLOR_MAKE(0x3F, 0x46, 0x66);
    com_p2.com_btn_sytle.body.grad_color = LV_COLOR_MAKE(0x3F, 0x46, 0x66);
    com_p2.com_btn_sytle.body.opa = LV_OPA_COVER;//设置背景色完全不透明
    com_p2.com_btn_sytle.text.color = LV_COLOR_WHITE;
	com_p2.com_btn_sytle.body.radius = 10;

	com_p2.btn_yes = mks_lv_btn_set_for_screen(com_p2.com_popup_src, 
                                                com_p2.btn_yes, 
                                                move_popup_btn_size_x, 
                                                move_popup_btn_size_y,
                                                0, 
                                                60, 
                                                event_handler_globel_popup_sure);
	lv_btn_set_style(com_p2.btn_yes, LV_BTN_STYLE_REL, &com_p2.com_btn_sytle);
	lv_btn_set_style(com_p2.btn_yes,LV_BTN_STYLE_PR, &com_p2.com_btn_sytle);

	com_p2.label_line1 = label_for_screen(com_p2.com_popup_src, com_p2.label_line1, 0, -30, text);
	com_p2.label_yes = label_for_btn_name(com_p2.btn_yes, com_p2.label_yes, 0, 0, "Yes");
}

void mks_draw_common_popup(char *title, char *line1, char *line2, lv_event_cb_t event_cb_yes, lv_event_cb_t event_cancle) {

    if(com_p1.mux == true) return;
    com_p1.com_popup_src = lv_obj_create(mks_global.mks_src, NULL);

	lv_obj_set_size(com_p1.com_popup_src ,350, 200);
	lv_obj_set_pos(com_p1.com_popup_src, 80,50);

	lv_style_copy(&com_p1.com_popup_sytle, &lv_style_scr);
	com_p1.com_popup_sytle.body.main_color = LV_COLOR_MAKE(0xCE, 0xD6, 0xE5); 
    com_p1.com_popup_sytle.body.grad_color = LV_COLOR_MAKE(0xCE, 0xD6, 0xE5); 
	com_p1.com_popup_sytle.text.color = LV_COLOR_BLACK;
	com_p1.com_popup_sytle.body.radius = 17;
	lv_obj_set_style(com_p1.com_popup_src, &com_p1.com_popup_sytle);
	
	lv_style_copy(&com_p1.com_btn_sytle, &lv_style_scr);
    com_p1.com_btn_sytle.body.main_color = LV_COLOR_MAKE(0x3F, 0x46, 0x66);
    com_p1.com_btn_sytle.body.grad_color = LV_COLOR_MAKE(0x3F, 0x46, 0x66);
	com_p1.com_btn_sytle.body.radius = 10;
    com_p1.com_btn_sytle.body.opa = LV_OPA_COVER; // 设置背景色完全不透明
    com_p1.com_btn_sytle.text.color = LV_COLOR_WHITE;
	
	com_p1.btn_yes = mks_lv_btn_set(com_p1.com_popup_src, com_p1.btn_yes, 100,40,10,130, event_cb_yes);
	lv_btn_set_style(com_p1.btn_yes, LV_BTN_STYLE_REL, &com_p1.com_btn_sytle);
    lv_btn_set_style(com_p1.btn_yes,LV_BTN_STYLE_PR,&com_p1.com_btn_sytle);
	// mks_lvgl_long_sroll_label_with_wight_set_center(com_p1.btn_yes, com_p1.label_yes, 50, 0, "Yes",50);
    label_for_btn_name(com_p1.btn_yes, com_p1.label_yes, 50, 0, "Yes");

	com_p1.btn_cancle = mks_lv_btn_set(com_p1.com_popup_src, com_p1.btn_cancle, 100,40,240,130, event_cancle);
	lv_btn_set_style(com_p1.btn_cancle, LV_BTN_STYLE_REL, &com_p1.com_btn_sytle);
    lv_btn_set_style(com_p1.btn_cancle,LV_BTN_STYLE_PR, &com_p1.com_btn_sytle);
	// mks_lvgl_long_sroll_label_with_wight_set_center(com_p1.btn_cancle, com_p1.label_cancle, 50, 0, "Cancel",50);
    label_for_btn_name(com_p1.btn_cancle, com_p1.label_cancle, 50, 0, "Cancel");

    label_for_screen(com_p1.com_popup_src, com_p1.label_title, 0, -60, title);
	label_for_screen(com_p1.com_popup_src, com_p1.label_line1, 0, -20, line1);
    label_for_screen(com_p1.com_popup_src, com_p1.label_line2, 0, 0, line2);

    com_p1.mux = true;
}

void mks_draw_common_pupup_info(char *title,char *line1, char *line2) {
    
    if(com_p_info.mux == true) return;
    com_p_info.com_popup_src = lv_obj_create(mks_global.mks_src, NULL);

	lv_obj_set_size(com_p_info.com_popup_src ,350, 200);
	lv_obj_set_pos(com_p_info.com_popup_src, 80,50);

    lv_style_copy(&com_p_info.com_popup_sytle, &lv_style_scr);
	com_p_info.com_popup_sytle.body.main_color = LV_COLOR_MAKE(0xCE, 0xD6, 0xE5); 
    com_p_info.com_popup_sytle.body.grad_color = LV_COLOR_MAKE(0xCE, 0xD6, 0xE5); 
	com_p_info.com_popup_sytle.text.color = LV_COLOR_BLACK;
	com_p_info.com_popup_sytle.body.radius = 17;
	lv_obj_set_style(com_p_info.com_popup_src, &com_p_info.com_popup_sytle);

    label_for_screen(com_p_info.com_popup_src, com_p_info.label_title, 0, -60, title);
	label_for_screen(com_p_info.com_popup_src, com_p_info.label_line1, 0, -20, line1);
    label_for_screen(com_p_info.com_popup_src, com_p_info.label_line2, 0, 0, line2);
    com_p_info.mux = true;
}

void mks_draw_common_popup_info_com(char *title, char *line1, char *line2, lv_event_cb_t event_cb_yes) {

    if(com_p_info_com.mux == true) return;
    com_p_info_com.com_popup_src = lv_obj_create(mks_global.mks_src, NULL);

	lv_obj_set_size(com_p_info_com.com_popup_src ,350, 200);
	lv_obj_set_pos(com_p_info_com.com_popup_src, 80,50);

	lv_style_copy(&com_p_info_com.com_popup_sytle, &lv_style_scr);
	com_p_info_com.com_popup_sytle.body.main_color = LV_COLOR_MAKE(0xCE, 0xD6, 0xE5); 
    com_p_info_com.com_popup_sytle.body.grad_color = LV_COLOR_MAKE(0xCE, 0xD6, 0xE5); 
	com_p_info_com.com_popup_sytle.text.color = LV_COLOR_BLACK;
	com_p_info_com.com_popup_sytle.body.radius = 17;
	lv_obj_set_style(com_p_info_com.com_popup_src, &com_p_info_com.com_popup_sytle);
	
	lv_style_copy(&com_p_info_com.com_btn_sytle, &lv_style_scr);
    com_p_info_com.com_btn_sytle.body.main_color = LV_COLOR_MAKE(0x3F, 0x46, 0x66);
    com_p_info_com.com_btn_sytle.body.grad_color = LV_COLOR_MAKE(0x3F, 0x46, 0x66);
	com_p_info_com.com_btn_sytle.body.radius = 10;
    com_p_info_com.com_btn_sytle.body.opa = LV_OPA_COVER; // 设置背景色完全不透明
    com_p_info_com.com_btn_sytle.text.color = LV_COLOR_WHITE;
	
    com_p_info_com.btn_yes = mks_lv_btn_set_for_screen(com_p_info_com.com_popup_src, com_p_info_com.btn_yes, 100, 40, 0, 60, event_cb_yes);
    
	lv_btn_set_style(com_p_info_com.btn_yes, LV_BTN_STYLE_REL, &com_p_info_com.com_btn_sytle);
    lv_btn_set_style(com_p_info_com.btn_yes,LV_BTN_STYLE_PR,&com_p_info_com.com_btn_sytle);
	// mks_lvgl_long_sroll_label_with_wight_set_center(com_p_info_com.btn_yes, com_p_info_com.label_yes, 50, 0, "Yes",50);
    label_for_btn_name(com_p_info_com.btn_yes, com_p_info_com.label_yes, 50, 0, "Yes");

    label_for_screen(com_p_info_com.com_popup_src, com_p_info_com.label_title, 0, -60, title);
	label_for_screen(com_p_info_com.com_popup_src, com_p_info_com.label_line1, 0, -20, line1);
    label_for_screen(com_p_info_com.com_popup_src, com_p_info_com.label_line2, 0, 0, line2);
    com_p_info_com.mux = true;
}


static void event_handler_cave_add(lv_obj_t* obj, lv_event_t event) {

	if (event == LV_EVENT_RELEASED) {
		char buff[20];
		mks_grbl.carve_times ++;
		if(mks_grbl.carve_times > 99) {
			mks_grbl.carve_times = 99;
		}

		sprintf(buff , "times:%d", mks_grbl.carve_times);
		lv_label_set_text(cavre_pupop.label_line2, buff);
	}
}

static void event_handler_cave_dec(lv_obj_t* obj, lv_event_t event) {

	if (event == LV_EVENT_RELEASED) {
		char buff[20];
		mks_grbl.carve_times --;
		if(mks_grbl.carve_times < 1) {
			mks_grbl.carve_times = 1;
		}
		sprintf(buff , "times:%d", mks_grbl.carve_times);
		lv_label_set_text(cavre_pupop.label_line2, buff);
	}
}

void mks_draw_cavre_popup(char *fn, lv_event_cb_t event_cb_yes, lv_event_cb_t event_cb_no) {
	
	char buff[20];
	mks_grbl.carve_times = 1;

    if(cavre_pupop.mux == true) return;
    cavre_pupop.com_popup_src = lv_obj_create(mks_global.mks_src, NULL);

	lv_obj_set_size(cavre_pupop.com_popup_src ,350, 200);
	lv_obj_set_pos(cavre_pupop.com_popup_src, 80,50);

    lv_style_copy(&cavre_pupop.com_popup_sytle, &lv_style_scr);
	cavre_pupop.com_popup_sytle.body.main_color = LV_COLOR_MAKE(0xCE, 0xD6, 0xE5); 
    cavre_pupop.com_popup_sytle.body.grad_color = LV_COLOR_MAKE(0xCE, 0xD6, 0xE5); 
	cavre_pupop.com_popup_sytle.text.color = LV_COLOR_BLACK;
	cavre_pupop.com_popup_sytle.body.radius = 17;
	lv_obj_set_style(cavre_pupop.com_popup_src, &cavre_pupop.com_popup_sytle);

	cavre_pupop.btn_add = 
	lv_imgbtn_creat_mks(cavre_pupop.com_popup_src, 
						cavre_pupop.btn_add, 
						&add, 
						&add, 
						LV_ALIGN_IN_LEFT_MID, 
						print_pwr_popup_add_btn_x,
						print_pwr_popup_add_btn_y, 
						event_handler_cave_add);

    cavre_pupop.btn_dec = 
	lv_imgbtn_creat_mks(cavre_pupop.com_popup_src, 
						cavre_pupop.btn_dec, 
						&reduce, &reduce, 
						LV_ALIGN_IN_LEFT_MID, 
						print_pwr_popup_add_btn_x+80,
						print_pwr_popup_add_btn_y, 
						event_handler_cave_dec);

    cavre_pupop.btn_yes = 
	lv_imgbtn_creat_mks(cavre_pupop.com_popup_src, 
                        cavre_pupop.btn_yes, 
                        &confirm, 
                        &confirm, 
                        LV_ALIGN_IN_LEFT_MID, 
                        print_pwr_popup_add_btn_x+160,
                        print_pwr_popup_add_btn_y, event_cb_yes);
    
    cavre_pupop.btn_cancle = 
	lv_imgbtn_creat_mks(cavre_pupop.com_popup_src, 
                        cavre_pupop.btn_cancle, 
                        &s_return, 
                        &s_return, 
                        LV_ALIGN_IN_LEFT_MID, 
                        print_pwr_popup_add_btn_x+240,
                        print_pwr_popup_add_btn_y, event_cb_no);
	
	label_for_screen(cavre_pupop.com_popup_src, 
					 cavre_pupop.label_line1,
					 0,
					 -20,
					 fn);

	sprintf(buff, "times:%d", mks_grbl.carve_times);
	cavre_pupop.label_line2 = label_for_screen(cavre_pupop.com_popup_src, 
											cavre_pupop.label_line2,
											0,
											0,
											buff);
    cavre_pupop.mux = true;
}

void common_pupup_info_del(void) { 
    lv_obj_del(com_p_info.com_popup_src);
    com_p_info.mux = false;
}

void common_popup_del(void) { 
    lv_obj_del(com_p2.com_popup_src);
    com_p2.mux = false;
}

void common_popup_com_del(void) { 
    lv_obj_del(com_p_info_com.com_popup_src);
    com_p_info_com.mux = false;
}

void global_popup_del(void) { 
    lv_obj_del(com_p1.com_popup_src);
    com_p1.mux = false;
}

void cavre_popup_del(void) { 
    lv_obj_del(cavre_pupop.com_popup_src);
    cavre_pupop.mux = false;
}

uint8_t get_current_page(void) {
    return mks_ui_page.mks_ui_page;
}

void disable_btn(void) { 
    uint8_t current_page;
    current_page = get_current_page();
    switch(current_page) {

        case MKS_UI_Logo:

        break;

        case MKS_UI_Ready:

        break;

        case MKS_UI_Adjust:

        break;

        case MKS_UI_Control:

        break;

        case MKS_UI_Caving:

        break;

        case MKS_UI_Pring:

        break;

        case MKS_UI_Tool:

        break;

        case MKS_UI_Wifi:

        break;

        case MKS_UI_Frame:

        break;

        case MKS_UI_inFile:

        break;

        case MKS_UI_UPDATA:

        break;

        default:   // Loading

        break;
    }
}








