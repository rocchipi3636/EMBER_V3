/** @file task_pitch_motor.cpp
 *  This file contains the contents of task pitch motor, a task which controls the power delivered to
 *  the pitch motor.
 */

//includes

#include <task_pitch_motor.h>
#include <PrintStream.h>
#include <taskshare.h>

//Define Shared Variables
extern Share<bool> fireCentered;            //Input
extern Share<uint8_t> topLimitSwitch;       //Input
extern Share<uint8_t> bottomLimitSwitch;    //Input
extern Share<bool> solenoidSwitched;        //Output

//Constants
const uint8_t taskDelayPM = 8;
//Motor pins
const uint8_t PM1 = 12;
const uint8_t PM2 = 13;
//PWM specifications
const int PWM_CHANNELP1 = 2;
const int PWM_CHANNELP2 = 3;
const int PWM_FREQP = 1000;
const int PWM_RESOLUTIONP = 8;
const int MAX_DUTY_CYCLEP = (int)(pow(2, PWM_RESOLUTIONP) - 1); 

/** @brief Task pitch_motor controls the movement of the pitch motor
*   @details This task controls the motor that adjusts the shooting arm pitch. While no fire is present, the pitch arm
*           resets back to the up position. While a fire is present, the pitch motor moves up and down repeatedly
*           within the bounds of the limit switch and signals the solenoid to turn on.
*   @param p_params Pointer is NULL
*/
void pitch_motor (void* p_params)
{
    //Define state variable
    uint8_t stateVariable = 1;
    //State 0: Initialize
    //Ensure solenoid is switched off during pitch motor initialization to prevent false positives
    solenoidSwitched.put(false);
    //Define local shared variables
    bool localFireCentered = false;
    uint8_t localTopLimitSwitch = false;
    uint8_t localBottomLimitSwitch = false;
    //Define variable to track movement direction
    bool movingDown = true;
    //Define motor speed
    uint8_t dutyCyclePitch = 245;
    //Setup PWM channels
    ledcSetup(PWM_CHANNELP1, PWM_FREQP, PWM_RESOLUTIONP);
    ledcSetup(PWM_CHANNELP2, PWM_FREQP, PWM_RESOLUTIONP);
    ledcAttachPin(PM1, PWM_CHANNELP1);
    ledcAttachPin(PM2, PWM_CHANNELP2);
    //STOP motor
    ledcWrite(PWM_CHANNELP1, MAX_DUTY_CYCLEP);
    ledcWrite(PWM_CHANNELP2, MAX_DUTY_CYCLEP);
    Serial <<  "pitch_motor running" << endl;
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
            if(localTopLimitSwitch != 1)
            {
                ledcWrite(PWM_CHANNELP2, dutyCyclePitch);
            }
            else
            {
                //When at top, stop moving and set direction of movement
                ledcWrite(PWM_CHANNELP1, MAX_DUTY_CYCLEP);
                ledcWrite(PWM_CHANNELP2, MAX_DUTY_CYCLEP);
                movingDown = true;
            }
            //Check if fire is centered (also implies present)   
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
            ledcWrite(PWM_CHANNELP1, MAX_DUTY_CYCLEP);
            ledcWrite(PWM_CHANNELP2, MAX_DUTY_CYCLEP);
            //Check if fire is still present
            fireCentered.get(localFireCentered);
            //Decided what direction to move next.
            if(!localFireCentered)
            {
                //Fire is no longer present, move to state 1
                stateVariable = 1;
            }
            else if(movingDown)
            {
                //if fire present and moving down, move to state 3
                stateVariable = 3;
            }
            else
            {
                //if fire present and moving up, move to state 4
                stateVariable = 4;
            }

        }
        else if(stateVariable == 3)
        {
            //State 3: Arm moving down
            //Move arm downwards
            ledcWrite(PWM_CHANNELP1, dutyCyclePitch);
            //Check limit switch
            bottomLimitSwitch.get(localBottomLimitSwitch);
            if(localBottomLimitSwitch == 1)
            {
                //if the bottom limit switch is triggered change direction
                movingDown = false;
                stateVariable = 2;
            }
            //Check if fire is still present
            fireCentered.get(localFireCentered);
            if(!localFireCentered)
            {
                //Fire is no longer present, override previous state change and move to state 1
                stateVariable = 1;
            }
        }
        else if(stateVariable == 4)
        {
            //State 4: Arm moving up
            ledcWrite(PWM_CHANNELP2, dutyCyclePitch);
            //Read limit switch
            topLimitSwitch.get(localTopLimitSwitch);
            if(localTopLimitSwitch == 1)
            {
                //if the top limit switch is triggered change direction
                movingDown = true;
                stateVariable = 2;
            }
            //Check if fire is still present
            fireCentered.get(localFireCentered);
            if(!localFireCentered)
            {
                //Fire is no longer present, override previous state change and move to state 1
                stateVariable = 1;
            }
        }
        else
        {
            //Error state
            Serial << "Pitch Motor State Error" << endl;
            //Reset back to state 1
            stateVariable = 1;
        }
        vTaskDelay(taskDelayPM);
    }
}