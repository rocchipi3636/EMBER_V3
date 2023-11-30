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
extern Share<uint8_t> firePosH;
extern Share<uint8_t> firePosV;


/** @brief Task
*   @details Task
*   @param p_params Pointer is NULL
*/
void thermal_camera (void* p_params)
{
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
    while(true)
    {
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
        vTaskDelay(5);
        if(z >= 30)
        {
            firePresent.put(true);
            firePosH.put(j-12);
            firePosV.put(j);
        }    
    }
}