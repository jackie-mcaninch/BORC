#include "globals.h"

//=========================================================================================================
// start() - change
//=========================================================================================================
bool flag = false;
void CHibernationModeMgr::start()
{
    ee.run_mode = System.iface_mode = HIBERNATION_MODE;
    Display.print("xx");
    delay(3000);
    Display.clear();

     
}

void CHibernationModeMgr::execute()
{
   
    knob_event_t event;

    while (Knob.get_event(&event))
    {
        
        switch (event)
        {
        case KNOB_LPRESS:
            MenuMgr.start();
            break;
        }
    }
}
