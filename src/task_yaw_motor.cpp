/** @file task_yaw_motor.cpp
 *  This file contains the contents of task yaw_motor, a task that controls the power
 *  delivered to the yaw motor.
 */

//includes

#include <task_yaw_motor.h>
#include <PrintStream.h>
#include <taskshare.h>

//Define Shared Variables
//Details
extern Share<bool> firePresent;         //Input
extern Share<int8_t> firePosH;         //Input
extern Share<bool> cameraConnected;     //Input
extern Share<bool> fireCentered;        //Output


//Define constants
uint8_t taskDelayYM = 10;
//Motor pins
const uint8_t YM1 = 27;
const uint8_t YM2 = 14;
//PWM specifications
const int PWM_CHANNELY1 = 0;
const int PWM_CHANNELY2 = 1;
const int PWM_FREQY = 1000;
const int PWM_RESOLUTIONY = 8;
const int MAX_DUTY_CYCLEY = (int)(pow(2, PWM_RESOLUTIONY) - 1); 

/** @brief Task yaw_motor controls the power delivered to the yaw motor
*   @details This task controls the motor that controls the yaw of the entire assembly. When no fire is detected,
*           the task spins the assembly counterclockwise slowly in order to scan the area for a fire. When a fire is detected,
*           the task adjusts the yaw of the assembly until it is centered on the fire. When the fire is out,
*           the task resumes spinning slowly
*   @param p_params Pointer is NULL
*/
void yaw_motor (void* p_params)
{
    //Define state variable
    uint8_t stateVariableYaw = 1;
    //State 0: Initialize
    //Variable to hold duty cycle value while centering
    uint8_t dutyCycleYaw = 0;
    //Variable to hold static slow duty cycle while spinning
    uint8_t dutyCycleYawSlow = 242;
    //Define local variables
    bool localFirePresent = false;
    bool localCameraConnected = false;
    int8_t localFirePosH = 0;
    //Setup PWM channels
    ledcSetup(PWM_CHANNELY1, PWM_FREQY, PWM_RESOLUTIONY);
    ledcSetup(PWM_CHANNELY2, PWM_FREQY, PWM_RESOLUTIONY);
    ledcAttachPin(YM1, PWM_CHANNELY1);
    ledcAttachPin(YM2, PWM_CHANNELY2);
    //STOP motor
    ledcWrite(PWM_CHANNELY1, MAX_DUTY_CYCLEY);
    ledcWrite(PWM_CHANNELY2, MAX_DUTY_CYCLEY);
    Serial <<  "yaw_motor running" << endl;
    vTaskDelay(taskDelayYM);
    while(true)
    {   
        if (stateVariableYaw == 1)
        {
            //State 1: Check camera connection
            //Check if camera is connected
            cameraConnected.get(localCameraConnected);
            if(localCameraConnected)
            {
                //If it is, move to state 2
                stateVariableYaw = 2;
            }
            //else do nothing        
        }
        else if(stateVariableYaw == 2)
        {
            //State 2: Check if fire is present
            firePresent.get(localFirePresent);
            if(!localFirePresent)
            {
                //No fire present
                //Rotate slowly CCW
                ledcWrite(PWM_CHANNELY2, dutyCycleYawSlow);
            }
            else
            {
                //Fire present, move to state 3
                stateVariableYaw = 3;
            }
        }
        else if(stateVariableYaw == 3)
        {
            //State 2: Fire is present, stop motors
            //Stop motor to allow for a change in direction
            ledcWrite(PWM_CHANNELY2, MAX_DUTY_CYCLEY);
            ledcWrite(PWM_CHANNELY2, MAX_DUTY_CYCLEY);
            //Move to state 3
            stateVariableYaw = 4;
        }
        else if(stateVariableYaw == 4)
        {
            //Get fire position and presence
            firePosH.get(localFirePosH);
            firePresent.get(localFirePresent);
            //adjust motor speed based on fire position
            dutyCycleYaw = -13*log10((1.2*abs(localFirePosH)));
            //Check if fire is centered, if not move accordinly
            if(localFirePosH  > 3)
            {
                //Motors CCW
                ledcWrite(PWM_CHANNELY2, dutyCycleYaw);
                //Return to state 3 to allow for a change in direction
                stateVariableYaw = 3;
            }
            else if (localFirePosH < -3)
            {
                //Motors move CW
                ledcWrite(PWM_CHANNELY1, dutyCycleYaw);
                //Return to state 3 to allow for a change in direction
                stateVariableYaw = 3;
            }
            else if (localFirePresent)
            {
                //Fire is centered
                fireCentered.put(true);
                Serial.println("Fire Centered");
                //Motors stay stopped from previous state
            }
            else
            {
                //No fire is present, return to state 2 to wait for fire
                stateVariableYaw = 2;
            }
        }
        else
        {
            //Error state, return to state 2 
            Serial << "Yaw Motor State Error";
            stateVariableYaw = 2;
        }
        vTaskDelay(taskDelayYM);
    }
}