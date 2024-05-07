//=========================================================================================================
// pid_ctrl.cpp - Keeps track of the information necessary to perform PID control
//=========================================================================================================
#include "globals.h"

#define HIST_THRESHOLD 100
#define CLIMB_EXPIRATION_SECS 1200 // 20 mins in between checks


struct history_point
{
    float temperature;
    uint8_t notch;
    uint32_t timestamp;
};

CPidController::CPidController() : m_history(sizeof(history_point), HIST_THRESHOLD, FIFO, true) {}

void CPidController::init()
{
    // initialize at fully closed, timer at zero
    m_current_mode = REST_MODE;
    m_timer = 0;
    m_last_action_ts = 0;
    // Gain values require tuning
    Kp = 0.5; // Proportional gain
    Ki = 0.1; // Integral gain
    Kd = 0.01; // Derivative gain
}

void CPidController::reset()
{
    // reset to fully closed, timer at zero
    m_current_mode = REST_MODE;
    m_timer = 0;
    m_last_action_ts = 0;

    // clear queue
    m_history.flush();
}

void CPidController::set_notch_count(uint8_t value)
{
    m_notch_count = value;
}

bool CPidController::enqueue_history_point(float temp_pt, uint8_t notch_pt)
{
    history_point hp = {temp_pt, notch_pt, m_timer};
    return m_history.push(&hp);
}

temp_ctrl_mode_t CPidController::switch_control_mode(float curr_temp, float l_bound, float u_bound)
{
    // compute the time elapsed since last action
    uint32_t time_since_last_action = m_timer - m_last_action_ts;
    
    switch (m_current_mode) {
        case CLIMB_MODE:
            // test if our climb timer has expired
            if (time_since_last_action > CLIMB_EXPIRATION_SECS) {
                // now we can see whether to switch to another mode
                return curr_temp > u_bound ? REST_MODE : CLIMB_MODE;
                
            }
            // if timer hasn't expired yet, stay in climb mode
            else { return CLIMB_MODE; }
        
        case PID_MODE:
            break;

        case ECO_MODE:
            break;

        case WAIT_MODE:
            break;

        case REST_MODE:
            // we should always be in rest mode unless temp drops too much
            return curr_temp < l_bound ? CLIMB_MODE : REST_MODE;

        default:
            // log something went wrong here
            break;
    }
    return REST_MODE;

}

uint8_t CPidController::get_new_notch_pos_pid(float curr_temp, uint8_t curr_notch, float l_bound, float u_bound)
{
        float desired_temp = (l_bound + u_bound)/2;
        double error = desired_temp - curr_temp;

        // Proportional term
        double p_term = Kp * error;

        // Integral term
        integral += error;
        double i_term = Ki * integral;

        // Derivative term
        double d_term = Kd * (error - prev_error);

        // Calculate new notch
        double pid_output = p_term + i_term + d_term;
        Serial.println("PID output:");
        Serial.println(pid_output);
        Serial.println("Current notch:");
        Serial.println(curr_notch);
        float new_notch = (float)(pid_output * (TempCtrl.MAX_NOTCHES-1)/2 + curr_notch);
        Serial.println(new_notch);
        new_notch = max(0, min(TempCtrl.MAX_NOTCHES-1, new_notch));

        // Save current error for next iteration
        prev_error = error;

        return (uint8_t)new_notch;
}

uint8_t CPidController::get_new_notch_pos(float curr_temp, float l_bound, float u_bound, uint8_t curr_notch, uint32_t dt)
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
            new_notch_pos = get_new_notch_pos_pid(curr_temp, curr_notch, l_bound, u_bound);
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
    m_current_mode = switch_control_mode(curr_temp, l_bound, u_bound);

    // return the resulting notch to travel to
    return new_notch_pos;
}
