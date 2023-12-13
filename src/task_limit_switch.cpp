/** @file task_limit_switch.cpp
 *  This file contains the contents of task limit switch, as task that mananages reading the states of the limit switches
 */

//includes

#include <task_limit_switch.h>
#include <PrintStream.h>
#include <taskshare.h>

//Define Shared Variables
extern Share<uint8_t> topLimitSwitch;       //Output
extern Share<uint8_t> bottomLimitSwitch;    //Output

//Constants
const uint8_t taskDelayLSW = 10;
//Define GPIO pins for switches
const uint8_t pinTLSW = 5;
const uint8_t pinBLSW = 18;


/** @brief Task limit_switch reads the state of the limit switches
*   @details This task reads the states of the limit switches, the top switch through GPIO 5
*           and the bottom through GPIO 18. The states are then uploaded to the shared variables
*           topLimitSwitch and bottomLimitSwitch
*   @param p_params Pointer is NULL
*/
void limit_switch (void* p_params)
{
    //State 0: Initiliaze
    //Initiliaze inputs
    pinMode(pinTLSW, INPUT);
    pinMode(pinBLSW, INPUT);
    Serial <<  "limit_switch running" << endl;
    vTaskDelay(taskDelayLSW);
    while(true)
    {
        //State 1: Read Limit Switches and set shared variables
        topLimitSwitch.put(digitalRead(pinTLSW));
        bottomLimitSwitch.put(digitalRead(pinBLSW));
        vTaskDelay(taskDelayLSW);
    }
}