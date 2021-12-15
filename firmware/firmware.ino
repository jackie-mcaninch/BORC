#include "globals.h"
#include "common.h"
#include <avr/wdt.h>

//=========================================================================================================
// Setup
//=========================================================================================================
void setup()
{    
  Serial.begin(115200);
  Serial.println("begin");

  // blink aqua for 1 second to show device booted
  Led.set(AQUA, 1000, true);

  // get the stored values from EEPROM
  // !!! THIS HAS TO BE THE FIRST THING WE DO !!!
  EEPROM.read();

  // !!! THIS HAS TO BE THE SECOND THING WE DO !!!
  System.init();

  // initialize the power manager
  PowerMgr.init();

  // turn power all for all devices
  PowerMgr.powerOnAll(); 

  // initialize all devices
  Knob.init(CHANNEL_A, CHANNEL_B, CLICK_PIN);
  Display.init();
  Servo.init();
  Led.init();

  // initialize sleep nanager
  SleepMgr.init();

  // restore the system orientation from EEPROM
  System.set_orientation(ee.orientation);

  // set system mode to the actual run mode
  System.return_to_run_mode();

  // if the servo hasn't been successfully calibrated, do so
  // if (!ee.is_servo_calibrated)  Servo.calibrate_bare();

  // Servo.move_to_pwm(92, 4000, true);
  // Servo.move_to_pwm(542, 4000, true);  
}
//=========================================================================================================

//=========================================================================================================
// BIG LOOP
//=========================================================================================================
void loop()
{
  wdt_reset(); //pat the dog...

  SleepMgr.execute();

  Led.execute();

  switch (System.iface_mode)
  {
    case MANUAL   : ManualModeMgr.execute();    break;
    case MENU     : MenuMgr.execute();          break;
    case SETPOINT : SetpointModeMgr.execute();  break;
  }
}
//=========================================================================================================
