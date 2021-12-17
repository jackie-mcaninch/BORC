#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <Arduino.h>
#include "common.h"
#include "changelog.h"
#include "rotary_knob.h"
#include "display.h"
#include "power_manager.h"
#include "servo.h"
#include "system.h"
#include "manual_mode.h"
#include "sleep_manager.h"
#include "led_manager.h"
#include "menu_manager.h"
#include "setpoint_mode.h"
#include "eeprom_manager.h"
#include "temphum.h"
#include "serialserver.h"
#include "pid_ctrl.h"

extern CRotaryKnob      Knob;
extern CLedMatrix       Display;
extern CPowerManager    PowerMgr;
extern CServoDriver     Servo;
extern CSystem          System;
extern CManualModeMgr   ManualModeMgr;
extern CSleepMgr        SleepMgr;
extern CLedMgr          Led;
extern CMenuMgr         MenuMgr;
extern CSetpointModeMgr SetpointModeMgr;
extern CTempHum         TempHum;
extern CSerialServer    SerialServer;
extern CPIDController   PID;
extern CEEPROM          EEPROM;
extern CEEPROM::data_t& ee;
int c_to_f(float c);
float f_to_c(int f);


#endif
