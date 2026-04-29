/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/

//include to use SQRT function
#include <cmath>

//include header for the ADXL345 class
#include "ADXL345.h"

//create a static instance of the ADXL345 class
ADXL345 accel;

void app_init(void)
{
  //**Step #1: Initialize the ADXL345**
    accel.initialize();

    //**Step #2: Perform a self-test to see if you can read
    //from the accelerometer. Device will not progress is self-test fails.
    bool success = accel.selfTest();

    //infinite loop if self-test fails
    while(!success){}
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{

    int16_t xReading = accel.getXAcceleration();
    int16_t yReading = accel.getYAcceleration();
    int16_t zReading = accel.getZAcceleration();

    int32_t magnitude = sqrt(xReading*xReading + yReading*yReading + zReading*zReading);

    float g_magnitude = magnitude / 256.0;

    int dummy = 0;

}
