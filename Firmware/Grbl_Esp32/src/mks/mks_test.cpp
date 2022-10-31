#include "mks_test.h"

MKS_PAGE_TEST_T test_page;
MKS_TEST_CTRL_T test_ctrl;

char test_probe_str[30] = "Probe Check:";
char test_x_limit_str[30] = "X_limit Check:";
char test_y_limit_str[30] = "Y_limit Check:";
char test_z_limit_str[30] = "Z_limit Check:";
char test_I2C_str[30] = "I2C Check:";
char test_CPU_T_str[30] = "CPU_TEMP:";

char test_OK[30] = "OK";
char test_ERR[30] = "#FF0000 ERR#";
char test_WAR[30] = "#FF0000 Warning!#";


static void event_handler_test(lv_obj_t* obj, lv_event_t event) { 
	
	if (event == LV_EVENT_RELEASED) {
        mks_lv_clean_ui();
        mks_ui_page.mks_ui_page = MKS_UI_PAGE_LOADING;
        phy_init_reinit();
        MKS_GRBL_CMD_SEND("M5\n");
        lv_obj_set_style(mks_global.mks_src ,&mks_global.mks_src_style);
        mks_grbl.is_test_mode = false;
        mks_draw_ready();
	}
}

void mks_draw_test_ui(void) {

    char dis_str[60];
    mks_ui_page.mks_ui_page = MKS_UI_PAGE_LOADING;

    mks_global.mks_src = lv_obj_create(NULL, NULL);
    mks_global.mks_src = lv_scr_act();
    lv_obj_set_style(mks_global.mks_src ,&mks_global.mks_test_style);

    test_page.label_title = label_for_text(mks_global.mks_src, 
                                            test_page.label_title, 
                                            NULL, 
                                            TITLE_X_POS, 
                                            TITLE_X_POS, 
                                            LV_ALIGN_IN_TOP_LEFT, 
                                            "Testing...");

    memset(dis_str, 0, sizeof(dis_str));
    strcpy(dis_str, test_probe_str);
    strcat(dis_str, test_ERR);
    test_page.label_probe = label_for_text(mks_global.mks_src, 
                                            test_page.label_probe, 
                                            NULL, 
                                            TITLE_X_POS, 
                                            TITLE_Y_POS + TITLE_Y_OFFSET*1, 
                                            LV_ALIGN_IN_TOP_LEFT, 
                                            dis_str);

    
    test_page.label_sd = label_for_text(mks_global.mks_src, 
                                            test_page.label_sd, 
                                            NULL, 
                                            TITLE_X_POS, 
                                            TITLE_Y_POS + TITLE_Y_OFFSET*2, 
                                            LV_ALIGN_IN_TOP_LEFT, 
                                            "SD Check:OK");

    memset(dis_str, 0, sizeof(dis_str));
    strcpy(dis_str, test_x_limit_str);
    strcat(dis_str, test_ERR);
    test_page.label_x_limit = label_for_text(mks_global.mks_src, 
                                            test_page.label_x_limit, 
                                            NULL, 
                                            TITLE_X_POS, 
                                            TITLE_Y_POS + TITLE_Y_OFFSET*3, 
                                            LV_ALIGN_IN_TOP_LEFT, 
                                            dis_str);

    memset(dis_str, 0, sizeof(dis_str));
    strcpy(dis_str, test_y_limit_str);
    strcat(dis_str, test_ERR);
    test_page.label_y_limit = label_for_text(mks_global.mks_src, 
                                            test_page.label_y_limit, 
                                            NULL, 
                                            TITLE_X_POS, 
                                            TITLE_Y_POS + TITLE_Y_OFFSET*4, 
                                            LV_ALIGN_IN_TOP_LEFT, 
                                            dis_str);

    memset(dis_str, 0, sizeof(dis_str));
    strcpy(dis_str, test_z_limit_str);
    strcat(dis_str, test_ERR);
    test_page.label_z_limit = label_for_text(mks_global.mks_src, 
                                            test_page.label_z_limit, 
                                            NULL, 
                                            TITLE_X_POS, 
                                            TITLE_Y_POS + TITLE_Y_OFFSET*5, 
                                            LV_ALIGN_IN_TOP_LEFT, 
                                            dis_str);
    
    memset(dis_str, 0, sizeof(dis_str));
    strcpy(dis_str, test_CPU_T_str);
    strcat(dis_str, String(temperatureRead(), 1).c_str());
    test_page.label_mcu_temp = label_for_text(mks_global.mks_src, 
                                            test_page.label_mcu_temp, 
                                            NULL, 
                                            TITLE_X_POS, 
                                            TITLE_Y_POS + TITLE_Y_OFFSET*6, 
                                            LV_ALIGN_IN_TOP_LEFT, 
                                            dis_str);

    memset(dis_str, 0, sizeof(dis_str));
    strcpy(dis_str, test_I2C_str);
    strcat(dis_str, test_ERR);
    test_page.label_i2c = label_for_text(mks_global.mks_src, 
                                            test_page.label_i2c, 
                                            NULL, 
                                            TITLE_X_POS, 
                                            TITLE_Y_POS + TITLE_Y_OFFSET*7, 
                                            LV_ALIGN_IN_TOP_LEFT, 
                                            dis_str);

    test_page.label_version = label_for_text(mks_global.mks_src, 
                                            test_page.label_version, 
                                            NULL, 
                                            TITLE_X_POS, 
                                            TITLE_Y_POS + TITLE_Y_OFFSET*8, 
                                            LV_ALIGN_IN_TOP_LEFT, 
                                            FW_NAME);

    test_page.btn_back = mks_lv_btn_set(mks_global.mks_src, 
                                        test_page.btn_back, 
                                        100, 
                                        100, 
                                        370, 
                                        10, 
                                        event_handler_test);

    test_page.label_back = label_for_btn_name(test_page.btn_back, test_page.label_back, 0, 0, "Exit");
    mks_ui_page.mks_ui_page = MKS_UI_TEST;
}

void pin_init(void) {
    pinMode(X_LIMIT_PIN, INPUT_PULLUP);
    pinMode(Y_LIMIT_PIN, INPUT_PULLUP);
    pinMode(GPIO_NUM_34, INPUT);
    pinMode(PROBE_PIN, INPUT_PULLUP);
    pinMode(IIC_SCL, INPUT_PULLUP);
    pinMode(IIC_SDA, INPUT_PULLUP);
}

bool pin_check(uint8_t pin) {
    return digitalRead(pin);
}

uint8_t test_count = 0; 
uint8_t test_m_flag = 0;
uint8_t test_i2c_bit = 0x00;

void draw_testing(void) { 

    char dis_str[60];
    if(pin_check(PROBE_PIN) == TEST_PIN_PROBE_LEVEL) {
        memset(dis_str, 0, sizeof(dis_str));
        strcpy(dis_str, test_probe_str);
        strcat(dis_str, test_OK);  
        lv_label_set_text(test_page.label_probe, dis_str);
    }

    if(pin_check(X_LIMIT_PIN) == TEST_PIN_LEVEL) {
        memset(dis_str, 0, sizeof(dis_str));
        strcpy(dis_str, test_x_limit_str);
        strcat(dis_str, test_OK);  
        lv_label_set_text(test_page.label_x_limit, dis_str);
    }

    if(pin_check(Y_LIMIT_PIN) == TEST_PIN_LEVEL) {
        memset(dis_str, 0, sizeof(dis_str));
        strcpy(dis_str, test_y_limit_str);
        strcat(dis_str, test_OK);  
        lv_label_set_text(test_page.label_y_limit, dis_str);
    }
    if(pin_check(GPIO_NUM_34) == TEST_PIN_LEVEL) {
        memset(dis_str, 0, sizeof(dis_str));
        strcpy(dis_str, test_z_limit_str);
        strcat(dis_str, test_OK);  
        lv_label_set_text(test_page.label_z_limit, dis_str);
    }

    if(pin_check(IIC_SCL) == 1) {
        test_i2c_bit |= TEST_I2C_SCL_BIT;
    }

    if(pin_check(IIC_SDA) == 1) {
        test_i2c_bit |= TEST_I2C_SDA_BIT;
    }

    if(test_i2c_bit == 0x11) {
        memset(dis_str, 0, sizeof(dis_str));
        strcpy(dis_str, test_I2C_str);
        strcat(dis_str, test_OK);  
        lv_label_set_text(test_page.label_i2c, dis_str);
    }


    if(temperatureRead() > 100) {
        memset(dis_str, 0, sizeof(dis_str));
        strcpy(dis_str, test_CPU_T_str);
        strcat(dis_str, test_WAR);  
        lv_label_set_text(test_page.label_mcu_temp, dis_str);
    }else{
        memset(dis_str, 0, sizeof(dis_str));
        strcpy(dis_str, test_CPU_T_str);
        strcat(dis_str, String(temperatureRead(), 1).c_str());  
        lv_label_set_text(test_page.label_mcu_temp, dis_str);
    }

    // test_count++;
    // if(test_count == 10 && stepper_idle) {
     if(stepper_idle) {
        // test_count = 0;
        if(test_m_flag == 0) {
            MKS_GRBL_CMD_SEND("M3 S900\n");
            MKS_GRBL_CMD_SEND("G1 X20 Y20 Z20 F4000\n");
            test_m_flag = 1;
        }else {
            MKS_GRBL_CMD_SEND("M5\n");
            MKS_GRBL_CMD_SEND("G1 X0 Y0 Z0 F4000\n");
            test_m_flag = 0;
        }
    }
}

void test_cfg_find_init(void) {

    bool is_exit_file = false;

    tf.init();

    is_exit_file = tf.file_check(CFG_FILE_PATG); 
    
    if(is_exit_file == true) {
        String p;
        p = tf.readFileLine(CFG_FILE_PATG, 2);

        if(strstr(p.c_str(), "DLC_TEST=1")) {

            mks_grbl.is_test_mode = true;

            mks_cfg_rename(CFG_FILE_PATG2);
        }
        else {
            mks_grbl.is_test_mode = false;
        }
    }else {

    }
}






