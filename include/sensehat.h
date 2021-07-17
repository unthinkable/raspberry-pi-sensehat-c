// =================================================================================================
//! @file sensehat.h
//! @author Gary Woodcock (gary.woodcock@unthinkable.com)
//! @brief This file contains public constants and function prototypes for the Raspberry Pi Sense
//! HAT C library.
//! @remarks Requires ANSI C99 (or better) compliant compilers.
//! @remarks This library requires Python 2.x/3.x or later.
//! @remarks Supported host operating systems: Raspbian Stretch or later.
//! @date 2019-09-25
//! @copyright Copyright (c) 2019 Unthinkable Research LLC. All rights reserved.
//! 
//  Includes
// =================================================================================================
#ifdef __cplusplus
    #pragma once
#endif

#ifndef __SENSEHAT_H__
#define __SENSEHAT_H__

#include "unthink_types.h"

// =================================================================================================
//  Types
// =================================================================================================

//! @brief An instance of the Raspberry Pi Sense HAT C library.
//!
//! An instance is required to invoke any of the Raspberry Pi Sense HAT library functions.
//!
typedef uint8_t* tSenseHAT_Instance;

//! @brief Rotation enumerations.
//! 
//! These are the four enumerations for rotation that can be used with the SenseHAT_LEDRotation 
//! function.
//! 
typedef enum
{
    eSenseHAT_LEDRotation0      = 0,    //!< No rotation; the default orientation. 
    eSenseHAT_LEDRotation90     = 90,   //!< A rotation of 90 degrees clockwise.
    eSenseHAT_LEDRotation180    = 180,  //!< A rotation of 180 degrees clockwise.
    eSenseHAT_LEDRotation270    = 270   //!< A rotation of 270 degrees clockwise.
}
tSenseHAT_LEDRotation;

//! @brief LED pixel.
//! 
//! This structure represents an LED pixel as a set of red, green and blue color components.
//! Each component must have a value between 0 and 255 (inclusive); other values are invalid.
//! 
typedef struct
{
    int32_t red;    //!< The value of the red color component of the LED pixel. 
    int32_t green;  //!< The value of the green color component of the LED pixel. 
    int32_t blue;   //!< The value of the blue color component of the LED pixel. 
}
tSenseHAT_LEDPixel;

//! @brief LED pixel array.
//! 
//! This array represents the 64 LEDs on the Sense HAT LED matrix. The indices of this array 
//! correspond to the locations of the matrix LEDs as follows:
//!  
//!     [ 0] [ 1] [ 2] [ 3] [ 4] [ 5] [ 6] [ 7] 
//!  
//!     [ 8] [ 9] [10] [11] [12] [13] [14] [15]
//! 
//!     [16] [17] [18] [19] [20] [21] [22] [23]
//! 
//!     [24] [25] [26] [27] [28] [29] [30] [31]
//! 
//!     [32] [33] [34] [35] [36] [37] [38] [39]
//! 
//!     [40] [41] [42] [43] [44] [45] [46] [47]
//! 
//!     [48] [49] [50] [51] [52] [53] [54] [55]
//! 
//!     [56] [57] [58] [59] [60] [61] [62] [63]
//! 
//! Note that in the default configuration (no rotation or flipping), the first row is the 
//! row of LEDs nearest the GPIO connector.
//! 
typedef tSenseHAT_LEDPixel tSenseHAT_LEDPixelArray[64];

//! @brief Orientation.
//!
//! This structure defines orientation in terms of pitch, roll, and yaw.
//!
typedef struct
{
    double  pitch;  //!< Pitch in degrees or radians.
    double  roll;   //!< Roll in degrees or radians.
    double  yaw;    //!< Yaw in degrees or radians.
}
tSenseHAT_Orientation;

//! @brief Raw data 
//!
//! This structure defines raw data along the x, y, and z axes.
//!
typedef struct
{
    double  x; //!< Value along the x axis.
    double  y; //!< Value along the y axis.
    double  z; //!< Value along the z axis.
}
tSenseHAT_RawData;

//! @brief Joystick direction enumerations.
//! 
//! These are the six enumerations for joystick direction that can be used with the 
//! SenseHAT_GetEvents and SenseHAT_WaitForEvent functions.
//! 
typedef enum
{
    eSenseHAT_JoystickDirectionNone     = 0,    //!< Invalid value. 
    eSenseHAT_JoystickDirectionUp       = 1,    //!< Joystick up. 
    eSenseHAT_JoystickDirectionDown     = 2,    //!< Joystick down. 
    eSenseHAT_JoystickDirectionLeft     = 3,    //!< Joystick left. 
    eSenseHAT_JoystickDirectionRight    = 4,    //!< Joystick right. 
    eSenseHAT_JoystickDirectionPush     = 5     //!< Joystick push. 
}
tSenseHAT_JoystickDirection;

//! @brief Joystick action enumerations.
//! 
//! These are the four enumerations for joystick actions that can be used with the 
//! SenseHAT_GetEvents and SenseHAT_WaitForEvent functions.
//! 
typedef enum
{
    eSenseHAT_JoystickActionNone        = 0,    //!< Invalid value. 
    eSenseHAT_JoystickActionPressed     = 1,    //!< Joystick pressed. 
    eSenseHAT_JoystickActionReleased    = 2,    //!< Joystick released. 
    eSenseHAT_JoystickActionHeld        = 3     //!< Joystick held. 
}
tSenseHAT_JoystickAction;

//! @brief Joystick event.
//! 
//! This structure defines a joystick event, which consists of a timestamp, a
//! direction, and an action.
//! 
typedef struct
{
    double                      timestamp;  //!< The time the event took place; expressed in fractional seconds. 
    tSenseHAT_JoystickDirection direction;  //!< The direction the joystick was moved. 
    tSenseHAT_JoystickAction    action;     //!< The action associated with the joystick movement. 
}
tSenseHAT_JoystickEvent;

// =================================================================================================
//  Prototypes
// =================================================================================================

#ifdef __cplusplus
extern "C"
{
#endif

    // =============================================================================================
    //  Base functions
    // =============================================================================================

    //! @brief Call SenseHAT_Version to get the version of the Sense HAT C library.
    //! 
    //! @return uint32_t The version of the Sense HAT C library. Note this is not the same as 
    //! the version of the underlying Sense HAT Python library. The value returned is formatted
    //! as follows:
    //!
    //! Bytes 0 and 1: Major version
    //!        Byte 2: Minor version
    //!        Byte 3: Revision
    //!
    //! For example, a value of 0x00010000 translates to a version of 1.0.0.
    //!
    uint32_t SenseHAT_Version (void);

    //! @brief Call SenseHAT_Open to create an instance of the Sense HAT C library.
    //! 
    //! All Sense HAT C functions require an instance as their first argument. This function 
    //! initializes an instance for use with the other Sense HAT C functions. Note that
    //! SenseHAT_Open must not be called multiple times in a single program.
    //! 
    //! @param[out] instance A pointer to an instance of the Sense HAT C library. This argument
    //! must not be NULL.
    //! @return int32_t A status code indicating whether the function call succeeded. A value equal 
    //! to 0 indicates success.
    //!
    int32_t SenseHAT_Open (tSenseHAT_Instance* instance);

    //! @brief Call SenseHAT_Close to close an instance of the Sense HAT C library.
    //! 
    //! This function is responsible for releasing all resources reserved by the Sense HAT C 
    //! library instance, including memory and references to Python functions.
    //! 
    //! @param[in] instance A pointer to an instance of the Sense HAT C library. This argument
    //! must not be NULL.
    //! @return int32_t A status code indicating whether the function call succeeded. A value equal 
    //! to 0 indicates success.
    //!
    int32_t SenseHAT_Close (tSenseHAT_Instance* instance);

    // =============================================================================================
    //  High level LED matrix functions
    // =============================================================================================

    //! @brief Call SenseHAT_LEDSetRotation to rotate the LED matrix.
    //!
    //! Use this function to adjust the orientation of the LED matrix in the event your Raspberry
    //! Pi is upside down or sideways.
    //!
    //! @param[in] instance An instance of the Sense HAT C library.
    //! @param[in] rotation The angle in degrees to rotate the LED matrix through. The only 
    //! valid values are those defined in the tSenseHAT_LEDRotation enumeration.
    //! @param[in] redraw Whether to redraw what is already being displayed on the LED matrix.
    //! @return int32_t A status code indicating whether the function call succeeded. A value equal 
    //! to 0 indicates success.
    //!
    int32_t SenseHAT_LEDSetRotation (const tSenseHAT_Instance instance,
                                     tSenseHAT_LEDRotation    rotation,
                                     bool                     redraw);

    //! @brief Call SenseHAT_LEDFlipHorizontal to flip the LED matrix horizontally.
    //! 
    //! @param[in] instance An instance of the Sense HAT C library.
    //! @param[in] redraw Whether to redraw what is already being displayed on the LED matrix.
    //! @param[out] pixels An LED pixel array containing the flipped pixels. If you don't need 
    //! this array, pass NULL in this argument.
    //! @return int32_t A status code indicating whether the function call succeeded. A value equal 
    //! to 0 indicates success.
    //!
    int32_t SenseHAT_LEDFlipHorizontal (const tSenseHAT_Instance instance,
                                        bool                     redraw,
                                        tSenseHAT_LEDPixelArray  pixels);

    //! @brief Call SenseHAT_LEDFlipVertical to flip the LED matrix vertically.
    //! 
    //! @param[in] instance An instance of the Sense HAT C library.
    //! @param[in] redraw Whether to redraw what is already being displayed on the LED matrix.
    //! @param[out] pixels An LED pixel array containing the flipped pixels. If you don't need 
    //! this array, pass NULL in this argument.
    //! @return int32_t A status code indicating whether the function call succeeded. A value equal 
    //! to 0 indicates success.
    //!
    int32_t SenseHAT_LEDFlipVertical (const tSenseHAT_Instance instance,
                                      bool                     redraw,
                                      tSenseHAT_LEDPixelArray  pixels);

    //! @brief Call SenseHAT_LEDSetPixels to set the color of every LED in the LED matrix.
    //! 
    //! @param[in] instance An instance of the Sense HAT C library.
    //! @param[in] pixels An array containing 64 LED pixels. If you pass NULL for this argument,
    //! then the LED matrix will be cleared (all LED pixels colors set to red:green:blue equal to
    //! 0:0:0 (off)).
    //! @return int32_t A status code indicating whether the function call succeeded. A value equal 
    //! to 0 indicates success.
    //!
    int32_t SenseHAT_LEDSetPixels (const tSenseHAT_Instance      instance,
                                   const tSenseHAT_LEDPixelArray pixels);

    //! @brief Call SenseHAT_LEDGetPixels to get the color of every LED in the display.
    //! 
    //! @param[in] instance An instance of the Sense HAT C library.
    //! @param[out] pixels An LED pixel array containing the current state of the pixels in the
    //! LED matrix. This argument must not be NULL.
    //! @return int32_t A status code indicating whether the function call succeeded. A value equal 
    //! to 0 indicates success.
    //!
    int32_t SenseHAT_LEDGetPixels (const tSenseHAT_Instance instance,
                                   tSenseHAT_LEDPixelArray  pixels);

    //! @brief Call SenseHAT_LEDSetPixel to set the color of a specific LED in the LED matrix.
    //! 
    //! @param[in] instance An instance of the Sense HAT C library.
    //! @param[in] xPosition X coordinate in the LED matrix. Valid values are between 0 and 7
    //! (inclusive); 0 is on the left, 7 is on the right.
    //! @param[in] yPosition Y coordinate in the LED matrix. Valid values are between 0 and 7
    //! (inclusive); 0 is at the top, 7 is on the bottom.
    //! @param[in] color LED pixel color. Pass NULL in this argument to set the LED pixel
    //! color to red:green:blue equal to 0:0:0 (off).
    //! @return int32_t A status code indicating whether the function call succeeded. A value equal 
    //! to 0 indicates success.
    //!
    int32_t SenseHAT_LEDSetPixel (const tSenseHAT_Instance  instance,
                                  int32_t                   xPosition,
                                  int32_t                   yPosition,
                                  const tSenseHAT_LEDPixel* color);

    //! @brief Call SenseHAT_LEDGetPixel to get the color of a specific LED in the display.
    //! 
    //! @param[in] instance An instance of the Sense HAT C library.
    //! @param[in] xPosition X coordinate in the LED matrix. Valid values are between 0 and 7
    //! (inclusive); 0 is on the left, 7 is on the right.
    //! @param[in] yPosition Y coordinate in the LED matrix. Valid values are between 0 and 7
    //! (inclusive); 0 is at the top, 7 is at the bottom.
    //! @param[out] color LED pixel color. This argument must not be NULL.
    //! @return int32_t A status code indicating whether the function call succeeded. A value equal 
    //! to 0 indicates success.
    //!
    int32_t SenseHAT_LEDGetPixel (const tSenseHAT_Instance instance,
                                  int32_t                  xPosition,
                                  int32_t                  yPosition,
                                  tSenseHAT_LEDPixel*      color);

    //! @brief Call SenseHAT_LEDLoadImage to display a 8x8 pixel images on the LED display.
    //! 
    //! @param[in] instance An instance of the Sense HAT C library.
    //! @param[in] imageFilePath File path to image file. The image must be 8x8 pixels in size.
    //! @param[in] redraw Whether to redraw what is already being displayed on the LED matrix.
    //! @param[out] pixels An LED pixel array containing the image. If you don't need this
    //! array, pass NULL in this argument.
    //! @return int32_t A status code indicating whether the function call succeeded. A value equal 
    //! to 0 indicates success.
    //!
    int32_t SenseHAT_LEDLoadImage (const tSenseHAT_Instance instance,
                                   const char*              imageFilePath,
                                   bool                     redraw,
                                   tSenseHAT_LEDPixelArray  pixels);

    //! @brief Call SenseHAT_LEDClear to reset all the pixels of the LED display to a specific color.
    //! 
    //! @param[in] instance An instance of the Sense HAT C library.
    //! @param[in] color LED color. Pass NULL in this argument to clear the LED matrix with 
    //! red:green:blue equal to 0:0:0 (off).
    //! @return int32_t A status code indicating whether the function call succeeded. A value equal 
    //! to 0 indicates success.
    //!
    int32_t SenseHAT_LEDClear (const tSenseHAT_Instance  instance,
                               const tSenseHAT_LEDPixel* color);

    //! @brief Call SenseHAT_LEDShowMessage to scroll a message from right to left on the LED 
    //! display at the specified speed, with the specified text and background colors.
    //! 
    //! @param[in] instance An instance of the Sense HAT C library.
    //! @param[in] message Message to show.
    //! @param[in] scrollSpeed Scroll speed in fractional seconds.
    //! @param[in] textColor Color of text. Pass NULL in this argument to use a text color of
    //! red:green:blue equal to 255:255:255 (white).
    //! @param[in] backColor Color of background. Pass NULL in this argument to use a back color
    //! of red:green:blue equal to 0:0:0 (off).
    //! @return int32_t A status code indicating whether the function call succeeded. A value equal 
    //! to 0 indicates success.
    //!
    int32_t SenseHAT_LEDShowMessage (const tSenseHAT_Instance  instance,
                                     const char*               message,
                                     double                    scrollSpeed,
                                     const tSenseHAT_LEDPixel* textColor,
                                     const tSenseHAT_LEDPixel* backColor);

    //! @brief Call SenseHAT_LEDShowLetter to show a letter on the LED display with the specified
    //! text and background colors.
    //! 
    //! @param[in] instance An instance of the Sense HAT C library.
    //! @param[in] letter Letter to show. This must be a single ASCII character - multiple
    //! characters are not allowed.
    //! @param[in] textColor Color of text. Pass NULL in this argument to use a text color of
    //! red:green:blue equal to 255:255:255 (white).
    //! @param[in] backColor Color of background. Pass NULL in this argument to use a back color
    //! of red:green:blue equal to 0:0:0 (off).
    //! @return int32_t A status code indicating whether the function call succeeded. A value equal 
    //! to 0 indicates success.
    //!
    int32_t SenseHAT_LEDShowLetter (const tSenseHAT_Instance  instance,
                                    const char*               letter,
                                    const tSenseHAT_LEDPixel* textColor,
                                    const tSenseHAT_LEDPixel* backColor);

    // =============================================================================================
    //  Low level LED matrix functions
    // =============================================================================================

    //! @brief Call SenseHAT_LEDGammaReset to reset the LED matrix gamma lookup table to
    //! its default configuration.
    //! 
    //! @param[in] instance An instance of the Sense HAT C library.
    //! @return int32_t A status code indicating whether the function call succeeded. A value equal 
    //! to 0 indicates success.
    //!
    int32_t SenseHAT_LEDGammaReset (const tSenseHAT_Instance instance);

    // =============================================================================================
    //  High level environmental functions
    // =============================================================================================

    //! @brief Call SenseHAT_GetHumidity to read the humidity in percent relative humidity.
    //! 
    //! @param[in] instance An instance of the Sense HAT C library.
    //! @param[out] percentRelativeHumidity Humidity in percent relative humidity. This argument
    //! must not be NULL.
    //! @return int32_t A status code indicating whether the function call succeeded. A value equal 
    //! to 0 indicates success.
    //!
    int32_t SenseHAT_GetHumidity (const tSenseHAT_Instance instance,
                                  double*                  percentRelativeHumidity);

    //! @brief Call SenseHAT_GetTemperature to read the temperature in degrees Celsius.
    //! 
    //! @param[in] instance An instance of the Sense HAT C library.
    //! @param[out] degreesCelsius Temperature in degrees Celsius. This argument must not be NULL.
    //! @return int32_t A status code indicating whether the function call succeeded. A value equal 
    //! to 0 indicates success.
    //!
    int32_t SenseHAT_GetTemperature (const tSenseHAT_Instance instance,
                                     double*                  degreesCelsius);

    //! @brief Call SenseHAT_GetPressure to read the pressure in millibars.
    //! 
    //! @param[in] instance An instance of the Sense HAT C library.
    //! @param[out] millibars Pressure in millibars. This argument must not be NULL.
    //! @return int32_t A status code indicating whether the function call succeeded. A value equal 
    //! to 0 indicates success.
    //!
    int32_t SenseHAT_GetPressure (const tSenseHAT_Instance instance,
                                  double*                  millibars);

    //! @brief Call SenseHAT_GetCompass to read the compass heading in degrees.
    //! 
    //! @param[in] instance An instance of the Sense HAT C library.
    //! @param[out] degrees Heading in degrees. This argument must not be NULL.
    //! @return int32_t A status code indicating whether the function call succeeded. A value equal 
    //! to 0 indicates success.
    //!
    int32_t SenseHAT_GetCompass (const tSenseHAT_Instance instance,
                                 double*                  degrees);

    // =============================================================================================
    //  Low level environmental functions
    // =============================================================================================

    //! @brief Call SenseHAT_GetAccelerometer to get the current orientation from the accelerometer
    //! only by disabling the magnetometer and the gyroscope.
    //! 
    //! @param[in] instance An instance of the Sense HAT C library.
    //! @param[out] orientation Orientation in degrees. This argument must not be NULL.
    //! @return int32_t A status code indicating whether the function call succeeded. A value equal
    //! to 0 indicates success.
    //!
    int32_t SenseHAT_GetAccelerometer (const tSenseHAT_Instance instance,
                                       tSenseHAT_Orientation*   orientation);

    //! @brief Call SenseHAT_GetAccelerometerRaw to get the raw x, y and z accelerometer data.
    //! 
    //! @param[in] instance An instance of the Sense HAT C library.
    //! @param[out] rawData Raw x, y and z accelerometer data in G's. This argument must not be
    //! NULL.
    //! @return int32_t A status code indicating whether the function call succeeded. A value equal
    //! to 0 indicates success.
    //!
    int32_t SenseHAT_GetAccelerometerRaw (const tSenseHAT_Instance instance,
                                          tSenseHAT_RawData*       rawData);

    //! @brief Call SenseHAT_GetCompassRaw to get the raw x, y and z magnetometer data.
    //! 
    //! @param[in] instance An instance of the Sense HAT C library.
    //! @param[out] rawData Raw x, y and z magnetometer data in microteslas (µT). This argument
    //! must not be NULL.
    //! @return int32_t A status code indicating whether the function call succeeded. A value equal
    //! to 0 indicates success.
    //!
    int32_t SenseHAT_GetCompassRaw (const tSenseHAT_Instance instance,
                                    tSenseHAT_RawData*       rawData);

    //! @brief Call SenseHAT_GetGyroscope to get the current orientation from the gyroscope
    //! only by disabling the magnetometer and the accelerometer.
    //! 
    //! @param[in] instance An instance of the Sense HAT C library.
    //! @param[out] orientation Orientation in degrees. This argument must not be NULL.
    //! @return int32_t A status code indicating whether the function call succeeded. A value equal
    //! to 0 indicates success.
    //!
    int32_t SenseHAT_GetGyroscope (const tSenseHAT_Instance instance,
                                   tSenseHAT_Orientation*   orientation);

    //! @brief Call SenseHAT_GetGyroscopeRaw to get the raw x, y, and z gyroscope data.
    //! 
    //! @param[in] instance An instance of the Sense HAT C library.
    //! @param[out] rawData Raw x, y and z gyroscope data in radians/second. This argument must
    //! not be NULL.
    //! @return int32_t A status code indicating whether the function call succeeded. A value equal
    //! to 0 indicates success.
    //!
    int32_t SenseHAT_GetGyroscopeRaw (const tSenseHAT_Instance instance,
                                      tSenseHAT_RawData*       rawData);

    //! @brief Call SenseHAT_GetOrientation to get the current orientation in degrees.
    //! 
    //! @param[in] instance An instance of the Sense HAT C library.
    //! @param[out] orientation Orientation in degrees. This argument must not be NULL.
    //! @return int32_t A status code indicating whether the function call succeeded. A value equal
    //! to 0 indicates success.
    //!
    int32_t SenseHAT_GetOrientation (const tSenseHAT_Instance instance,
                                     tSenseHAT_Orientation*   orientation);

    //! @brief Call SenseHAT_GetOrientationDegrees to get the current orientation in degrees
    //! using the aircraft principal axes of pitch, roll, and yaw.
    //! 
    //! @param[in] instance An instance of the Sense HAT C library.
    //! @param[out] orientation Orientation in degrees. This argument must not be NULL.
    //! @return int32_t A status code indicating whether the function call succeeded. A value equal
    //! to 0 indicates success.
    //!
    int32_t SenseHAT_GetOrientationDegrees (const tSenseHAT_Instance instance,
                                            tSenseHAT_Orientation*   orientation);

    //! @brief Call SenseHAT_GetOrientationRadians to get the current orientation in radians
    //! using the aircraft principal axes of pitch, roll, and yaw.
    //! 
    //! @param[in] instance An instance of the Sense HAT C library.
    //! @param[out] orientation Orientation in degrees. This argument must not be NULL.
    //! @return int32_t A status code indicating whether the function call succeeded. A value equal
    //! to 0 indicates success.
    //!
    int32_t SenseHAT_GetOrientationRadians (const tSenseHAT_Instance instance,
                                            tSenseHAT_Orientation*   orientation);

    //! @brief Call SenseHAT_GetTemperatureFromHumidity to read the temperature in degrees Celsius
    //! from the humidity sensor.
    //! 
    //! @param[in] instance An instance of the Sense HAT C library.
    //! @param[out] degreesCelsius Temperature in degrees Celsius. This argument must not be NULL.
    //! @return int32_t A status code indicating whether the function call succeeded. A value equal 
    //! to 0 indicates success.
    //!
    int32_t SenseHAT_GetTemperatureFromHumidity (const tSenseHAT_Instance instance,
                                                 double*                  degreesCelsius);

    //! @brief Call SenseHAT_GetTemperatureFromPressure to read the temperature in degrees Celsius
    //! from the pressure sensor.
    //! 
    //! @param[in] instance An instance of the Sense HAT C library.
    //! @param[out] degreesCelsius Temperature in degrees Celsius. This argument must not be NULL.
    //! @return int32_t A status code indicating whether the function call succeeded. A value equal 
    //! to 0 indicates success.
    //!
    int32_t SenseHAT_GetTemperatureFromPressure (const tSenseHAT_Instance instance,
                                                 double*                  degreesCelsius);

    //! @brief Call SenseHAT_SetIMUConfiguration to enable and/or disable the magnetometer,
    //! gyroscope, and accelerometer contributions to the "get orientation" functions.
    //!
    //! @param[in] instance An instance of the Sense HAT C library.
    //! @param[in] enableCompass Enable the magnetometer's contribution to orientation.
    //! @param[in] enableGyroscope Enable the gyroscope's contribution to orientation.
    //! @param[in] enableAccelerometer Enable the accelerometer's contribution to orientation.
    //! @return int32_t A status code indicating whether the function call succeeded. A value equal 
    //! to 0 indicates success.
    //!
    int32_t SenseHAT_SetIMUConfiguration (const tSenseHAT_Instance instance,
                                          bool                     enableCompass,
                                          bool                     enableGyroscope,
                                          bool                     enableAccelerometer);

    // =============================================================================================
    //  Event functions
    // =============================================================================================

    //! @brief Call SenseHAT_GetEvents to get the queue of events that have occurred since the
    //! last call to SenseHAT_GetEvents or SenseHAT_WaitForEvent.
    //! 
    //! @param[in] instance An instance of the Sense HAT C library.
    //! @param[out] eventCount Number of events in queue. This argument must not be NULL.
    //! @param[out] events Events in event queue.
    //! @return int32_t A status code indicating whether the function call succeeded. A value equal 
    //! to 0 indicates success.
    //!
    int32_t SenseHAT_GetEvents (const tSenseHAT_Instance  instance,
							    int32_t*                  eventCount,
                                tSenseHAT_JoystickEvent** events);

    //! @brief Call SenseHAT_WaitForEvent to block and wait for an event to occur.
    //! 
    //! @param[in] instance An instance of the Sense HAT C library.
    //! @param[in] flushPendingEvents Set to true to flush any pending events.
    //! @param[out] event The first event that occurs after making this function call.
    //! @return int32_t A status code indicating whether the function call succeeded. A value equal 
    //! to 0 indicates success.
    //! 
    int32_t SenseHAT_WaitForEvent (const tSenseHAT_Instance instance,
                                   bool                     flushPendingEvents,
                                   tSenseHAT_JoystickEvent* event);

#ifdef __cplusplus
}
#endif

// =================================================================================================
#endif	// __SENSEHAT_H__
// =================================================================================================
