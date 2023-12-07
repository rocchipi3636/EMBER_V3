/** @file task_pitch_motor.cpp
 *  This file contains the contents of task pitch motor
 */

//includes

#include <task_pitch_motor.h>
#include <PrintStream.h>
#include <taskshare.h>

//Define Shared Variables
//Details
extern Share<bool> fireCentered;
extern Share<bool> topLimitSwitch;
extern Share<bool> bottomLimitSwitch;
extern Share<bool> solenoidSwitched;

//Constants
const uint8_t taskDelayPM = 20;
const uint8_t PM1 = 12;
const uint8_t PM2 = 13;
const int PWM_CHANNEL1 = 2;
const int PWM_CHANNEL2 = 3;
const int PWM_FREQ = 1000;
const int PWM_RESOLUTION = 8;
const int MAX_DUTY_CYCLE = (int)(pow(2, PWM_RESOLUTION) - 1); 

/** @brief Task
*   @details Task
*   @param p_params Pointer is NULL
*/
void pitch_motor (void* p_params)
{
    //State 0: Initialize
    Serial <<  "pitch_motor running";
    uint8_t stateVariable = 1;
    bool localFireCentered = false;
    bool localTopLimitSwitch = false;
    bool localBottomLimitSwitch = false;
    bool movingDown = true;
    u_int8_t dutyCyclePitch = 200;
    //Define PWM channels
    ledcSetup(PWM_CHANNEL1, PWM_FREQ, PWM_RESOLUTION);
    ledcSetup(PWM_CHANNEL2, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(PM1, PWM_CHANNEL1);
    ledcAttachPin(PM2, PWM_CHANNEL2);
    //STOP motor
    ledcWrite(PWM_CHANNEL1, MAX_DUTY_CYCLE);
    ledcWrite(PWM_CHANNEL2, MAX_DUTY_CYCLE);
    vTaskDelay(taskDelayPM);
    while(true)
    {   
        if(stateVariable == 1)
        {
            //State 1: Reset to default position
            //Ensure solenoid is off
            solenoidSwitched.put(false);
            //Reset arm position to top
            topLimitSwitch.get(localTopLimitSwitch);
            if(!localTopLimitSwitch)
            {
                ledcWrite(PWM_CHANNEL2, 240);
            }
            else
            {
                //When at top, stop moving and set direction of movement
                ledcWrite(PWM_CHANNEL2, MAX_DUTY_CYCLE);
                movingDown = true;
            }
            //Check if fire is centered (also imples present)   
            fireCentered.get(localFireCentered);
            if(localFireCentered)
            {
                //If fire is centered, move to state 2;
                stateVariable = 2;
            }
        }
        else if(stateVariable == 2)
        {
            //State 2: Fire present
            //Ensure solenoid on
            solenoidSwitched.put(true);
            //Then stop motor to allow for a change of direction
            ledcWrite(PWM_CHANNEL1, MAX_DUTY_CYCLE);
            ledcWrite(PWM_CHANNEL2, MAX_DUTY_CYCLE);
            //Check direction it should be moving and if fire is still present
            fireCentered.get(localFireCentered);
            if(!localFireCentered)
            {
                //Fire is no longer present, move to state 1
                stateVariable = 1;
            }
            else if(movingDown)
            {
                //if moving down move to state 3
                stateVariable = 3;
            }
            else
            {
                //if moving up move to state 4
                stateVariable = 4;
            }

        }
        else if(stateVariable == 3)
        {
            //State 3: Arm moving down
            ledcWrite(PWM_CHANNEL1, 240);
            //Check limit switch
            bottomLimitSwitch.get(localBottomLimitSwitch);
            if(localBottomLimitSwitch == 1)
            {
                //if the bottom limit switch is triggered change direction
                movingDown = false;
                stateVariable = 2;
            }
        }
        else if(stateVariable == 4)
        {
            //State 4: Arm moving up
            ledcWrite(PWM_CHANNEL2, 240);
            //Read limit switch
            topLimitSwitch.get(localTopLimitSwitch);
            if(localTopLimitSwitch)
            {
                //if the top limit switch is triggered change direction
                movingDown = true;
                stateVariable = 2;
            }
        }
        //Serial.println("PM: " + stateVariable);
        vTaskDelay(taskDelayPM);
    }
}