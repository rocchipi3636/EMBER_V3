/** @file task_yaw_motor.h
 *  This file contains the definition for the yaw_motor task function
 */

#ifndef _TASK_YAW_MOTOR_    // These lines prevent the contents of this file from
#define _TASK_YAW_MOTOR_    // being #included more than once in any .cpp file

//includes
#include <Arduino.h>    

//Defining yaw_motor, a task function with a void pointer.
void yaw_motor (void* p);

#endif // _TASK_YAW_MOTOR_
