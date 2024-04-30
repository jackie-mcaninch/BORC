//=========================================================================================================
// pid_ctrl.h - Defines a controller which determines optimal servo position based on PID computation
//=========================================================================================================
#ifndef _PID_CTRL_H_
#define _PID_CTRL_H_
#include <cppQueue.h> // requires "Queue" lib by SMFSW

enum temp_ctrl_mode_t : uint8_t
{
    CLIMB_MODE =    0,
    PID_MODE =      1,
    ECO_MODE =      2,
    WAIT_MODE =     3,
    REST_MODE =     4
};

class CPidController 
{
public:
    CPidController();
    void init();
    void reset();
    void set_notch_count(uint8_t value);
    bool enqueue_history_point(float temp_pt, uint8_t notch_pt);
    temp_ctrl_mode_t switch_control_mode(float curr_temp, float l_bound, float u_bound);
    uint8_t get_new_notch_pos(float curr_temp, float l_bound, float u_bound, uint8_t curr_notch, uint32_t dt);
    uint8_t get_new_notch_pos_climb();
    uint8_t get_new_notch_pos_pid(float curr_temp, uint8_t curr_notch, float l_bound, float u_bound);
    uint8_t get_new_notch_pos_eco();
    uint8_t get_new_notch_pos_wait();
    uint8_t get_new_notch_pos_rest();

private:
    uint8_t m_notch_count = 10; // default value only, do not rely on this
    temp_ctrl_mode_t m_current_mode;
    uint32_t m_timer;
    uint32_t m_last_action_ts;

    cppQueue m_history;
};

#endif