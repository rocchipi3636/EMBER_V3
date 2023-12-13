/** @file main.cpp
* This file contains the definitions for the tasks used in the program, as well as the webserver task.
*/

//Includes
#include <Arduino.h>
#include <Adafruit_I2CDevice.h>
#include <PrintStream.h>
#include <SPI.h>
#include <Wifi.h>
#include <WebServer.h>
#include <taskshare.h>
#include <shares.h>
#include <task_limit_switch.h>
#include <task_pitch_motor.h>
#include <task_solenoid.h>
#include <task_thermal_camera.h>
#include <task_yaw_motor.h>

//Define Shared Variables
extern Share<float*> printedFrame;      //Input
extern Share<uint16_t> highestTemp;     //Input

/*
const char* ssid = "ESP32";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

WebServer server(80);

uint16_t z = 0;

String SendHTML(String s){
  String ptr = "<!DOCTYPE html>\n";
ptr += "<html>\n";
ptr += "<body>\n";
ptr += "<h1>ME 507 EMBER</h1>\n";
ptr += "<p>Highest Temp: " + s + "</p>\n";
ptr += "</body>\n";
ptr += "</html>";
return ptr;
}

void handle_OnConnect() {
  highestTemp.get(z);
  server.send(200, "text/html", SendHTML(String(z))); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

void task_webserver (void* p)
{
  while(1)
  {
    server.handleClient ();
    vTaskDelay (500);
  }
}
*/
/** @brief   Task which sets up and runs a web server.
 *  @details After setup, function @c handleClient() must be run periodically
 *           to check for page requests from web clients. One could run this
 *           task as the lowest priority task with a short or no delay, as there
 *           generally isn't much rush in replying to web queries.
 *  @param   p_params Pointer to unused parameters
 */

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
  /*
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);
  
  server.begin();
  Serial.println("HTTP server started");
  */
  // Create the limit switch task
  xTaskCreate (limit_switch, "Limit Switch", 1024, NULL, 4, NULL);
  // Create the pitch motor task
  xTaskCreate (pitch_motor, "Pitch Motor", 1024, NULL, 5, NULL);
  // Create the solenoid task
  xTaskCreate (solenoid, "Solenoid", 4096, NULL, 6, NULL);
  // Create the thermal camera task
  xTaskCreate (thermal_camera, "Thermal Camera", 16000, NULL, 7, NULL);
  // Create the yaw motor task
  xTaskCreate (yaw_motor, "Yaw Motor", 1024, NULL, 8, NULL);
  // Task Webserver
  //xTaskCreate (task_webserver, "Web Server", 4096, NULL, 8, NULL);
  
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
