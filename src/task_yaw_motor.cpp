/** @file task_yaw_motor.cpp
 *  This file contains the contents of task yaw_motor
 * controls how the assembly moves around the vertical axis
 */

//includes

#include <task_yaw_motor.h>
#include <PrintStream.h>
#include <taskshare.h>

//Define Shared Variables
//Details
extern Share<bool> firePresent;
extern Share<uint8_t> firePosH;
extern Share<bool> fireCentered;

u_int8_t motorYawPin = 27;

/** @brief Task
*   @details Task
*   @param p_params Pointer is NULL
*/
void yaw_motor (void* p_params)
{
    Serial <<  "yaw_motor running";
    pinMode(motorYawPin, OUTPUT);
    u_int8_t dutyCycleYaw = 0;
    bool localFirePresent = false;
    uint8_t localFirePosH = 0;
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