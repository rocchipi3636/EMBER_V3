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

//Local Variables
const uint8_t YM1 = 27;
const uint8_t YM2 = 14;
const int PWM_CHANNEL1 = 0;
const int PWM_CHANNEL2 = 1;
const int PWM_FREQ = 1000;
const int PWM_RESOLUTION = 8;
const int MAX_DUTY_CYCLE = (int)(pow(2, PWM_RESOLUTION) - 1); 
uint8_t taskDelay = 3;

/** @brief Task
*   @details Task
*   @param p_params Pointer is NULL
*/
void yaw_motor (void* p_params)
{
    //State 0: Initialize
    Serial <<  "yaw_motor running";
    //Define dynamic variables
    uint8_t stateVariable = 1;
    u_int8_t dutyCycleYaw = 0;
    bool localFirePresent = false;
    uint8_t localFirePosH = 0;
    //Define PWM channels
    ledcSetup(PWM_CHANNEL1, PWM_FREQ, PWM_RESOLUTION);
    ledcSetup(PWM_CHANNEL2, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(YM1, PWM_CHANNEL1);
    ledcAttachPin(YM2, PWM_CHANNEL2);
    //STOP motor
    ledcWrite(PWM_CHANNEL1, MAX_DUTY_CYCLE);
    ledcWrite(PWM_CHANNEL2, MAX_DUTY_CYCLE);
    while(true)
    {        
        if(stateVariable == 1)
        {
            //State 1: Check if fire is present
            firePresent.get(localFirePresent);
            if(!localFirePresent)
            {
                //No fire present
                //Rotate slowly CCW
                ledcWrite(PWM_CHANNEL2, 13);
            }
            else
            {
                //Fire present, move to state 2
                stateVariable = 2;
            }
        }
        else if(stateVariable == 2)
        {
            //State 2: Fire is present
            //Stop motors to allow for a change in direction
            ledcWrite(PWM_CHANNEL2, MAX_DUTY_CYCLE);
            ledcWrite(PWM_CHANNEL2, MAX_DUTY_CYCLE);
            //Move to state 3
            stateVariable = 3;
        }
        else if(stateVariable == 3)
        {
            //Get fire position
            firePosH.get(localFirePosH);
            //adjust motor speed based on fire position
            dutyCycleYaw = (int)(1.5*abs(localFirePosH));
            //move motor
            if(localFirePosH  > 2)
            {
                ledcWrite(PWM_CHANNEL1, dutyCycleYaw);
            }
            else if (localFirePosH < -2)
            {
                ledcWrite(PWM_CHANNEL2, dutyCycleYaw);
            }
            else if (localFirePresent)
            {
                //Fire is centered
                fireCentered.put(true);
                //Motors stay stopped
            }
            else
            {
                //No fire is present, return to state 1
                stateVariable = 1;
            }
            //Return to state 2 to allow for a change of direction
            stateVariable = 2;
        }
        else
        {
            //Error state, return to state 1
            Serial << "Yaw Motor State Error";
            stateVariable = 1;
        }
        Serial << stateVariable << endl;
        vTaskDelay(taskDelay);
    }
}