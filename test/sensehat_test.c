// ==================================================================================================
//
//  sensehat_test.c
//
//  Copyright (c) 2019 Unthinkable Research LLC. All rights reserved.
//
//  Supported host operating systems:
//      Raspbian Stretch or later
//
//  Description:
//      This file contains unit tests for a C wrapper for the Raspberry Pi Sense HAT Python library.
//
//  Notes:
//      1)  Requires ANSI C99 (or better) compliant compilers.
//  
// =================================================================================================
//  Includes
// =================================================================================================
#include <CUnit.h>
#include <Automated.h>
#include <stdlib.h>
#include <unistd.h>
#include "sensehat.h"

// =================================================================================================
//  Globals
// =================================================================================================

static tSenseHAT_Instance gInstance = NULL;

// =================================================================================================
//  SenseHAT_SuiteInit
// =================================================================================================
int SenseHAT_SuiteInit (void)
{
    CU_ErrorCode status = CUE_SUCCESS;

    // Check version
    uint32_t version = SenseHAT_Version();
    if (version <= 0)
    {
        CU_FAIL_FATAL("SenseHAT_Version failed!");
        status = CUE_SINIT_FAILED;
    }

    if (status == CUE_SUCCESS)
    {
        if (gInstance == NULL)
        {
            int32_t result = SenseHAT_Open(&gInstance);
            if (result != UNTHINK_SUCCESS)
            {
                CU_FAIL_FATAL("SenseHAT_Open failed!");
                status = CUE_SINIT_FAILED;
            }
        }
        else    // Instance was not NULL
        {
            CU_FAIL_FATAL("Instance was not NULL!");
            status = CUE_SINIT_FAILED;
        }
    }
    return status;
}

// =================================================================================================
//  SenseHAT_SuiteCleanup
// =================================================================================================
int SenseHAT_SuiteCleanup (void)
{
    CU_ErrorCode status = CUE_SUCCESS;
    if (gInstance != NULL)
    {
        int32_t result = SenseHAT_Close(&gInstance);
        if (result != UNTHINK_SUCCESS)
        {
            CU_FAIL_FATAL("SenseHAT_Close failed!");
            status = CUE_SCLEAN_FAILED;
        }
        gInstance = NULL;
    }
    return status;
}

// =================================================================================================
//  TestLEDFunctions
// =================================================================================================
void TestLEDFunctions (void)
{
    tSenseHAT_LEDPixel clearColor = {0,0,0};
    tSenseHAT_LEDPixel redColor = {255,0,0};
    tSenseHAT_LEDPixel greenColor = {0,255,0};
    tSenseHAT_LEDPixel blueColor = {0,0,255};
    tSenseHAT_LEDPixel whiteColor = {255,255,255};
    tSenseHAT_LEDPixel badLowRedPixel = {-1,0,0};
    tSenseHAT_LEDPixel badHighRedPixel = {256,0,0};
    tSenseHAT_LEDPixel badLowGreenPixel = {0,-1,0};
    tSenseHAT_LEDPixel badHighGreenPixel = {0,256,0};
    tSenseHAT_LEDPixel badLowBluePixel = {0,0,-1};
    tSenseHAT_LEDPixel badHighBluePixel = {0,0,256};
    tSenseHAT_LEDPixel pixel = {0,0,0};
    tSenseHAT_LEDPixelArray pixels;
    uint16_t i = 0;
    uint16_t j = 0;
    int32_t result = UNTHINK_SUCCESS;

    // Test SenseHAT_LEDClear
    result = SenseHAT_LEDClear(gInstance, &clearColor);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    result = SenseHAT_LEDClear(gInstance, NULL);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    result = SenseHAT_LEDClear(NULL, &clearColor);
    CU_ASSERT_EQUAL(result, EINVAL);

    // Test SenseHAT_LEDSetPixel
    result = SenseHAT_LEDSetPixel(gInstance, 0, 0, &clearColor);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    result = SenseHAT_LEDSetPixel(gInstance, 0, 0, NULL);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    result = SenseHAT_LEDSetPixel(NULL, 0, 0, &redColor);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_LEDSetPixel(gInstance, -1, 0, &redColor);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_LEDSetPixel(gInstance, 8, 0, &redColor);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_LEDSetPixel(gInstance, 0, -1, &redColor);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_LEDSetPixel(gInstance, 0, 8, &redColor);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_LEDSetPixel(gInstance, 0, 0, &badLowRedPixel);
    CU_ASSERT_EQUAL(result, ENOTSUP);
    result = SenseHAT_LEDSetPixel(gInstance, 0, 0, &badHighRedPixel);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_LEDSetPixel(gInstance, 0, 0, &badLowGreenPixel);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_LEDSetPixel(gInstance, 0, 0, &badHighGreenPixel);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_LEDSetPixel(gInstance, 0, 0, &badLowBluePixel);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_LEDSetPixel(gInstance, 0, 0, &badHighBluePixel);
    CU_ASSERT_EQUAL(result, EINVAL);

    // Test SenseHAT_LEDGetPixel
    result = SenseHAT_LEDGetPixel(gInstance, 0, 0, &pixel);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    result = SenseHAT_LEDGetPixel(NULL, 0, 0, &pixel);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_LEDGetPixel(gInstance, -1, 0, &pixel);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_LEDGetPixel(gInstance, 8, 0, &pixel);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_LEDGetPixel(gInstance, 0, -1, &pixel);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_LEDGetPixel(gInstance, 0, 8, &pixel);
    CU_ASSERT_EQUAL(result, EINVAL);

    // Test LED matrix
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 8; j++)
        {
            // Set the pixel color
            result = SenseHAT_LEDSetPixel(gInstance, i, j, &redColor); 
            CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
            usleep(100000);

            // Get the pixel color
            pixel.red = pixel.green = pixel.blue = 0;
            result = SenseHAT_LEDGetPixel(gInstance, i, j, &pixel);
            CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);

            // Make sure the pixel color was set properly
            CU_ASSERT_EQUAL(pixel.red, (redColor.red & 0xF8));
            CU_ASSERT_EQUAL(pixel.green, (redColor.green & 0xFC));
            CU_ASSERT_EQUAL(pixel.blue, (redColor.blue & 0xF8));
        }
    }

    // Test SenseHAT_LEDGetPixels
    result = SenseHAT_LEDGetPixels(gInstance, pixels);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    result = SenseHAT_LEDGetPixels(NULL, pixels);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_LEDGetPixels(gInstance, NULL);
    CU_ASSERT_EQUAL(result, EINVAL);

    // Use SenseHAT_LEDGetPixels to check state of LED pixels 
    result = SenseHAT_LEDGetPixels(gInstance, pixels);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    for (i = 0; i < 64; i++)
    {
        CU_ASSERT_EQUAL(pixels[i].red, (redColor.red & 0xF8));
        CU_ASSERT_EQUAL(pixels[i].green, (redColor.green & 0xFC));
        CU_ASSERT_EQUAL(pixels[i].blue, (redColor.blue & 0xF8));
    }

    // Test SenseHAT_LEDSetPixels
    for (i = 0; i < 64; i++)
    {
        pixels[i].red =
            pixels[i].green =
            pixels[i].blue = 255;
    }
    result = SenseHAT_LEDSetPixels(gInstance, NULL);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    usleep(500000);
    result = SenseHAT_LEDSetPixels(gInstance, pixels);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    usleep(500000);
    result = SenseHAT_LEDSetPixels(NULL, pixels);
    CU_ASSERT_EQUAL(result, EINVAL);
    pixels[1].red = -1;
    result = SenseHAT_LEDSetPixels(gInstance, pixels);
    CU_ASSERT_EQUAL(result, EINVAL);
    pixels[1].red = 256;
    result = SenseHAT_LEDSetPixels(gInstance, pixels);
    CU_ASSERT_EQUAL(result, EINVAL);
    pixels[1].red = 0;
    pixels[1].green = -1;
    result = SenseHAT_LEDSetPixels(gInstance, pixels);
    CU_ASSERT_EQUAL(result, EINVAL);
    pixels[1].green = 256;
    result = SenseHAT_LEDSetPixels(gInstance, pixels);
    CU_ASSERT_EQUAL(result, EINVAL);
    pixels[1].green = 0;
    pixels[1].blue = -1;
    result = SenseHAT_LEDSetPixels(gInstance, pixels);
    CU_ASSERT_EQUAL(result, EINVAL);
    pixels[1].blue = 256;
    result = SenseHAT_LEDSetPixels(gInstance, pixels);
    CU_ASSERT_EQUAL(result, EINVAL);

    // Test SenseHAT_LEDShowLetter
    result = SenseHAT_LEDShowLetter(gInstance, "1", &redColor, &clearColor);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    sleep(1);
    result = SenseHAT_LEDShowLetter(gInstance, "2", &greenColor, NULL);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    sleep(1);
    result = SenseHAT_LEDShowLetter(gInstance, "3", NULL, &clearColor);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    sleep(1);
    result = SenseHAT_LEDShowLetter(gInstance, "4", NULL, NULL);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    sleep(1);
    result = SenseHAT_LEDShowLetter(NULL, "0", &redColor, &clearColor);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_LEDShowLetter(gInstance, NULL, &redColor, &clearColor);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_LEDShowLetter(gInstance, "0", &badLowRedPixel, &clearColor);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_LEDShowLetter(gInstance, "0", &badHighRedPixel, &clearColor);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_LEDShowLetter(gInstance, "0", &badLowGreenPixel, &clearColor);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_LEDShowLetter(gInstance, "0", &badHighGreenPixel, &clearColor);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_LEDShowLetter(gInstance, "0", &badLowBluePixel, &clearColor);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_LEDShowLetter(gInstance, "0", &badHighBluePixel, &clearColor);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_LEDShowLetter(gInstance, "0", &whiteColor, &badLowRedPixel);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_LEDShowLetter(gInstance, "0", &whiteColor, &badHighRedPixel);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_LEDShowLetter(gInstance, "0", &whiteColor, &badLowGreenPixel);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_LEDShowLetter(gInstance, "0", &whiteColor, &badHighGreenPixel);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_LEDShowLetter(gInstance, "0", &whiteColor, &badLowBluePixel);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_LEDShowLetter(gInstance, "0", &whiteColor, &badHighBluePixel);
    CU_ASSERT_EQUAL(result, EINVAL);

    // Test SenseHAT_LEDFlipHorizontal
    result = SenseHAT_LEDFlipHorizontal(gInstance, false, NULL);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    result = SenseHAT_LEDFlipHorizontal(gInstance, true, NULL);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    sleep(1);
    result = SenseHAT_LEDFlipHorizontal(gInstance, true, pixels);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    sleep(1);
    result = SenseHAT_LEDFlipHorizontal(NULL, false, NULL);
    CU_ASSERT_EQUAL(result, EINVAL);

    // Test SenseHAT_LEDFlipVertical
    result = SenseHAT_LEDFlipVertical(gInstance, false, NULL);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    result = SenseHAT_LEDFlipVertical(gInstance, true, NULL);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    sleep(1);
    result = SenseHAT_LEDFlipVertical(gInstance, true, pixels);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    sleep(1);
    result = SenseHAT_LEDFlipVertical(NULL, false, NULL);
    CU_ASSERT_EQUAL(result, EINVAL);

    // Test SenseHAT_LEDSetRotation
    result = SenseHAT_LEDSetRotation(gInstance, eSenseHAT_LEDRotation90, true);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    result = SenseHAT_LEDSetRotation(gInstance, eSenseHAT_LEDRotation90, false);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    sleep(1);
    result = SenseHAT_LEDSetRotation(NULL, eSenseHAT_LEDRotation90, true);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_LEDSetRotation(gInstance, eSenseHAT_LEDRotation180, true);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    sleep(1);
    result = SenseHAT_LEDSetRotation(gInstance, eSenseHAT_LEDRotation180, false);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    result = SenseHAT_LEDSetRotation(gInstance, eSenseHAT_LEDRotation270, true);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    sleep(1);
    result = SenseHAT_LEDSetRotation(gInstance, eSenseHAT_LEDRotation270, false);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);   
    result = SenseHAT_LEDSetRotation(gInstance, eSenseHAT_LEDRotation0, true);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    sleep(1);
    result = SenseHAT_LEDSetRotation(gInstance, eSenseHAT_LEDRotation0, false);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    result = SenseHAT_LEDClear(gInstance, &clearColor);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);

    // Test SenseHAT_ShowMessage
    result = SenseHAT_LEDShowMessage(gInstance, "Test 1", 0.05, &redColor, &clearColor);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    sleep(1);
    result = SenseHAT_LEDShowMessage(gInstance, "Test 2", 0.05, NULL, &clearColor);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    sleep(1);
    result = SenseHAT_LEDShowMessage(gInstance, "Test 3", 0.05, &greenColor, NULL);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    sleep(1);
    result = SenseHAT_LEDShowMessage(gInstance, "Test 4", 0.05, NULL, NULL);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    sleep(1);
    result = SenseHAT_LEDShowMessage(gInstance, "Test 5", 0.0, NULL, NULL);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    sleep(1);
    result = SenseHAT_LEDShowMessage(NULL, "Test", 0.05, &blueColor, &clearColor);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_LEDShowMessage(gInstance, NULL, 0.05, &blueColor, &clearColor);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_LEDShowMessage(gInstance, "Test", -0.05, NULL, NULL);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_LEDShowMessage(gInstance, "Test", 0.05, &badLowRedPixel, &clearColor);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_LEDShowMessage(gInstance, "Test", 0.05, &badHighRedPixel, &clearColor);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_LEDShowMessage(gInstance, "Test", 0.05, &badLowGreenPixel, &clearColor);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_LEDShowMessage(gInstance, "Test", 0.05, &badHighGreenPixel, &clearColor);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_LEDShowMessage(gInstance, "Test", 0.05, &badLowBluePixel, &clearColor);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_LEDShowMessage(gInstance, "Test", 0.05, &badHighBluePixel, &clearColor);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_LEDShowMessage(gInstance, "Test", 0.05, &whiteColor, &badLowRedPixel);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_LEDShowMessage(gInstance, "Test", 0.05, &whiteColor, &badHighRedPixel);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_LEDShowMessage(gInstance, "Test", 0.05, &whiteColor, &badLowGreenPixel);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_LEDShowMessage(gInstance, "Test", 0.05, &whiteColor, &badHighGreenPixel);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_LEDShowMessage(gInstance, "Test", 0.05, &whiteColor, &badLowBluePixel);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_LEDShowMessage(gInstance, "Test", 0.05, &whiteColor, &badHighBluePixel);
    CU_ASSERT_EQUAL(result, EINVAL);

    // Test SenseHAT_LEDLoadImage
    result = SenseHAT_LEDLoadImage(gInstance, "../../../../example/space_invader.png", false, pixels);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    result = SenseHAT_LEDLoadImage(gInstance, "../../../../example/space_invader.png", true, pixels);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    result = SenseHAT_LEDLoadImage(gInstance, "../../../../example/space_invader.png", true, NULL);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    sleep(1);
    result = SenseHAT_LEDLoadImage(NULL, "../../../../example/space_invader.png", true, NULL);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_LEDLoadImage(gInstance, "", true, NULL);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_LEDLoadImage(gInstance, NULL, true, NULL);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_LEDLoadImage(gInstance, "./file_does_not_exist.png", true, NULL);
    CU_ASSERT_EQUAL(result, ENOENT);

    // Test SenseHAT_LEDGammaReset
    result = SenseHAT_LEDGammaReset(gInstance);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    result = SenseHAT_LEDGammaReset(NULL);
    CU_ASSERT_EQUAL(result, EINVAL);
        
    // Clear LED matrix
    result = SenseHAT_LEDClear(gInstance, &clearColor);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);

    return;
}

// =================================================================================================
//  TestEnvironmentalFunctions
// =================================================================================================
void TestEnvironmentalFunctions (void)
{
    double humidity = 0.0;
    double temperature = 0.0;
    double pressure = 0.0;
    double heading = 0.0;
    tSenseHAT_Orientation orientation;
    tSenseHAT_RawData rawData;
    int32_t result = UNTHINK_SUCCESS;

    // High level functions
    result = SenseHAT_GetHumidity(gInstance, &humidity);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    result = SenseHAT_GetHumidity(NULL, &humidity);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_GetHumidity(gInstance, NULL);
    CU_ASSERT_EQUAL(result, EINVAL);

    result = SenseHAT_GetTemperature(gInstance, &temperature);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    result = SenseHAT_GetTemperature(NULL, &temperature);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_GetTemperature(gInstance, NULL);
    CU_ASSERT_EQUAL(result, EINVAL);

    result = SenseHAT_GetPressure(gInstance, &pressure);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    result = SenseHAT_GetPressure(NULL, &pressure);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_GetPressure(gInstance, NULL);
    CU_ASSERT_EQUAL(result, EINVAL);

    result = SenseHAT_GetCompass(gInstance, &heading);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    result = SenseHAT_GetCompass(NULL, &heading);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_GetCompass(gInstance, NULL);
    CU_ASSERT_EQUAL(result, EINVAL);

    // Low level functions
    result = SenseHAT_GetAccelerometer(gInstance, &orientation);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    result = SenseHAT_GetAccelerometer(NULL, &orientation);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_GetAccelerometer(gInstance, NULL);
    CU_ASSERT_EQUAL(result, EINVAL);
    
    result = SenseHAT_GetAccelerometerRaw(gInstance, &rawData);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    result = SenseHAT_GetAccelerometerRaw(NULL, &rawData);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_GetAccelerometerRaw(gInstance, NULL);
    CU_ASSERT_EQUAL(result, EINVAL);

    result = SenseHAT_GetCompassRaw(gInstance, &rawData);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    result = SenseHAT_GetCompassRaw(NULL, &rawData);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_GetCompassRaw(gInstance, NULL);
    CU_ASSERT_EQUAL(result, EINVAL);

    result = SenseHAT_GetGyroscope(gInstance, &orientation);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    result = SenseHAT_GetGyroscope(NULL, &orientation);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_GetGyroscope(gInstance, NULL);
    CU_ASSERT_EQUAL(result, EINVAL);

    result = SenseHAT_GetGyroscopeRaw(gInstance, &rawData);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    result = SenseHAT_GetGyroscopeRaw(NULL, &rawData);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_GetGyroscopeRaw(gInstance, NULL);
    CU_ASSERT_EQUAL(result, EINVAL);

    result = SenseHAT_GetOrientation(gInstance, &orientation);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    result = SenseHAT_GetOrientation(NULL, &orientation);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_GetOrientation(gInstance, NULL);
    CU_ASSERT_EQUAL(result, EINVAL);

    result = SenseHAT_GetOrientationDegrees(gInstance, &orientation);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    result = SenseHAT_GetOrientationDegrees(NULL, &orientation);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_GetOrientationDegrees(gInstance, NULL);
    CU_ASSERT_EQUAL(result, EINVAL);

    result = SenseHAT_GetOrientationRadians(gInstance, &orientation);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    result = SenseHAT_GetOrientationRadians(NULL, &orientation);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_GetOrientationRadians(gInstance, NULL);
    CU_ASSERT_EQUAL(result, EINVAL);

    result = SenseHAT_GetTemperatureFromHumidity(gInstance, &temperature);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    result = SenseHAT_GetTemperatureFromHumidity(NULL, &temperature);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_GetTemperatureFromHumidity(gInstance, NULL);
    CU_ASSERT_EQUAL(result, EINVAL);

    result = SenseHAT_GetTemperatureFromPressure(gInstance, &temperature);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    result = SenseHAT_GetTemperatureFromPressure(NULL, &pressure);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_GetTemperatureFromPressure(gInstance, NULL);
    CU_ASSERT_EQUAL(result, EINVAL);

    result = SenseHAT_SetIMUConfiguration(gInstance, true, true, true);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    result = SenseHAT_SetIMUConfiguration(gInstance, true, true, false);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    result = SenseHAT_SetIMUConfiguration(gInstance, true, false, false);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    result = SenseHAT_SetIMUConfiguration(gInstance, false, false, false);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    result = SenseHAT_SetIMUConfiguration(gInstance, false, false, true);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    result = SenseHAT_SetIMUConfiguration(gInstance, false, true, true);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    result = SenseHAT_SetIMUConfiguration(gInstance, false, true, false);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    result = SenseHAT_SetIMUConfiguration(gInstance, true, false, true);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);

    return;
}

// =================================================================================================
//  TestEventFunctions
// =================================================================================================
void TestEventFunctions (void)
{
    int32_t result = UNTHINK_SUCCESS;
    int32_t count = 0;
    tSenseHAT_JoystickEvent event;
    tSenseHAT_JoystickEvent* events = NULL;

    // Test SenseHAT_GetEvents
    result = SenseHAT_GetEvents(gInstance, &count, &events);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    if (events != NULL)
    {
        free((void*)events);
        events = NULL;
    }
    result = SenseHAT_GetEvents(gInstance, &count, NULL);
    CU_ASSERT_EQUAL(result, UNTHINK_SUCCESS);
    result = SenseHAT_GetEvents(NULL, &count, &events);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_GetEvents(gInstance, NULL, &events);
    CU_ASSERT_EQUAL(result, EINVAL);

    // Test SenseHAT_WaitForEvent
    result = SenseHAT_WaitForEvent(NULL, true, &event);
    CU_ASSERT_EQUAL(result, EINVAL);
    result = SenseHAT_WaitForEvent(gInstance, false, NULL);
    CU_ASSERT_EQUAL(result, EINVAL);

    return;
}

// =================================================================================================
//  main
// =================================================================================================
int main (int argc, const char * argv[])
{
    (void)argc;
    (void)argv;

    // Initialize CUnit
    CU_ErrorCode status = CU_initialize_registry();
    if (status == CUE_SUCCESS)
    {
        printf("Sense HAT C library unit test has started.\n");

        // Set up test suite
        CU_pSuite senseHATTestSuite = CU_add_suite("Sense HAT C library test suite",
                                                   SenseHAT_SuiteInit,
                                                   SenseHAT_SuiteCleanup);
        if (senseHATTestSuite != NULL)
        {
            CU_ADD_TEST(senseHATTestSuite, TestLEDFunctions);
            CU_ADD_TEST(senseHATTestSuite, TestEnvironmentalFunctions);
            CU_ADD_TEST(senseHATTestSuite, TestEventFunctions);
        }
        else    // CU_add_suite failed
        {
            status = CU_get_error();
            printf("CU_add_suite failed with error code %d!\n", status);
        }

        // Check status
        if (status == CUE_SUCCESS)
        {
            CU_set_output_filename("./Sense-HAT-C-Library-Unit-Test");
            CU_automated_run_tests();
        }

        // Clean up
        CU_cleanup_registry();
        printf("Sense HAT C library unit test has completed.\n");
    }
    else    // CU_initialize_registry failed
    {
        printf("CU_initialize_registry failed with error code %d!\n", status);
    }
    return 0;
}

// =================================================================================================
