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
const uint8_t taskDelay = 100;
const uint8_t solenoidPin = 23;

/** @brief Task
*   @details Task
*   @param p_params Pointer is NULL
*/
void solenoid (void* p_params)
{
    //State 0: Initialize
    pinMode(solenoidPin, OUTPUT);
    Serial <<  "solenoid running";
    bool localSolenoidSwitched = false;
    vTaskDelay(taskDelay);
    while(true)
    {
        solenoidSwitched.get(localSolenoidSwitched);
        if(!localSolenoidSwitched){
            //State 1: Solenoid off
            digitalWrite(solenoidPin, false);
        }
        else
        {
            //State 2: Solenoid on
            digitalWrite(solenoidPin, true);
        }
        vTaskDelay(taskDelay);
    }
}