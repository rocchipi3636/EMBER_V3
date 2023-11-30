/** @file task_thermal_camera.h
 *  This file contains the definition for the debounce task function
 */

#ifndef _TASK_THERMAL_CAMERA_    // These lines prevent the contents of this file from
#define _TASK_THERMAL_CAMERA_    // being #included more than once in any .cpp file

//includes
#include <Arduino.h>    

//Defining thermal_camera, a task function with a void pointer.
void thermal_camera (void* p);

#endif // _TASK_THERMAL_CAMERA_
