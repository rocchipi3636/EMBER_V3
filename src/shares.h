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
Share<uint8_t> firePosH;
Share<uint8_t> firePosV;
//Fire is centered, used by yaw motor and pitch motor
Share<bool> fireCentered;
//Limit Switch States, used by pitch motor and limit switch
Share<uint8_t> topLimitSwitch;
Share<uint8_t> bottomLimitSwitch;
//Solenoid state, used by pitch motor and solenoid
Share<bool> solenoidSwitched;


#endif // _SHARES_
