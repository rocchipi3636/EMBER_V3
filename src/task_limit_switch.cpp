/** @file task_limit_switch.cpp
 *  This file contains the contents of task limit switch
 */

//includes

#include <task_limit_switch.h>
#include <PrintStream.h>
#include <taskshare.h>

//Define Shared Variables
//Details
extern Share<bool> hide;

/** @brief Task
*   @details Task
*   @param p_params Pointer is NULL
*/
void limit_switch (void* p_params)
{
    Serial <<  "limit_switch running";
    while(true)
    {
        vTaskDelay(100);
    }
}