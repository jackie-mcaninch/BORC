#ifndef _SETPOINT_MODE_H_
#define _SETPOINT_MODE_H_

#define MIN_SETPOINT        55
#define MAX_SETPOINT        90
#define DEFAULT_SETPOINT    72

class CSetpointModeMgr
{
public:

    void init();

    void start();
    
    void execute();

    int m_setpoint_f;
};

#endif
