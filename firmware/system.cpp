#include "globals.h"

void CSystem::init()
{
    if (ee.setpoint == 0)   ee.setpoint = f_to_c(DEFAULT_SETPOINT);
}

//=========================================================================================================
// reboot() - soft reboots the system
//=========================================================================================================
void CSystem::reboot()
{
    // reboot system
    asm volatile ("jmp 0");
}
//=========================================================================================================


//=========================================================================================================
// return_to_run_mode()
//=========================================================================================================
void CSystem::return_to_run_mode()
{
    if (ee.run_mode == MANUAL)
        ManualModeMgr.start();
    else
        SetpointModeMgr.start();
}
//=========================================================================================================


//=========================================================================================================
// rotate()
//=========================================================================================================
void CSystem::rotate()
{
  set_orientation(!ee.orientation);
}
//=========================================================================================================


//=========================================================================================================
// set_orientation()
//=========================================================================================================
void CSystem::set_orientation(bool orientation)
{
  ee.orientation = orientation;

  Knob.set_orientation(ee.orientation);

  Display.set_orientation(ee.orientation);
}
//=========================================================================================================
