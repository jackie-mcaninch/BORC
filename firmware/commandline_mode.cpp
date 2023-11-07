#include "globals.h"

//=========================================================================================================
// start() - change
//=========================================================================================================
void CCommandLineModeMgr::start()
{
    ee.run_mode = System.iface_mode = COMMANDLINE_MODE;

    // display the current manual mode index
    Display.print("!!");
}

void CCommandLineModeMgr::execute()
{
}
//=========================================================================================================
