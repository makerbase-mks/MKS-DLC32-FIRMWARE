#ifndef __mks_test_h
#define __mks_test_h

#include "MKS_draw_lvgl.h"

#define TEST_PIN_LEVEL              0
#define TEST_PIN_PROBE_LEVEL        0

#define TEST_I2C_SCL_BIT            0x01
#define TEST_I2C_SDA_BIT            0X10

enum {
    TITLE_X_POS = 10,
    TITLE_Y_POS = 10,
    TITLE_X_OFFSET = 0,
    TITLE_Y_OFFSET = 30,
};

typedef struct {
    lv_obj_t *btn_back;
    lv_obj_t *label_back;
    lv_obj_t *label_title;
    lv_obj_t *label_probe;
    lv_obj_t *label_sd;
    lv_obj_t *label_x_limit;
    lv_obj_t *label_y_limit;
    lv_obj_t *label_z_limit;
    lv_obj_t *label_mcu_temp;
    lv_obj_t *label_version;
    lv_obj_t *label_i2c;
}MKS_PAGE_TEST_T;

typedef struct {
    bool is_exit_mode = false;

}MKS_TEST_CTRL_T;

void mks_draw_test_ui(void);
void pin_init(void);
void test_cfg_find_init(void);
void draw_testing(void);
#endif
