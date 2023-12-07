/** @file task_solenoid.cpp
 *  This file contains the contents of task solenoid
 */

//includes

#include <task_solenoid.h>
#include <PrintStream.h>
#include <taskshare.h>

//Define Shared Variables
//Details
extern Share<bool> solenoidSwitched;

//Constants
const uint8_t taskDelay = 10;
const uint8_t solenoidPin = 23;

/** @brief Task
*   @details Task
*   @param p_params Pointer is NULL
*/
void solenoid (void* p_params)
{
    //State 0: Initialize
    //Define dynamic variables
    uint8_t stateVariable = 1;
    bool localSolenoidSwitched = false;
    pinMode(solenoidPin, OUTPUT);
    Serial <<  "solenoid running";
    vTaskDelay(taskDelay);
    while(true)
    {
        if(stateVariable == 1){
            //State 1: Solenoid off
            digitalWrite(solenoidPin, false);
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
            digitalWrite(solenoidPin, true);
            //check if solenoid should be off;
            solenoidSwitched.get(localSolenoidSwitched);
            if(!localSolenoidSwitched)
            {
                //if it should be off, go to state 1
                stateVariable = 1;
            }
        }
        else
        {
            //Error state, return to state 1
            Serial << "Solenoid State Error";
            stateVariable = 1;
        }
        Serial << "Solenoid:" << localSolenoidSwitched << " " << stateVariable << endl;
        vTaskDelay(taskDelay);
    }
}