/** @file task_solenoid.h
 *  This file contains the definition for the solenoid task function
 */

#ifndef _TASK_SOLENOID_    // These lines prevent the contents of this file from
#define _TASK_SOLENOID_    // being #included more than once in any .cpp file

//includes
#include <Arduino.h>    

//Defining solenoid, a task function with a void pointer.
void solenoid (void* p);

#endif // _TASK_SOLENOID_
