/** @file task_limit_switch.cpp
 *  This file contains the contents of task limit switch
 */

//includes

#include <task_limit_switch.h>
#include <PrintStream.h>
#include <taskshare.h>

//Define Shared Variables
extern Share<bool> topLimitSwitch;
extern Share<bool> bottomLimitSwitch;

//Constants
const uint8_t taskDelayLSW = 15;
//Define GPIO pins for switches
const uint8_t pinTLSW = 5;
const uint8_t pinBLSW = 18;


/** @brief Task
*   @details Task
*   @param p_params Pointer is NULL
*/
void limit_switch (void* p_params)
{
    //State 0: Initiliaze
    pinMode(pinTLSW, INPUT);
    pinMode(pinBLSW, INPUT);
    Serial <<  "limit_switch running";
    vTaskDelay(taskDelayLSW);
    while(true)
    {
        //State 1: Read Limit Switches and set shared variables
        topLimitSwitch.put(digitalRead(pinTLSW));
        bottomLimitSwitch.put(digitalRead(pinBLSW));
        vTaskDelay(taskDelayLSW);
    }
}