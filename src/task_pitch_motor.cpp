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

u_int8_t motorPitchPin = 28;

/** @brief Task
*   @details Task
*   @param p_params Pointer is NULL
*/
void pitch_motor (void* p_params)
{
    Serial <<  "pitch_motor running";
    pinMode(motorPitchPin, OUTPUT);
    u_int8_t dutyCycleYaw = 0;
    while(true)
    {
        //Check if fire is present
        firePresent.get(localFirePresent);
        if(localFirePresent)
        {
            //If fire is present, locate
            firePosH.get(localFirePosH);
            //adjust motor speed
            dutyCycleYaw = 15*abs(localFirePosH);
            //move motor
            if(localFirePosH  > 2)
            {
                //Move motor forwards
            }
            else if (localFirePosH < -2)
            {
                //Move motor backwards
            }
            else
            {
                //Fire is centered
                fireCentered.put(true);
                //Stop motor
            }    
        }
        else
        {
            //If no fire, rotate slowly
            dutyCycleYaw = 40;
            //Move motor forwards
        }
        vTaskDelay(100);
    }
}