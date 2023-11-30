/** @file task_limit_switch.h
 *  This file contains the definition for the limit_switch task function
 */

#ifndef _TASK_LIMIT_SWITCH_    // These lines prevent the contents of this file from
#define _TASK_LIMIT_SWITCH_    // being #included more than once in any .cpp file

//includes
#include <Arduino.h>    

//Defining limit_switch, a task function with a void pointer.
void limit_switch (void* p);

#endif // _TASK_LIMIT_SWITCH_
