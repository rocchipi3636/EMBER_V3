/** @file shares.h
 *  This file contains shared variables between all of the tasks
 */

#ifndef _SHARES_    // These lines prevent the contents of this file from
#define _SHARES_    // being #included more than once in any .cpp file

//includes
#include <Arduino.h> 
#include <taskshare.h>

//Defining shared variables
//Fire Present, used by thermal camera, data upload, pitch motor, and yaw motor
Share<bool> firePresent;
//Fire position horizontal and vertical, used by thermal camera, data upload, pitch motor, and yaw motor
Share<int8_t> firePosH;
Share<int8_t> firePosV;
//Fire is centered, used by yaw motor and pitch motor
Share<bool> fireCentered;

#endif // _SHARES_