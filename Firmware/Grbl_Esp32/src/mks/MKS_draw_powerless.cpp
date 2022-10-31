#include "MKS_draw_powerless.h"
#include "../SDCard.h"

MKS_PL_T powerless;

void mks_powerless_init(void) {

    powerless.file_line = 0;
    powerless.is_pl_flag = false;
    memset(powerless.pl_file_name, 0, sizeof(powerless.pl_file_name));
}

void mks_powerless_check(void) {
    powerless.is_pl_flag = sd_file_check(PL_FILE_PATG);

    if(powerless.is_pl_flag == true) {
        grbl_send(CLIENT_SERIAL,"PLA file had found\n");
        
        
    }
}


void sd_read_line_num_change() {

    if(powerless.is_pl_flag == true) {

        sd_current_line_number = powerless.file_line;

    }else {


    }
}




