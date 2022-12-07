#ifndef __draw_ui_h
#define __draw_ui_h

#include "MKS_draw_lvgl.h"


LV_FONT_DECLARE(dlc32Font);	


LV_IMG_DECLARE(mks_logo);

LV_IMG_DECLARE(back);
LV_IMG_DECLARE(png_back);
LV_IMG_DECLARE(png_back_pre);	

LV_IMG_DECLARE(Control);  
LV_IMG_DECLARE(Sculpture);	
LV_IMG_DECLARE(Tool);
LV_IMG_DECLARE(png_ctrl_pre);  
LV_IMG_DECLARE(png_file_pre);	
LV_IMG_DECLARE(png_tool_pre);	
LV_IMG_DECLARE(png_wifi_disconnect);	
LV_IMG_DECLARE(png_wifi_connect);	
LV_IMG_DECLARE(png_w_pos);	
LV_IMG_DECLARE(png_m_pos);

LV_IMG_DECLARE(X_N);			
LV_IMG_DECLARE(X_P);			
LV_IMG_DECLARE(Y_N);			
LV_IMG_DECLARE(Y_P);
LV_IMG_DECLARE(png_m_up);
LV_IMG_DECLARE(png_m_right);		
LV_IMG_DECLARE(png_m_left);		
LV_IMG_DECLARE(png_m_down);
LV_IMG_DECLARE(png_m_z_n);			
LV_IMG_DECLARE(png_m_z_n_pre);		
LV_IMG_DECLARE(png_m_z_p);			
LV_IMG_DECLARE(png_m_z_p_pre);

LV_IMG_DECLARE(png_xyhome);
LV_IMG_DECLARE(png_z_home);
LV_IMG_DECLARE(png_z_down);
LV_IMG_DECLARE(png_z_up);
LV_IMG_DECLARE(png_up);
LV_IMG_DECLARE(png_down);
LV_IMG_DECLARE(png_left);
LV_IMG_DECLARE(png_right);

LV_IMG_DECLARE(png_xyhome_pre);
LV_IMG_DECLARE(png_z_home_pre);
LV_IMG_DECLARE(png_z_down_pre);
LV_IMG_DECLARE(png_z_up_pre);
LV_IMG_DECLARE(png_up_pre);
LV_IMG_DECLARE(png_down_pre);
LV_IMG_DECLARE(png_left_pre);
LV_IMG_DECLARE(png_right_pre);

LV_IMG_DECLARE(png_xyclear);
LV_IMG_DECLARE(png_zclear);
LV_IMG_DECLARE(png_knife);
LV_IMG_DECLARE(png_cooling);
LV_IMG_DECLARE(png_position);
LV_IMG_DECLARE(png_knife_pre);
LV_IMG_DECLARE(png_cooling_pre);
LV_IMG_DECLARE(png_position_pre);
LV_IMG_DECLARE(png_xyclear_pre);
LV_IMG_DECLARE(png_zclear_pre);

LV_IMG_DECLARE(png_stop);
LV_IMG_DECLARE(png_stop_pre);
LV_IMG_DECLARE(png_start);
LV_IMG_DECLARE(png_start_pre);
LV_IMG_DECLARE(png_print_file);
LV_IMG_DECLARE(png_adj);
LV_IMG_DECLARE(png_adj_pre);
LV_IMG_DECLARE(png_pause);
LV_IMG_DECLARE(png_pause_pre);

LV_IMG_DECLARE(png_sp_dec);
LV_IMG_DECLARE(png_sp_dec_pre);
LV_IMG_DECLARE(png_sp_add);
LV_IMG_DECLARE(png_sp_add_pre);
LV_IMG_DECLARE(png_sp_comfirm);
LV_IMG_DECLARE(png_sp_comfirm_pre);
LV_IMG_DECLARE(png_sp_back);
LV_IMG_DECLARE(png_sp_back_per);

LV_IMG_DECLARE(png_language_pre);
LV_IMG_DECLARE(png_language);

LV_IMG_DECLARE(png_l_up);
LV_IMG_DECLARE(png_l_up_pre);		
LV_IMG_DECLARE(png_l_next);		
LV_IMG_DECLARE(png_l_next_pre);


void mks_global_style_init(void);

#endif
