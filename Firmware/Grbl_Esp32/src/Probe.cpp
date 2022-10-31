/*
  Probe.cpp - code pertaining to probing methods
  Part of Grbl

  Copyright (c) 2014-2016 Sungeun K. Jeon for Gnea Research LLC

	2018 -	Bart Dring This file was modifed for use on the ESP32
					CPU. Do not use this with Grbl for atMega328P

  Grbl is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Grbl is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Grbl.h"
#include "mks/MKS_draw_lvgl.h"

// Inverts the probe pin state depending on user settings and probing cycle mode.
static bool is_probe_away;

PROBE_CTRL_T probe_ctrl;

// Probe pin initialization routine.
void probe_init() {
    static bool show_init_msg = true;  // used to show message only once.

    if (PROBE_PIN != UNDEFINED_PIN) {
#ifdef DISABLE_PROBE_PIN_PULL_UP
        pinMode(PROBE_PIN, INPUT);
#else
        pinMode(PROBE_PIN, INPUT_PULLUP);  // Enable internal pull-up resistors. Normal high operation.
#endif

        if (show_init_msg) {
            grbl_msg_sendf(CLIENT_SERIAL, MsgLevel::Info, "Probe on pin %s", pinName(PROBE_PIN).c_str());
            show_init_msg = false;
        }
    }
}

void set_probe_direction(bool is_away) {
    is_probe_away = is_away;
}

// Returns the probe pin state. Triggered = true. Called by gcode parser and probe state monitor.
bool probe_get_state() {
    return (PROBE_PIN == UNDEFINED_PIN) ? false : digitalRead(PROBE_PIN) ^ probe_invert->get();
}

// Monitors probe pin state and records the system position when detected. Called by the
// stepper ISR per ISR tick.
// NOTE: This function must be extremely efficient as to not bog down the stepper ISR.
void probe_state_monitor() {
    if (probe_get_state() ^ is_probe_away) {
        sys_probe_state = Probe::Off;
        memcpy(sys_probe_position, sys_position, sizeof(sys_position));
        sys_rt_exec_state.bit.motionCancel = true;
    }
}


static void event_probe_yes(lv_obj_t* obj, lv_event_t event) {
    if(event == LV_EVENT_RELEASED) {
        if(PROBE_READ() == PROBE_READ_LOW) {
            grbl_send(CLIENT_SERIAL, "probe is low\n");
        }else {
            grbl_send(CLIENT_SERIAL, "probe is high\n");

            if(mks_ui_page.mks_ui_page != MKS_UI_Ready) {
                mks_ui_page.mks_ui_page = MKS_UI_PAGE_LOADING;
                lv_obj_clean(mks_global.mks_src);
                mks_draw_ready();
            }else {
                common_popup_com_del();
            }
            probe_ctrl.status = PROBE_GET_STATUS;
        }
    }
}

void mks_probe_init(void) { 

    if (PROBE_PIN != UNDEFINED_PIN) { 
        pinMode(PROBE_PIN, INPUT_PULLUP);
        probe_ctrl.is_probe_open = true;

        if(mks_grbl.is_test_mode == true) {
            probe_ctrl.status = PROBE_NONE;
        }else {
            probe_ctrl.status = PROBE_GET_STATUS;
        }
        probe_ctrl.probe_time_start = 0;
        probe_ctrl.probe_time_end = 0;
    }
} 

void mks_probe_check_disable() {
    probe_ctrl.status = PROBE_NONE;
}

void mks_probe_check_enable() {
    probe_ctrl.status = PROBE_GET_STATUS;
}

void mks_probe_check(void) {

    switch(probe_ctrl.status) {

        case PROBE_NONE:
        break;

        case PROBE_GET_STATUS: 
            if(PROBE_READ() == PROBE_READ_LOW) {
                probe_ctrl.probe_time_start = millis();
                probe_ctrl.status = PROBE_IS_WRONG;
            }
        break;

        case PROBE_IS_WRONG:
            if(PROBE_READ() == PROBE_READ_LOW) { // 再次确定触发了

                probe_ctrl.probe_time_end = millis();

                if( probe_ctrl.probe_time_end -  probe_ctrl.probe_time_start >= 500) {  // 防抖延时

                    if((sys.state == State::Cycle) || (sys.state == State::Hold))  {  // 正在打印中
                    uint16_t buf_cmd[]={0x18};
                    closeFile();
                    MKS_GRBL_CMD_SEND(buf_cmd);
                    }
                    mks_draw_common_popup_info_com( "Error", 
                                                    "Machine accident", 
                                                    "Please make sure the machine is normal",
                                                    event_probe_yes);

                    probe_ctrl.status = PROBE_WAIT_WRONG;
                }
            }
            else {
                probe_ctrl.status = PROBE_GET_STATUS;
                probe_ctrl.probe_time_start = 0;
                probe_ctrl.probe_time_end = 0;
            }
        break;

        case PROBE_WAIT_WRONG:
            MKS_GRBL_CMD_SEND("M3 S0\n");
            if(PROBE_READ() == PROBE_READ_HIGH) {
                probe_ctrl.status = PROBE_IS_OK;
            }
        break;

        case PROBE_IS_OK:
            if(PROBE_READ() == PROBE_READ_HIGH) {
                probe_ctrl.status = PROBE_GET_STATUS;
                probe_ctrl.probe_time_start = 0;
                probe_ctrl.probe_time_end = 0;
            }
        break;
    }
}
