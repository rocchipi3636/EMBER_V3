/** @file task_solenoid.cpp
 *  This file contains the contents of task solenoid
 */

//includes

#include <task_solenoid.h>
#include <PrintStream.h>
#include <taskshare.h>

//Define Shared Variables
//Details
extern Share<bool> hide;

/** @brief Task
*   @details Task
*   @param p_params Pointer is NULL
*/
void solenoid (void* p_params)
{
    Serial <<  "solenoid running";
    while(true)
    {
        vTaskDelay(100);
    }
}