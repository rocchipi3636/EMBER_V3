/** @file task_solenoid.cpp
 *  This file contains the contents of task solenoid, a task that controls the operation of the solenoid
 */

//includes

#include <task_solenoid.h>
#include <PrintStream.h>
#include <taskshare.h>

//Define Shared Variables
//Details
extern Share<bool> solenoidSwitched;        //INPUT

//Constants
const uint8_t taskDelayS = 10;
const uint8_t solenoidPin = 25;

/** @brief Task solenoid controls the operation of the solenoid
*   @details This task reads the shared variable solenoidSwitched to determine when the solenoid should be
*           switched on or off.
*   @param p_params Pointer is NULL
*/
void solenoid (void* p_params)
{
    uint8_t stateVariable = 1;
    //State 0: Initialize
    //Define local variables
    bool localSolenoidSwitched = false;
    //Initialize Pin modes
    pinMode(solenoidPin, OUTPUT);
    Serial <<  "solenoid running" << endl;
    vTaskDelay(taskDelayS);
    while(true)
    {
        if(stateVariable == 1){
            //State 1: Solenoid off
            //Turn solenoid off
            digitalWrite(solenoidPin, LOW);
            //check if solenoid should be on;
            solenoidSwitched.get(localSolenoidSwitched);
            if(localSolenoidSwitched)
            {
                //if it should be on, go to state 2
                stateVariable = 2;
            }
        }
        else if(stateVariable == 2)
        {
            //State 2: Solenoid on
            //Turn solenoid on
            digitalWrite(solenoidPin, HIGH);
            //check if solenoid should be off
            solenoidSwitched.get(localSolenoidSwitched);
            if(!localSolenoidSwitched)
            {
                //if it should be off, go to state 1
                stateVariable = 1;
            }
        }
        else
        {
            //Error state
            Serial << "Solenoid State Error";
            //return to state 1
            stateVariable = 1;
        }
        vTaskDelay(taskDelayS);
    }
}