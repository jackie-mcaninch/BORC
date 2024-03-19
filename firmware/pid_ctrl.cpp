//=========================================================================================================
// pid_ctrl.cpp - Keeps track of the information necessary to perform PID control
//=========================================================================================================
#include "globals.h"

#define	IMPLEMENTATION FIFO
#define HIST_THRESHOLD 100


struct history_point
{
    float temperature;
    uint8_t notch;
    uint32_t timestamp;
};

CPidController::CPidController() : m_history(sizeof(history_point), HIST_THRESHOLD, IMPLEMENTATION, true) {}

void CPidController::init()
{
    // initialize at fully closed, timer at zero
    m_current_mode = REST_MODE;
    m_timer = 0;
    m_last_action_ts = 0;
}

void CPidController::reset()
{
    // reset to fully closed, timer at zero
    m_current_mode = REST_MODE;
    m_timer = 0;
    m_last_action_ts = 0;

    // clear queues
    m_history.flush()
}

bool CPidController::enqueue_history_point(float temp_pt, uint8_t notch_pt)
{
    history_point hp = {temp_pt, notch_pt, m_timer};
    return m_history.push(&hp);
}

temp_ctrl_mode_t CPidController::switch_control_mode(float curr_temp)
{
    // keep it simple for testing purposes first


}

uint8_t CPidController::get_new_notch_pos(float curr_temp, uint8_t curr_notch, uint32_t dt)
{
    // get the current time
    m_timer += dt;
    
    // record the current conditions for PID
    enqueue_history_point(curr_temp, curr_notch);

    // set a default value for the new notch position
    uint8_t new_notch_pos = curr_notch;

    // take an action depending on what mode we in
    switch(m_current_mode) {

        // TODO: m_last_action_ts MUST BE UPDATED INSIDE EACH FUNCTION BELOW

        // CLIMB: setpoint is far away, turn BORC fully open
        case CLIMB_MODE:
            new_notch_pos = get_new_notch_pos_climb();
            break;

        // PID: we are in range of setpoint, use PID temperature control to find optimal position
        case PID_MODE:
            new_notch_pos = get_new_notch_pos_pid();
            break;

        // ECO: we are in deadband but want to save power, keep notch the same for time period
        case ECO_MODE:
            new_notch_pos = get_new_notch_pos_eco();
            break;

        // WAIT: we detected steam is not on, keep notch the same for time period
        case WAIT_MODE:
            new_notch_pos = get_new_notch_pos_wait();
            break;

        // REST: we are outside of user-defined working hours, turn BORC fully closed
        case REST_MODE:
            new_notch_pos = get_new_notch_pos_rest();
            break;

        default:
            break;
            // log that something went wrong
    }

    // see if we need to switch between modes
    m_current_mode = switch_control_mode(curr_temp);

    // return the resulting notch to travel to
    return new_notch_pos;
}
