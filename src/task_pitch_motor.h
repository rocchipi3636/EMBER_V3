/** @file task_pitch_motor.h
 *  This file contains the definition for the pitch_motor task function
 */

#ifndef _TASK_PITCH_MOTOR_    // These lines prevent the contents of this file from
#define _TASK_PITCH_MOTOR_    // being #included more than once in any .cpp file

//includes
#include <Arduino.h>    

//Defining pitch_motor, a task function with a void pointer.
void pitch_motor (void* p);

#endif // _TASK_PITCH_MOTOR_
