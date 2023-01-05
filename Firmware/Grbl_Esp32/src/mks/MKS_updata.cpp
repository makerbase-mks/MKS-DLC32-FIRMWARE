#include "MKS_updata.h"

MKS_UPDATA_T mks_updata;
UPDATA_PAGE_T updata_page;

void mks_updata_init(void) {
    bool is_exit_file = false;
    mks_updata.updata_flag = UD_NONE;       // 默认初始没有flag
    mks_updata.updata_persen = 0;           // 默认初始0%
    mks_updata.updata_cont = 0;             // 默认初始第0个 
    mks_updata.updata_line = 0;             // 默认初始第0行
    tf.init();

    mks_updata.updata_flag = UD_CHEAK;

    is_exit_file = tf.file_check(CFG_FILE_PATG);

    if(is_exit_file == true) {
        mks_updata.updata_flag = UD_HAD_FILE;   // exit cfg updata file
    }else {
        mks_updata.updata_flag = UD_NO_FILE;   // exit cfg updata file
    }
}

void mks_cfg_find(void) {
    String p;
    char send_data[128];

    p = tf.readFileLine(CFG_FILE_PATG, mks_updata.updata_line);
    if( (strstr(p.c_str() ,"-") == NULL) 
         && (strstr(p.c_str() ,"/*") == NULL) 
         && (strstr(p.c_str() ,"*-") == NULL) 
         && (strstr(p.c_str() ,"//") == NULL)
         && (p.length() < 127) 
        )  
    {   
        if( (strstr(p.c_str() ,"=") != NULL)) {
            mks_updata.is_have_data_ud = true;
        }else if(strstr(p.c_str() ,"[ESP110]") != NULL) {
            mks_updata.is_have_data_ud = true;
        }else if(strstr(p.c_str() ,"[ESP100]") != NULL) {
            mks_updata.is_have_data_ud = true;
        }else if(strstr(p.c_str() ,"[ESP101]") != NULL) {
            mks_updata.is_have_data_ud = true;
        }else if(strstr(p.c_str() ,"[ESP131]") != NULL) {
            mks_updata.is_have_data_ud = true;
        }else {
            mks_updata.is_have_data_ud = false;
        }
    }else{

    }

    if(mks_updata.is_have_data_ud == true) {
        mks_updata.updata_cont++;
        memset(send_data, 0, sizeof(send_data));
        strcpy(send_data, p.c_str());
        strcat(send_data, "\n");
        MKS_GRBL_CMD_SEND(send_data);
        mks_updata_data(); // 更新进度条
        mks_updata.is_have_data_ud = false;
    }

    mks_updata.updata_line++;

    if(mks_updata.updata_line > CFG_FILE_MAX_LINE) {   //搜索完毕，开始校验

        if(mks_updata.updata_cont >= CFG_FILE_NUM ) {
            
            mks_updata.updata_flag = UD_UPDATA_FINSH;
        }else {
            mks_updata.updata_flag = UD_UPDATA_FAIL;
        } 
    }
}

void mks_cfg_rename(const char* path1) {
    tf.renameFile(CFG_FILE_PATG, path1);
}

void mks_draw_updata(void) {
    updata_page.bar_updata = mks_lv_bar_set(mks_global.mks_src, updata_page.bar_updata, 440, 50, 20, 140, 0);
    updata_page.label_updata_title = label_for_screen(mks_global.mks_src, updata_page.label_updata_title, 0, -80, "Updata...");
    updata_page.label_updata_persen = label_for_screen(updata_page.label_updata_title, updata_page.label_updata_persen, 0, -100, "%0");
    mks_ui_page.mks_ui_page = MKS_UI_UPDATA;
    mks_updata.updata_flag = UD_UPDATA_ING;
}

void mks_updata_data(void) {
    uint8_t percent = 0;
    char str[16];
    if(mks_updata.updata_cont > CFG_FILE_NUM) mks_updata.updata_cont = CFG_FILE_NUM;
    percent = (mks_updata.updata_cont *100 / CFG_FILE_NUM);
    updata_page.bar_updata = mks_lv_bar_updata(updata_page.bar_updata, percent);
    sprintf(str, "%d%%", percent);
}