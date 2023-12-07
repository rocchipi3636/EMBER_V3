/** @file task_thermal_camera.cpp
 *  This file contains the contents of task thermal_camera
 */

//includes

#include <task_thermal_camera.h>
#include <PrintStream.h>
#include <taskshare.h>
#include <Arduino.h>
#include <Adafruit_MLX90640.h>
#include <SPI.h>

//Define Shared Variables
//Details

extern Share<bool> firePresent;
extern Share<bool> fireCentered;
extern Share<uint8_t> firePosH;
extern Share<uint8_t> firePosV;

//Constants
uint8_t taskDelay = 5;

/** @brief Task
*   @details Task
*   @param p_params Pointer is NULL
*/
void thermal_camera (void* p_params)
{
    //State 0: Initialize
    Adafruit_MLX90640 mlx;
    float frame[32*24]; // buffer for full frame of temperatures
    Serial <<  "thermal_camera running";
    if (! mlx.begin(MLX90640_I2CADDR_DEFAULT, &Wire))
    {
        Serial.println("MLX90640 not found!");
        while (1) delay(10);
    }
    Serial.println("Found Adafruit MLX90640");
    mlx.setMode(MLX90640_CHESS);
    mlx.setResolution(MLX90640_ADC_18BIT);
    mlx.setRefreshRate(MLX90640_2_HZ);
    delay(2000);
    int z = 0 ;
    int i = 0;
    int j = 0;
    vTaskDelay(taskDelay);
    while(true)
    {
        //State 1: Read Thermal Camera
        if (mlx.getFrame(frame) != 0)
        {
            Serial.println("Failed");
            return;
        }
        for (uint8_t h=0; h<24; h++)
        {
            for (uint8_t w=0; w<32; w++)
            {
                int t = frame[h*32 + w];
                if (t >= z)
                {
                    z=t; 
                    i=h;
                    j=w;
                }
            } 
        }
        
        if(z >= 30)
        {
            firePresent.put(true);
            firePosH.put(j-12);
            firePosV.put(j);
        }
        else
        {
            firePresent.put(false);
            fireCentered.put(false);
        }
        Serial << "ThermCam: " << z << endl;
        vTaskDelay(taskDelay);    
    }
}
/*
      char c = '&';
      if (t < 20) c = ' ';
      else if (t < 23) c = '.';
      else if (t < 25) c = '-';
      else if (t < 27) c = '*';
      else if (t < 29) c = '+';
      else if (t < 31) c = 'x';
      else if (t < 33) c = '%';
      else if (t < 35) c = '#';
      else if (t < 37) c = 'X';
      else if (t < 40) c = '1';
      else if (t < 50) c = '2';
      else if (t < 60) c = '3';
      else if (t < 70) c = '4';
      else if (t < 80) c = '5';
      else if (t < 90) c = '6';
      else if (t < 100) c = '7';
      else if (t < 120) c = '8';
      else if (t < 200 ) c = '9';
*/