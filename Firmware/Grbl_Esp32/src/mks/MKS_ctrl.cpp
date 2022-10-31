#include "MKS_ctrl.h"
#include "../WebUI/WebSettings.h"
#include "../WebUI/WifiConfig.h"
#include "string.h"

mks_spindle_t mks_spindle;
void spindle_check_init(void) { 
    /*
        - 默认激光头空闲关使能
        - 默认起始时间0
        - 默认结束时间0
        - 默认等待时间30s
    */
    mks_spindle.spindle_status = sp_none;
    mks_spindle.tick_start = 0;
    mks_spindle.tick_end = 0;
    mks_spindle.out_time = 30*1000000; //30s
}

void spindle_check(void) {

    switch(mks_spindle.spindle_status) {

        case sp_none:
            mks_spindle.spindle_status = sp_begin;
        break;

        case sp_begin:
            if(stepper_idle) {
                mks_spindle.spindle_status = sp_load;
            }
        break;

        case sp_load:
            if(stepper_idle) {
                mks_spindle.tick_start = esp_timer_get_time();
                mks_spindle.spindle_status = sp_wait;
            }else {
                mks_spindle.spindle_status = sp_begin;
            }
        break;

        case sp_wait:
            if(stepper_idle) {
                mks_spindle.tick_end = esp_timer_get_time();

                if(mks_spindle.tick_end <= mks_spindle.tick_start) {   // 时间防止溢出
                    mks_spindle.tick_start = esp_timer_get_time();
                }
                else if((mks_spindle.tick_end - mks_spindle.tick_start) > mks_spindle.out_time) {
                    spindle->stop();
                    mks_spindle.spindle_status = sp_end;
                }
            }else {
                mks_spindle.spindle_status = sp_begin; // 重头开始
            }
        break;

        case sp_end:
            mks_spindle.tick_start = 0;
            mks_spindle.tick_end = 0;
            mks_spindle.spindle_status = sp_begin; // 重头开始
        break;
    }
}

/***********************************************************************MKS WIFI*************************************************************/
#if defined(ENABLE_WIFI)
MKS_WIFI_t mks_wifi;
const char mks_ssid[] = "MAKERBASE3D";
const char mks_password[] = "makerbase3d";

char WIFI_SSID_CMD[128]="ESP100";
char WIFI_PASSWORD_CMD[128]="ESP101";
char WIFI_CONNECT_CMD[]="ESP210";
char mks_wifi_send[128];

void mks_wifi_scanf(void) {
    int n = WiFi.scanComplete();
    mks_wifi.begin_scanf_num = 0;
    switch (n) {
        case -2:                      // Scan not triggered
            WiFi.scanNetworks(true);  // Begin async scan
            // grbl_send(CLIENT_SERIAL ,"scanNetworks\n\n");
            mks_wifi.wifi_scanf_status = wifi_scanf_begin;
            break;
        case -1:  // Scan in progress
            // grbl_send(CLIENT_SERIAL ,"waitting\n\n");
            mks_wifi.wifi_scanf_status = wifi_scanf_waitting;
            break;
        default:
            // 获取16个wifi名
            for (int i = 0; i < MKS_WIFI_NUM; ++i) {  
                memcpy(mks_wifi.wifi_name_str[mks_wifi.begin_scanf_num], WiFi.SSID(i).c_str(), 128);
                mks_wifi.wifi_rssi[i] = WiFi.RSSI(i);
                mks_wifi.begin_scanf_num++;
            }
            mks_wifi.wifi_scanf_status = wifi_scanf_succeed;
            WiFi.scanDelete();
            // Restart the scan in async mode so new data will be available
            // when we ask again.
            n = WiFi.scanComplete();
            if (n == -2) {
                WiFi.scanNetworks(true);
            }
            break;
        }
}

bool mks_get_wifi_status(void) { 

    wl_status_t status;
    status = WiFiSTAClass::status();
    if(status == WL_CONNECTED) {
        return true;
    }else {
        return false;
    }
}

uint8_t wifi_check_count = 0;
bool mks_wifi_check_is_out(bool flag) {

    if(flag == true) {
        wifi_check_count = 0;
        return true;
    }else {
        wifi_check_count++;
        if(wifi_check_count == 10) {
            MKS_GRBL_CMD_SEND("[ESP115]OFF\n");
            wifi_check_count = 0;
            return false;
        }
    }
}
#endif






