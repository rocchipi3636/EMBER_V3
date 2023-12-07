/** @file occhipinti.cpp
* This file contains a program that uses two tasks to hide and show a square wave using a pushbutton
*/

#include <Arduino.h>
#include <PrintStream.h>
#include <taskshare.h>
#include <shares.h>
#include <task_data_upload.h>
#include <task_limit_switch.h>
#include <task_pitch_motor.h>
#include <task_solenoid.h>
#include <task_thermal_camera.h>
#include <task_yaw_motor.h>


/** @brief The Arduino setup function.
* @details This function is used to set up the microcontroller by starting
* the serial port and creating the tasks.
*/
void setup (void)
{
  // Enable serial port
  Serial.begin (115200);
  while (!Serial)
  {}
  Serial.println("Serial Running");
  // Create data upload task
  //xTaskCreate (data_upload, "Data Upload", 1024, NULL, 5, NULL);
  // Create the limit switch task
  xTaskCreate (limit_switch, "Limit Switch", 1024, NULL, 5, NULL);
  // Create the pitch motor task
  xTaskCreate (pitch_motor, "Pitch Motor", 1024, NULL, 5, NULL);
  // Create the solenoid task
  xTaskCreate (solenoid, "Solenoid", 4096, NULL, 5, NULL);
  // Create the thermal camera task
  xTaskCreate (thermal_camera, "Thermal Camera", 12288, NULL, 5, NULL);
  // Create the yaw motor task
  xTaskCreate (yaw_motor, "Yaw Motor", 1024, NULL, 5, NULL);
  Serial.println("Tasks Initialized");
}

/** @brief The Arduino loop function.
* @details This function is called periodically by the Arduino system.
*/
void loop (void)
{
  // Delay for a minute
  vTaskDelay (60000);
}
