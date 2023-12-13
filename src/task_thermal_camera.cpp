/** @file task_thermal_camera.cpp
 *  This file contains the contents of task thermal_camera which reads and interprets data from the thermal camera
 */

//includes
#include <task_thermal_camera.h>
#include <PrintStream.h>
#include <taskshare.h>
#include <Arduino.h>
#include <Adafruit_MLX90640.h>
#include <SPI.h>

//Define Shared Variables
extern Share<bool> firePresent;         //Ouput
extern Share<bool> fireCentered;        //Output
extern Share<int8_t> firePosH;          //Output
extern Share<int8_t> firePosV;          //Output      
extern Share<uint16_t> highestTemp;     //Output
extern Share<float> globalThermalFrame;
extern Share<bool> newDataAvailable;
extern Share<bool> cameraConnected;     //Output

//Constants
uint8_t taskDelayTC = 16;

/** @brief Task thermal_camera reads and interprets data from the thermal camera
*   @details This task reads data from the thermal camera and determines a few things. If a fire is present, it sets
*           the shared variable "firePresent" true and additionally stores the pixel of highest intensity in "firePosH",
*           "firePosY", and "highestTemp". If a fire is not present, it sets "firePresent" and "fireCentered" false. Additionally
*           it always updates "printedFrame" and "cameraConnected" for the purposes of the webserver.
*   @param p_params Pointer is NULL
*/
void thermal_camera (void* p_params)
{
    //State 0: Initialize
    //Ensure that the fire variables and camera are initilially false to prevent false positives
    //before the camera is connected
    firePresent.put(false);
    fireCentered.put(false);
    cameraConnected.put(false);
    //Initialize thermal camera
    Adafruit_MLX90640 mlx;
    //buffer for full frame of temperatures
    float frame[32*24];
    // initliaze variables to record hottest pixel intensity and location
    int16_t currentHighTemp = 0 ;
    int8_t highTempX = 0;
    int8_t highTempY = 0;
    //If camera does not connect, halt camera task
    if (! mlx.begin(MLX90640_I2CADDR_DEFAULT, &Wire))
    {
        Serial.println("MLX90640 not found!");
        while (1)
        {
            //Put false into shared variable camera connected to alert other tasks of failed connection
            cameraConnected.put(false);
            vTaskDelay(100);
        }
    }
    //If camera does connect, put true to alert tasks of appropriate operation
    cameraConnected.put(true);
    Serial.println("Found Adafruit MLX90640");
    //Setting up camera settings
    //Output type matrix
    mlx.setMode(MLX90640_CHESS);
    //Output resolution
    mlx.setResolution(MLX90640_ADC_18BIT);
    //Refresh rate, in a task 4 HZ seems to be the limit, pretty abysmal :(
    mlx.setRefreshRate(MLX90640_4_HZ);
    //Delay to allow the settings to take effect
    vTaskDelay(1000);
    Serial <<  "thermal_camera running" << endl;
    vTaskDelay(taskDelayTC);
    while(true)
    {
        //State 1: Read Thermal Camera Continuously
        if (mlx.getFrame(frame) != 0)
        {
            Serial.println("Failed");
            return;
        }
        //Retrieve image from camera
        globalThermalFrame.put(*frame);
        //Iterate through results to find hottest pixel
        for (uint8_t height=0; height<24; height++)
        {
            for (uint8_t width=0; width<32; width++)
            {
                int currentPixelIntensity = frame[height*32 + width];
                //Check if current pixel is hottest
                if (currentPixelIntensity >= currentHighTemp)
                {
                    //If hottest, set variables
                    currentHighTemp=currentPixelIntensity;    //Intensity
                    highTempX=width-16;    //x location centered
                    highTempY=height;    //y location
                }
            } 
        }
        //Check for fire based on temperature
        if(currentHighTemp >= 80)
        {
            //If fire, set true and set location
            firePresent.put(true);
            firePosH.put(highTempX);
            firePosV.put(highTempY);
            //Reset temp reading
            currentHighTemp=0;
        }
        else
        {
            //If no fire, set false
            firePresent.put(false);
            fireCentered.put(false);
            //reset temp Reading
            currentHighTemp=0;
        }
        vTaskDelay(taskDelayTC);
    }
}