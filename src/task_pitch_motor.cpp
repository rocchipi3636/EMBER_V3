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
const uint8_t taskDelay = 100;
const uint8_t motorPitchPin = 28;

/** @brief Task
*   @details Task
*   @param p_params Pointer is NULL
*/
void pitch_motor (void* p_params)
{
    //State 0: Initialize
    Serial <<  "pitch_motor running";
    pinMode(motorPitchPin, OUTPUT);
    bool localFireCentered = false;
    bool localTopLimitSwitch = false;
    bool localBottomLimitSwitch = false;
    bool movingDown = true;
    u_int8_t dutyCyclePitch = 200;
    vTaskDelay(taskDelay);
    while(true)
    {
        //Load shared variables to local
        //fire is centered from motor yaw, note that centered implies present
        fireCentered.get(localFireCentered);
        //Variables for limit switch activation
        topLimitSwitch.get(localTopLimitSwitch);
        bottomLimitSwitch.get(localBottomLimitSwitch);
        
        if(!localFireCentered)
        {
            //State 1: Wait for fire to be centered
            //If fire is not present, move motor up to reset position
            if(!localTopLimitSwitch)
            {
                //Move motor up
            }
            //Additionally reset direction of movement
            movingDown = true;
            //And ensure solenoid is off
            solenoidSwitched.put(false);

        }
        else if(movingDown)
        {
            //State 2: Fire is centered, activate solenoid and move arm down
            solenoidSwitched.put(true);
            //Move motor down
            if(localBottomLimitSwitch)
            {
                //if the bottom limit switch is triggered
                movingDown = false;
            }
        }
        else if(!movingDown)
        {
            //State 3: fire is centered, move arm up
            //Move arm up
            if(localTopLimitSwitch)
            {
                //if the top limit switch is triggered
                movingDown = true;
            }
        }
        vTaskDelay(taskDelay);
    }
}