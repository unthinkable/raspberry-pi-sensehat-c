// ==================================================================================================
//
//  sensehat.c
//
//  Copyright (c) 2019 Unthinkable Research LLC. All rights reserved.
//
//  Supported host operating systems:
//      Raspbian Stretch or later
//
//  Description:
//      This file contains private constants, function prototypes and function implementations
//      for the Raspberry Pi Sense HAT C library.
//
//  Notes:
//      1)  Requires ANSI C99 (or better) compliant compilers.
//      2)  This library requires Python 2.x/3.x or later.
//  
// =================================================================================================
//! @file sensehat.c
//! @author Gary Woodcock (gary.woodcock@unthinkable.com)
//! @brief This file contains private constants, function prototypes and function implementations
//! for the Raspberry Pi Sense HAT C library.
//! @date 2019-09-25
//! @copyright Copyright (c) 2019 Unthinkable Research LLC. All rights reserved.
//! 
//  Includes
// =================================================================================================
#include "sensehat.h"
#include "python-support.h"
#include <memory.h>
#include <math.h>
#include <stdlib.h>

// =================================================================================================
//  Constants
// =================================================================================================

// Version
static const uint32_t kSenseHAT_Version = 0x00000100;   // 0.1.0

// Python module name
static const char* kSenseHAT_ModuleName = "sense_hat";

// Sense HAT submodule
static const char* kSenseHAT_SubmoduleName  = "SenseHat";

// Sense HAT submodule function names
static const char* kClearFunctionName                       = "clear";
static const char* kFlipHorizontalFunctionName              = "flip_h";
static const char* kFlipVerticalFunctionName                = "flip_v";
//static const char* kGammaFunctionName                       = "gamma";                          // Data descriptor; not exposed
static const char* kGammaResetFunctionName                  = "gamma_reset";                    
static const char* kGetAccelerometerFunctionName            = "get_accelerometer";              
static const char* kGetAccelerometerRawFunctionName         = "get_accelerometer_raw";
static const char* kGetCompassFunctionName                  = "get_compass";
static const char* kGetCompassRawFunctionName               = "get_compass_raw";                
static const char* kGetGyroscopeFunctionName                = "get_gyroscope";                  
static const char* kGetGyroscopeRawFunctionName             = "get_gyroscope_raw";              
static const char* kGetHumidityFunctionName                 = "get_humidity";
static const char* kGetOrientationFunctionName              = "get_orientation";                
static const char* kGetOrientationDegreesFunctionName       = "get_orientation_degrees";        
static const char* kGetOrientationRadiansFunctionName       = "get_orientation_radians";        
static const char* kGetPixelFunctionName                    = "get_pixel";
static const char* kGetPixelsFunctionName                   = "get_pixels";
static const char* kGetPressureFunctionName                 = "get_pressure";
static const char* kGetTemperatureFunctionName              = "get_temperature";
static const char* kGetTemperatureFromHumidityFunctionName  = "get_temperature_from_humidity";  
static const char* kGetTemperatureFromPressureFunctionName  = "get_temperature_from_pressure";  
static const char* kLoadImageFunctionName                   = "load_image";
//static const char* kLowLightFunctionName                    = "low_light";                      // Data descriptor; not exposed
static const char* kSetIMUConfigFunctionName                = "set_imu_config";                 
static const char* kSetPixelFunctionName                    = "set_pixel";
static const char* kSetPixelsFunctionName                   = "set_pixels";
static const char* kSetRotationFunctionName                 = "set_rotation";
static const char* kShowLetterFunctionName                  = "show_letter";
static const char* kShowMessageFunctionName                 = "show_message";

// Orientation components
static const char* kOrientationPitch    = "pitch";
static const char* kOrientationRoll     = "roll";
static const char* kOrientationYaw      = "yaw";

// Raw components
static const char* kRawX    = "x";
static const char* kRawY    = "y";
static const char* kRawZ    = "z";

// Stick submodule
static const char* kStickSubmoduleName  = "stick";

// Stick submodule function names
static const char* kGetEventsFunctionName       = "get_events";
static const char* kWaitForEventFunctionName    = "wait_for_event";

// =================================================================================================
//  Types
// =================================================================================================

//! @brief Private instance data.
//! 
//! This structure represents the private instance data required by the Raspberry Pi Sense HAT
//! C library.
//! 
typedef struct
{
    PyObject*   senseHATModule;                     //!< Top level Python module reference. 
    PyObject*   self;                               //!< Python object instance.

    PyObject*   senseHATSubModule;                  //!< Sense HAT Python submodule reference. 
    PyObject*   clearFunction;                      //!< clear Python function reference. 
    PyObject*   flipHorizontalFunction;             //!< flip_h Python function reference.
    PyObject*   flipVerticalFunction;               //!< flip_v Python function reference.
    PyObject*   gammaResetFunction;                 //!< gamma_reset Python function reference.
    PyObject*   getAccelerometerFunction;           //!< get_accelerometer Python function reference.
    PyObject*   getAccelerometerRawFunction;        //!< get_accelerometer_raw Python function reference.
    PyObject*   getCompassFunction;                 //!< get_compass Python function reference.
    PyObject*   getCompassRawFunction;              //!< get_compass_raw Python function reference.
    PyObject*   getGyroscopeFunction;               //!< get_gyroscope Python function reference.
    PyObject*   getGyroscopeRawFunction;            //!< get_gyroscope_raw Python function reference.
    PyObject*   getHumidityFunction;                //!< get_humidity Python function reference.
    PyObject*   getOrientationFunction;             //!< get_orientation Python function reference.
    PyObject*   getOrientationDegreesFunction;      //!< get_orientation_degrees Python function reference.
    PyObject*   getOrientationRadiansFunction;      //!< get_orientation_radians Python function reference.
    PyObject*   getPixelFunction;                   //!< get_pixel Python function reference.
    PyObject*   getPixelsFunction;                  //!< get_pixels Python function reference.
    PyObject*   getPressureFunction;                //!< get_pressure Python function reference.
    PyObject*   getTemperatureFunction;             //!< get_temperature Python function reference.
    PyObject*   getTemperatureFromHumidityFunction; //!< get_temperature_from_humidity Python function reference.
    PyObject*   getTemperatureFromPressureFunction; //!< get_temperature_from_pressure Python function reference.
    PyObject*   loadImageFunction;                  //!< load_image Python function reference.
    PyObject*   setIMUConfigFunction;               //!< set_imu_config Python function reference.
    PyObject*   setPixelFunction;                   //!< set_pixel Python function reference.
    PyObject*   setPixelsFunction;                  //!< set_pixels Python function reference.
    PyObject*   setRotationFunction;                //!< set_rotation Python function reference.
    PyObject*   showLetterFunction;                 //!< show_letter Python function reference.
    PyObject*   showMessageFunction;                //!< show_message Python function reference.

    PyObject*   stickSubModule;                     //!< Joystick Python submodule reference.
    PyObject*   getEventsFunction;                  //!< get_events Python function reference.
    PyObject*   waitForEventFunction;               //!< wait_for_event Python function reference.
}
tSenseHAT_InstancePrivate;

// =================================================================================================
//  Private prototypes
// =================================================================================================

// SenseHAT_ConvertPixelToLEDPixel
static int32_t SenseHAT_ConvertPixelToLEDPixel (const PyObject* pixel,
                                                tSenseHAT_LEDPixel* color);

// SenseHAT_ConvertPixelListToLEDPixelArray
static int32_t SenseHAT_ConvertPixelListToLEDPixelArray (const PyObject* pixelList,
                                                         tSenseHAT_LEDPixelArray pixelArray);

// SenseHAT_ConvertDictToOrientation
static int32_t SenseHAT_ConvertDictToOrientation (const PyObject* dict,
                                                  tSenseHAT_Orientation* orientation);
                                                  
// SenseHAT_ConvertDictToRawData
static int32_t SenseHAT_ConvertDictToRawData (const PyObject* dict,
                                              tSenseHAT_RawData* rawData);

// SenseHAT_ParseJoystickEvent
static int32_t SenseHAT_ParseJoystickEvent (const PyObject* tuple,
                                            tSenseHAT_JoystickEvent* event);

// SenseHAT_Release
static int32_t SenseHAT_Release (tSenseHAT_InstancePrivate* instancePrivate);

// =================================================================================================
//  SenseHAT_Version
// =================================================================================================
uint32_t SenseHAT_Version (void)
{
    // Return the version of the Raspberry Pi Sense HAT C library
    return kSenseHAT_Version;
}

// =================================================================================================
//  SenseHAT_Open
// =================================================================================================
int32_t SenseHAT_Open (tSenseHAT_Instance* instance)
{
    int32_t result = 0;

    // Check arguments
    if (instance != NULL)
    {
        // Setup
        *instance = NULL;

        // Allocate space
        tSenseHAT_InstancePrivate* instancePrivate =
            (tSenseHAT_InstancePrivate*)malloc(sizeof(tSenseHAT_InstancePrivate));
        if (instancePrivate != NULL)
        {
            // Initialize memory
            memset(instancePrivate, 0, sizeof(tSenseHAT_InstancePrivate));

            // Initialize
            Py_Initialize();

            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Get the Sense HAT module name
            PyObject* pName = PyUnicode_FromString(kSenseHAT_ModuleName);
            if (pName != NULL)
            {
                // Import the module
                instancePrivate->senseHATModule = PyImport_Import(pName);
                if (instancePrivate->senseHATModule != NULL) 
                {
                     // Get a reference to the Sense HAT submodule
                    instancePrivate->senseHATSubModule = 
                        PyObject_GetAttrString (instancePrivate->senseHATModule, kSenseHAT_SubmoduleName);
                    if (instancePrivate->senseHATSubModule != NULL)
                    {
                        // Initialize the submodule
                        instancePrivate->self = PyObject_CallObject(instancePrivate->senseHATSubModule, NULL);
                        if (instancePrivate->self != NULL)
                        { 
                            // Get a reference to the clear function
                            result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                 kClearFunctionName,
                                                                 &(instancePrivate->clearFunction));

                            // Check for success
                            if (result == 0) 
                            {
                                // Get a reference to the flip horizontal function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kFlipHorizontalFunctionName,
                                                                     &(instancePrivate->flipHorizontalFunction));
                            }

                            // Check for success
                            if (result == 0) 
                            {
                                // Get a reference to the flip vertical function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kFlipVerticalFunctionName,
                                                                     &(instancePrivate->flipVerticalFunction));
                            }

                            // Check for success
                            if (result == 0)
                            {
                                // Get a reference to the gamma reset function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kGammaResetFunctionName,
                                                                     &(instancePrivate->gammaResetFunction));
                            }

                            // Check for success
                            if (result == 0)
                            {
                                // Get a reference to the get accelerometer function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kGetAccelerometerFunctionName,
                                                                     &(instancePrivate->getAccelerometerFunction));
                            }

                            // Check for success
                            if (result == 0)
                            {
                                // Get a reference to the get accelerometer raw function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kGetAccelerometerRawFunctionName,
                                                                     &(instancePrivate->getAccelerometerRawFunction));
                            }

                            // Check for success
                            if (result == 0) 
                            {
                                // Get a reference to the get compass function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kGetCompassFunctionName,
                                                                     &(instancePrivate->getCompassFunction));
                            }
        
                            // Check for success
                            if (result == 0) 
                            {
                                // Get a reference to the get compass raw function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kGetCompassRawFunctionName,
                                                                     &(instancePrivate->getCompassRawFunction));
                            }
        
                            // Check for success
                            if (result == 0) 
                            {
                                // Get a reference to the get gyroscope function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kGetGyroscopeFunctionName,
                                                                     &(instancePrivate->getGyroscopeFunction));
                            }
        
                            // Check for success
                            if (result == 0) 
                            {
                                // Get a reference to the get gyroscope raw function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kGetGyroscopeRawFunctionName,
                                                                     &(instancePrivate->getGyroscopeRawFunction));
                            }
        
                            // Check for success
                            if (result == 0) 
                            {
                                // Get a reference to the get humidity function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kGetHumidityFunctionName,
                                                                     &(instancePrivate->getHumidityFunction));
                            }

                            // Check for success
                            if (result == 0) 
                            {
                                // Get a reference to the get orientation function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kGetOrientationFunctionName,
                                                                     &(instancePrivate->getOrientationFunction));
                            }

                            // Check for success
                            if (result == 0) 
                            {
                                // Get a reference to the get orientation degrees function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kGetOrientationDegreesFunctionName,
                                                                     &(instancePrivate->getOrientationDegreesFunction));
                            }

                            // Check for success
                            if (result == 0) 
                            {
                                // Get a reference to the get orientation radians function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kGetOrientationRadiansFunctionName,
                                                                     &(instancePrivate->getOrientationRadiansFunction));
                            }

                            // Check for success
                            if (result == 0) 
                            {
                                // Get a reference to the get pixel function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kGetPixelFunctionName,
                                                                     &(instancePrivate->getPixelFunction));
                            }

                            // Check for success
                            if (result == 0) 
                            {
                                // Get a reference to the get pixels function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kGetPixelsFunctionName,
                                                                     &(instancePrivate->getPixelsFunction));
                            }

                            // Check for success
                            if (result == 0) 
                            {
                                // Get a reference to the get pressure function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kGetPressureFunctionName,
                                                                     &(instancePrivate->getPressureFunction));
                            }

                            // Check for success
                            if (result == 0) 
                            {
                                // Get a reference to the get temperature function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kGetTemperatureFunctionName,
                                                                     &(instancePrivate->getTemperatureFunction));
                            }

                            // Check for success
                            if (result == 0) 
                            {
                                // Get a reference to the get temperature from humidity function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kGetTemperatureFromHumidityFunctionName,
                                                                     &(instancePrivate->getTemperatureFromHumidityFunction));
                            }

                            // Check for success
                            if (result == 0) 
                            {
                                // Get a reference to the get temperature from pressure function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kGetTemperatureFromPressureFunctionName,
                                                                     &(instancePrivate->getTemperatureFromPressureFunction));
                            }

                            // Check for success
                            if (result == 0) 
                            {
                                // Get a reference to the load image function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kLoadImageFunctionName,
                                                                     &(instancePrivate->loadImageFunction));
                            }

                            // Check for success
                            if (result == 0) 
                            {
                                // Get a reference to the set IMU config function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kSetIMUConfigFunctionName,
                                                                     &(instancePrivate->setIMUConfigFunction));
                            }

                            // Check for success
                            if (result == 0) 
                            {
                                // Get a reference to the set pixel function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kSetPixelFunctionName,
                                                                     &(instancePrivate->setPixelFunction));
                            }

                            // Check for success
                            if (result == 0) 
                            {
                                // Get a reference to the set pixels function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kSetPixelsFunctionName,
                                                                     &(instancePrivate->setPixelsFunction));
                            }

                            // Check for success
                            if (result == 0) 
                            {
                                // Get a reference to the set rotation function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kSetRotationFunctionName,
                                                                     &(instancePrivate->setRotationFunction));
                            }

                            // Check for success
                            if (result == 0) 
                            {
                                // Get a reference to the show letter function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kShowLetterFunctionName,
                                                                     &(instancePrivate->showLetterFunction));
                            }

                            // Check for success
                            if (result == 0) 
                            {
                                // Get a reference to the show message function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kShowMessageFunctionName,
                                                                     &(instancePrivate->showMessageFunction));
                            }

                            // Check for success
                            if (result == 0)
                            {
                                // Get a reference to the stick submodule
                                instancePrivate->stickSubModule = 
                                    PyObject_GetAttrString (instancePrivate->self, kStickSubmoduleName);
                                if (instancePrivate->stickSubModule != NULL)
                                {
                                    // Get a reference to the get events function
                                    result = Python_GetFunctionReference(instancePrivate->stickSubModule,
                                                                         kGetEventsFunctionName,
                                                                         &(instancePrivate->getEventsFunction));

                                    // Check for error
                                    if (result == 0) 
                                    {
                                        // Get a reference to the wait for event function
                                        result = Python_GetFunctionReference(instancePrivate->stickSubModule,
                                                                             kWaitForEventFunctionName,
                                                                             &(instancePrivate->waitForEventFunction));
                                    }
                                }
                            }
                        }
                        else    // PyObject_CallObject failed 
                        {
                            result = Python_Error("PyObject_CallObject failed!");
                        }
                    }
                    else    // PyObject_GetAttrString failed
                    {
                        result = Python_Error("PyObject_GetAttrString failed!");
                    }
                }
                else	// PyImport_Import failed 
                {
                    result = Python_Error("PyImport_Import failed!");
                }

                // Clean up
                Py_DECREF(pName);
            }
            else    // PyUnicode_FromString faield
            {
                result = Python_Error("PyUnicode_FromString failed!");
            }

            // Restore current thread's Python state
            PyGILState_Release(state);

            if (result == 0)
            {
                *instance = (tSenseHAT_Instance)instancePrivate;
            }
            else    // There was an error
            {
                // Clean up
                SenseHAT_Release(instancePrivate);
                free((void*)instancePrivate);
                instancePrivate = NULL;

                // Close down the interpreter
                Python_CloseInterpreter();
            }
        }
        else    // malloc failed
        {
            result = ENOMEM;
        }
    }
    else    // Invalid argument
    {
        result = EINVAL;
    }
    return result;
}

// =================================================================================================
//  SenseHAT_Close
// =================================================================================================
int32_t SenseHAT_Close (tSenseHAT_Instance* instance)
{
    int32_t result = 0;

    // Check arguments
    if (instance != NULL)
    {
        // Get private data
	    tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)(*instance);

        // Clean up 
        if (instancePrivate != NULL)
        {
            SenseHAT_Release(instancePrivate);
            free((void*)instancePrivate);
            *instance = NULL;
        }

        // Close down the interpreter
        Python_CloseInterpreter();
    }
    else    // Invalid argument
    {
        result = EINVAL;
    }
    return result;
}

// =================================================================================================
//  SenseHAT_LEDSetRotation
// =================================================================================================
int32_t SenseHAT_LEDSetRotation (const tSenseHAT_Instance instance,
                                 tSenseHAT_LEDRotation rotation,
                                 bool redraw)
{
	int32_t result = 0;

    // Check arguments
    if (instance != NULL)
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;
        if (instancePrivate->setRotationFunction != NULL)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Convert rotation argument
            PyObject* pRotation = Py_BuildValue("i", (int32_t)rotation);
            if (pRotation != NULL)
            {
                // Convert redraw argument
                PyObject* pRedraw = PyBool_FromLong((int32_t)redraw);
                if (pRedraw != NULL)
                {
                    // Call the function
                    PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->setRotationFunction,
                                                                     instancePrivate->self, 
                                                                     pRotation,
                                                                     pRedraw,
                                                                     NULL);
                    // Check for success
                    if (pResult != NULL)
                    {
                        // Release reference
                        Py_DECREF(pResult);
                    }
                    else    // PyObject_CallFunctionObjArgs failed
                    {
                        result = Python_Error("PyObject_CallFunctionObjArgs failed!");
                    }

                    // Release reference
                    Py_DECREF(pRedraw);
                }
                else    // PyBool_FromLong failed
                {
                    result = Python_Error("PyBool_FromLong failed!");
                }

                // Release reference
                Py_DECREF(pRotation);
            }
            else    // Py_BuildValue failed  
            {
                result = Python_Error("Py_BuildValue failed!");
            }

            // Release our lock
            PyGILState_Release(state);
        }
        else    // Bad function pointer
        {
            result = EFAULT;
        }
    }
    else    // Invalid argument
    {
        result = EINVAL;
    }
    return result;
}

// =================================================================================================
//  SenseHAT_LEDFlipHorizontal
// =================================================================================================
int32_t SenseHAT_LEDFlipHorizontal (const tSenseHAT_Instance instance,
                                    bool redraw,
                                    tSenseHAT_LEDPixelArray pixels)
{
	int32_t result = 0;

    // Check arguments
    if (instance != NULL)
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;
        if (instancePrivate->flipHorizontalFunction != NULL)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Convert redraw argument
            PyObject* pRedraw = PyBool_FromLong((int32_t)redraw);
            if (pRedraw != NULL)
            {
                // Call the function
                PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->flipHorizontalFunction,
                                                                 instancePrivate->self, 
                                                                 pRedraw,
                                                                 NULL);
                // Check for success
                if (pResult != NULL)
                {
                    // If the caller wants the resulting pixel array, return it
                    if (pixels != NULL)
                    {
                        memset((void*)pixels, 0, sizeof(tSenseHAT_LEDPixelArray));
                        result = SenseHAT_ConvertPixelListToLEDPixelArray(pResult, pixels);
                    }

                    // Release reference
                    Py_DECREF(pResult);
                }
                else    // PyObject_CallFunctionObjArgs failed
                {
                    result = Python_Error("PyObject_CallFunctionObjArgs failed!");
                }

                // Release reference
                Py_DECREF(pRedraw);
            }
            else    // PyBool_FromLong failed  
            {
                result = Python_Error("PyBool_FromLong failed!");
            }

            // Release our lock
            PyGILState_Release(state);
        }
        else    // Bad function pointer
        {
            result = EFAULT;
        }
    }
    else    // Invalid argument
    {
        result = EINVAL;
    }
    return result;
}

// =================================================================================================
//  SenseHAT_LEDFlipVertical
// =================================================================================================
int32_t SenseHAT_LEDFlipVertical (const tSenseHAT_Instance instance,
                                  bool redraw,
                                  tSenseHAT_LEDPixelArray pixels)
{
	int32_t result = 0;

    // Check arguments
    if (instance != NULL)
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;
        if (instancePrivate->flipVerticalFunction != NULL)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Convert redraw argument
            PyObject* pRedraw = PyBool_FromLong((int32_t)redraw);
            if (pRedraw != NULL)
            {
                // Call the function
                PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->flipVerticalFunction,
                                                                 instancePrivate->self, 
                                                                 pRedraw,
                                                                 NULL);
                // Check for success
                if (pResult != NULL)
                {
                    // If the caller wants the resulting pixel array, return it
                    if (pixels != NULL)
                    {
                        memset((void*)pixels, 0, sizeof(tSenseHAT_LEDPixelArray));
                        result = SenseHAT_ConvertPixelListToLEDPixelArray(pResult, pixels);
                    }

                    // Release reference
                    Py_DECREF(pResult);
                }
                else    // PyObject_CallFunctionObjArgs failed
                {
                    result = Python_Error("PyObject_CallFunctionObjArgs failed!");
                }

                // Release reference
                Py_DECREF(pRedraw);
            }
            else    // PyBool_FromLong failed  
            {
                result = Python_Error("PyBool_FromLong failed!");
            }

            // Release our lock
            PyGILState_Release(state);
        }
        else    // Bad function pointer
        {
            result = EFAULT;
        }
    }
    else    // Invalid argument
    {
        result = EINVAL;
    }
    return result;
}

// =================================================================================================
//  SenseHAT_LEDGammaReset
// =================================================================================================
int32_t SenseHAT_LEDGammaReset (const tSenseHAT_Instance instance)
{
    int32_t result = 0;

    // Check arguments
    if (instance != NULL)
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;
        if (instancePrivate->gammaResetFunction != NULL)
        {
             // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Call the function
            PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->gammaResetFunction,
                                                             instancePrivate->self,
                                                             NULL);
            if (pResult != NULL)
            {
                // Release reference
                Py_DECREF(pResult);
            }

            // Release our lock
            PyGILState_Release(state);
        }
        else    // Bad function pointer
        {
            result = EFAULT;
        }
    }
    else    // Invalid argument
    {
        result = EINVAL;
    }
    return result;
}

// =================================================================================================
//  SenseHAT_LEDSetPixels
// =================================================================================================
int32_t SenseHAT_LEDSetPixels (const tSenseHAT_Instance instance,
                               const tSenseHAT_LEDPixelArray pixels)
{
	int32_t result = 0;

    // Check arguments
    if (instance != NULL)
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;
        if (instancePrivate->setPixelsFunction != NULL)
        {
             // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Create pixels list
            PyObject* pPixels = PyList_New(0);
            if (pPixels != NULL)
            {
                uint32_t index = 0;
                int res = 0;
                bool useDefault = (pixels == NULL) ? true : false;
                tSenseHAT_LEDPixel pixel = {0,0,0};

                // Convert pixels argument to Python list
                for (index = 0; index < 64; index++)
                {
                    // Using default?
                    if (!useDefault)
                    {
                        // No, using a user specified pixel color; check for validity
                        if ((pixels[index].red >= 0) &&
                            (pixels[index].red <= 255) &&
                            (pixels[index].green >= 0) &&
                            (pixels[index].green <= 255) &&
                            (pixels[index].blue >= 0) &&
                            (pixels[index].blue <= 255))
                        {
                            pixel.red = pixels[index].red;
                            pixel.green = pixels[index].green;
                            pixel.blue = pixels[index].blue;
                        }
                        else    // Invalid argument
                        {
                            result = EINVAL;
                        }
                    }

                    // Check for success
                    if (result == 0)
                    {
                        // Create list of pixel color components
                        PyObject* pPixel = PyList_New(0);
                        if (pPixel != NULL)
                        {
                            // Add red component
                            PyObject* pRed = Py_BuildValue("i", pixel.red);

                            // Check for success
                            if (pRed != NULL)
                            {
                                // Append red component to pixel component list
                                res = PyList_Append(pPixel, pRed);
                                if (res != 0)
                                {
                                    // PyList_Append failed
                                    result = res;
                                }

                                // Release reference
                                Py_DECREF(pRed);
                            }
                            else    // Py_BuildValue failed
                            {
                                result = -1;
                            }

                            // Check for success
                            if (result == 0)
                            {
                                // Add green component
                                PyObject* pGreen = Py_BuildValue("i", pixel.green);

                                // Check for success
                                if (pGreen != NULL)
                                {
                                    // Append green component to pixel component list
                                    res = PyList_Append(pPixel, pGreen);
                                    if (res != 0)
                                    {
                                        // PyList_Append failed
                                        result = res;
                                    }

                                    // Release reference
                                    Py_DECREF(pGreen);
                                }
                                else    // Py_BuildValue failed
                                {
                                    result = -1;
                                }
                            }
                        
                            // Check for success
                            if (result == 0)
                            {
                                // Add blue component
                                PyObject* pBlue = Py_BuildValue("i", pixel.blue);

                                // Check for success
                                if (pBlue != NULL)
                                {
                                    // Append blue component to pixel component list
                                    res = PyList_Append(pPixel, pBlue);
                                    if (res != 0)
                                    {
                                        // PyList_Append failed
                                        result = res;
                                    }

                                    // Release reference
                                    Py_DECREF(pBlue);
                                }
                                else    // Py_BuildValue failed
                                {
                                    result = -1;
                                }
                            }

                            // Check for success
                            if (result == 0)
                            {
                                // Make sure the list is the correct size
                                if (PyList_Size(pPixel) == 3)
                                {
                                    // Append pixel component list to pixel list
                                    res = PyList_Append(pPixels, pPixel);
                                    if (res != 0)
                                    {
                                        // PyList_Append failed
                                        result = res;
                                    }
                                }
                                else
                                {
                                    // Pixel list size is wrong
                                    result = -1;
                                }
                            }

                            // Release reference
                            Py_DECREF(pPixel);
                        }
                        else    // PyList_New failed
                        {
                            result = Python_Error("PyList_New failed!");
                        }
                    }

                    // If there's an error, break
                    if (result != 0)  
                    {
                        break;
                    }
                }       

                // Check for success
                if (result == 0)
                {
                    // Make sure the pixel list is the correct size
                    if (PyList_Size(pPixels) == 64)
                    {
                        // Call the function
                        PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->setPixelsFunction,
                                                                         instancePrivate->self, 
                                                                         pPixels,
                                                                         NULL);
                        if (pResult != NULL)
                        {
                            // Release reference
                            Py_DECREF(pResult);
                        }
                        else    // PyObject_CallFunctionObjArgs failed
                        {
                            result = Python_Error("PyObject_CallFunctionObjArgs failed!");
                        }
                    }
                    else
                    {
                        // Pixel list size is wrong
                        result = 1;
                    }
                }

                // Release reference
                Py_DECREF(pPixels);
            }
            else    // PyList_New failed
            {
                result = Python_Error("PyList_New failed!");
            }

            // Release our lock
            PyGILState_Release(state);
        }
        else    // Bad function pointer
        {
            result = EFAULT;
        }
    }
    else    // Invalid argument
    {
        result = EINVAL;
    }
    return result;
}

// =================================================================================================
//  SenseHAT_LEDGetPixels
// =================================================================================================
int32_t SenseHAT_LEDGetPixels (const tSenseHAT_Instance instance,
                               tSenseHAT_LEDPixelArray pixels)
{
	int32_t result = 0;

    // Check arguments
    if ((instance != NULL) &&
        (pixels != NULL))
    {
        // Initialize array
        memset((void*)pixels, 0, sizeof(tSenseHAT_LEDPixelArray));

        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;
        if (instancePrivate->getPixelsFunction != NULL)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();
            
            // Call the function
            PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->getPixelsFunction,
                                                             instancePrivate->self, NULL);
            
            // Check for success
            if (pResult != NULL)
            {
                // Convert to pixel array
                result = SenseHAT_ConvertPixelListToLEDPixelArray(pResult, pixels);

                // Release reference
                Py_DECREF(pResult);
            }
            else    // PyObject_CallFunctionObjArgs failed
            {
                result = Python_Error("PyObject_CallFunctionObjArgs failed!");
            }

            // Release our lock
            PyGILState_Release(state);
        }
        else    // Bad function pointer
        {
            result = EFAULT;
        }
    }
    else    // Invalid argument
    {
        result = EINVAL;
    }
    return result;
}

// =================================================================================================
//  SenseHAT_LEDSetPixel
// =================================================================================================
int32_t SenseHAT_LEDSetPixel (const tSenseHAT_Instance instance,
                              int32_t xPosition,
                              int32_t yPosition,
                              const tSenseHAT_LEDPixel* color)
{
	int32_t result = 0;

    // Check arguments
    if ((instance != NULL) &&
        (xPosition >= 0) &&
        (xPosition <= 7) &&
        (yPosition >= 0) &&
        (yPosition <= 7))
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;
        if (instancePrivate->setPixelFunction != NULL)
        {
             // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Convert x argument
            PyObject* pXPos = Py_BuildValue("i", xPosition);
            if (pXPos != NULL)
            {
                // Convert y argument
                PyObject* pYPos = Py_BuildValue("i", yPosition);
                if (pYPos != NULL)
                {
                    // Convert color argument
                    PyObject* pColor = NULL;

                    // Was a user specified pixel provided?
                    if (color != NULL)
                    {
                        // Check pixel for validity
                        if ((color->red >= 0) &&
                            (color->red <= 255) &&
                            (color->green >= 0) &&
                            (color->green <= 255) &&
                            (color->blue >= 0) &&
                            (color->blue <= 255))
                        {
                            // Convert pixel to pixel component list
                            pColor = Py_BuildValue("(lll)",
                                                   color->red,
                                                   color->green,
                                                   color->blue);
                            
                            // Check for failure
                            if (pColor == NULL)
                            {
                                result = Python_Error("Py_BuildValue failed!");
                            }
                        }
                        else    // Invalid argument
                        {
                            result = EINVAL;
                        }
                    }
                    else    // Use default values for pixel component list
                    {
                        pColor = Py_BuildValue("(lll)", 0, 0, 0);
                    }

                    // Make sure we got a color object
                    if (pColor != NULL)
                    {
                        // Call the function
                        PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->setPixelFunction,
                                                                         instancePrivate->self, 
                                                                         pXPos,
                                                                         pYPos,
                                                                         pColor,
                                                                         NULL);
                        if (pResult != NULL)
                        {
                            // Release reference
                            Py_DECREF(pResult);
                        }
                        else    // PyObject_CallFunctionObjArgs failed
                        {
                            result = Python_Error("PyObject_CallFunctionObjArgs failed!");
                        }

                        // Release reference
                        Py_DECREF(pColor);
                    }
                    
                    // Release reference
                    Py_DECREF(pYPos);
                }
                else    // Py_BuildValue failed  
                {
                    result = Python_Error("Py_BuildValue failed!");
                }

                // Release reference
                Py_DECREF(pXPos);
            }
            else    // Py_BuildValue failed
            {
                result = Python_Error("Py_BuildValue failed!");
            }

            // Release our lock
            PyGILState_Release(state);
       }
        else    // Bad function pointer
        {
            result = EFAULT;
        }
    }
    else    // Invalid argument
    {
        result = EINVAL;
    }
    return result;
}

// =================================================================================================
//  SenseHAT_LEDGetPixel
// =================================================================================================
int32_t SenseHAT_LEDGetPixel (const tSenseHAT_Instance instance,
                              int32_t xPosition,
                              int32_t yPosition,
                              tSenseHAT_LEDPixel* color)
{
    // NOTE: Pixel channel values are represented in RGB888 format, but returned
    // pixel channel values are represented in RGB565 format. This means that the
    // channel value you set may not be the channel value you get. To convert RGB888
    // to RGB565, do the following:
    //
    // RGB565.red = RGB888.red & 0xF8 (lower 3 bits are zero)
    // RGB565.green = RGB888.green & 0xFC (lower 2 bits are zero)
    // RGB565.blue = RGB888.blue & 0xF8 (lower 3 bits are zero)

	int32_t result = 0;

    // Check arguments
    if ((instance != NULL) &&
        (xPosition >= 0) &&
        (xPosition <= 7) &&
        (yPosition >= 0) &&
        (yPosition <= 7) &&
        (color != NULL))
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;

        // Initialize pixel color
        memset(color, 0, sizeof(tSenseHAT_LEDPixel));

        if (instancePrivate->getPixelFunction != NULL)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Convert x argument
            PyObject* pXPos = Py_BuildValue("i", xPosition);
            if (pXPos != NULL)
            {
                // Convert y argument
                PyObject* pYPos = Py_BuildValue("i", yPosition);
                if (pYPos != NULL)
                {
                    // Call the function
                    PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->getPixelFunction,
                                                                     instancePrivate->self, 
                                                                     pXPos,
                                                                     pYPos,
                                                                     NULL);

                    // Check for success
                    if (pResult != NULL)
                    {
                        // Result should be a list
                        if (PyList_Check(pResult))
                        {
                            // The list should have 3 elements
                            if (PyList_Size(pResult) == 3)
                            {
                                // Convert pixel component list to pixel
                                result = SenseHAT_ConvertPixelToLEDPixel(pResult, color);
                            }
                            else    // List doesn't have 3 elements
                            {
                                result = -1;
                            }
                        }
                        else    // Result is not a list
                        {
                            result = -1;
                        }
                        
                        // Release reference
                        Py_DECREF(pResult);
                    }
                    else    // PyObject_CallFunctionObjArgs failed
                    {
                        result = Python_Error("PyObject_CallFunctionObjArgs failed!");
                    }

                    // Release reference
                    Py_DECREF(pYPos);
                }
                else    // Py_BuildValue failed  
                {
                    result = Python_Error("Py_BuildValue failed!");
                }

                // Release reference
                Py_DECREF(pXPos);
            }
            else    // Py_BuildValue failed
            {
                result = Python_Error("Py_BuildValue failed!");
            }

            // Release our lock
            PyGILState_Release(state);
        }
        else    // Bad function pointer
        {
            result = EFAULT;
        }
    }
    else    // Invalid argument
    {
        result = EINVAL;
    }
    return result;
}

// =================================================================================================
//  SenseHAT_LEDLoadImage
// =================================================================================================
int32_t SenseHAT_LEDLoadImage (const tSenseHAT_Instance instance,
                               const char* imageFilePath,
                               bool redraw,
                               tSenseHAT_LEDPixelArray pixels)
{
	int32_t result = 0;

    // Check arguments
    if ((instance != NULL) && 
        (imageFilePath != NULL) && 
        (strlen(imageFilePath) > 0))
    {
        // Make sure file path points to a file
        bool fileExists = false;
        FILE* fp = fopen(imageFilePath, "rb");
        if (fp != NULL)
        {
            // The file exists

            // TODO: Is the file an image (JPEG, PNG, etc.)?

            // TODO: Is the image 8-bit?

            // TODO: Is the image 8x8 pixels?

            (void)fclose(fp);
            fileExists = true;
        }

        // Does the file exist?
        if (fileExists)
        {
            // Get private data
            tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;
            if (instancePrivate->loadImageFunction != NULL)
            {
                // Get a lock
                PyGILState_STATE state = PyGILState_Ensure();

                // Convert path argument
                PyObject* pPath = PyUnicode_DecodeUTF8(imageFilePath, strlen(imageFilePath), NULL);
                if (pPath != NULL)
                {
                    // Convert redraw argument
                    PyObject* pRedraw = PyBool_FromLong((int32_t)redraw);
                    if (pRedraw != NULL)
                    {
                        // Call the function
                        PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->loadImageFunction,
                                                                         instancePrivate->self, 
                                                                         pPath,
                                                                         pRedraw,
                                                                         NULL);

                        // Check for success
                        if (pResult != NULL)
                        {
                            // If the caller wants the resulting pixel array, return it
                            if (pixels != NULL)
                            {
                                memset((void*)pixels, 0, sizeof(tSenseHAT_LEDPixelArray));
                                result = SenseHAT_ConvertPixelListToLEDPixelArray(pResult, pixels);
                            }

                            // Release reference
                            Py_DECREF(pResult);
                        }
                        else    // PyObject_CallFunctionObjArgs failed
                        {
                            result = Python_Error("PyObject_CallFunctionObjArgs failed!");
                        }

                        // Release reference
                        Py_DECREF(pRedraw);
                    }
                    else    // PyBool_FromLong failed 
                    {
                        result = Python_Error("PyBool_FromLong failed!");
                    }

                    // Release reference
                    Py_DECREF(pPath);
                }
                else    // PyUnicode_DecodeUTF8 failed 
                {
                    result = Python_Error("PyUnicode_DecodeUTF8 failed!");
                }

                // Release our lock
                PyGILState_Release(state);
            }
            else    // Bad function pointer
            {
                result = EFAULT;
            }
        }
        else    // File doesn't exist
        {
            result = ENOENT;
        }
    }
    else    // Invalid argument
    {
        result = EINVAL;
    }
    return result;
}

// =================================================================================================
//  SenseHAT_LEDClear
// =================================================================================================
int32_t SenseHAT_LEDClear (const tSenseHAT_Instance instance,
                           const tSenseHAT_LEDPixel* color)
{
	int32_t result = 0;

    // Check arguments
    if (instance != NULL)
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;
        if (instancePrivate->clearFunction != NULL)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Convert color argument
            PyObject* pColor = NULL;

            // Did caller provide pixel color?
            if (color != NULL)
            {
                // Check pixel validity
                if ((color->red >= 0) &&
                    (color->red <= 255) &&
                    (color->green >= 0) &&
                    (color->green <= 255) &&
                    (color->blue >= 0) &&
                    (color->blue <= 255))
                {
                    // Build the pixel color component list
                    pColor = Py_BuildValue("(lll)",
                                           color->red,
                                           color->green,
                                           color->blue);
                }
                else    // Invalid argument
                {
                    result = EINVAL;
                }
            }
            else    // Use default pixel color
            {
                // Build the pixel color component list
                pColor = Py_BuildValue("(lll)", 0L, 0L, 0L);
            }

            // Make sure we got a color object
            if (pColor != NULL)
            {
                // Call the function
                PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->clearFunction,
                                                                 instancePrivate->self, 
                                                                 pColor,
                                                                 NULL);
                if (pResult != NULL)
                {
                    // Release reference
                    Py_DECREF(pResult);
                }
                else    // PyObject_CallFunctionObjArgs failed
                {
                    result = Python_Error("PyObject_CallFunctionObjArgs failed!");
                }

                // Release reference
                Py_DECREF(pColor);
            }
            else    // Py_BuildValue failed
            {
                result = Python_Error("Py_BuildValue failed!");
            }

            // Release our lock
            PyGILState_Release(state);
        }
        else    // Bad function pointer
        {
            result = EFAULT;
        }
    }
    else    // Invalid argument
    {
        result = EINVAL;
    }
    return result;
}

// =================================================================================================
//  SenseHAT_LEDShowLetter
// =================================================================================================
int32_t SenseHAT_LEDShowLetter (const tSenseHAT_Instance instance,
                                const char* letter,
                                const tSenseHAT_LEDPixel* textColor,
                                const tSenseHAT_LEDPixel* backColor)
{
	int32_t result = 0;

    // Check arguments
    if ((instance != NULL) && 
        (letter != NULL) && 
        (strlen(letter) == 1))
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;
        if (instancePrivate->showLetterFunction != NULL)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Convert letter argument
            PyObject* pLetter = PyUnicode_DecodeUTF8(letter, strlen(letter), NULL);
            if (pLetter != NULL)
            {
                PyObject* pTextColor = NULL;
                PyObject* pBackColor = NULL;

                // Convert text color argument
                if (textColor != NULL)
                {
                    if ((textColor->red >= 0) &&
                        (textColor->red <= 255) &&
                        (textColor->green >= 0) &&
                        (textColor->green <= 255) &&
                        (textColor->blue >= 0) &&
                        (textColor->blue <= 255))
                    {
                        pTextColor = Py_BuildValue("(lll)",
                                                   textColor->red,
                                                   textColor->green,
                                                   textColor->blue);
                        if (pTextColor == NULL)
                        {
                            result = Python_Error("Py_BuildValue failed!");
                        }
                    }
                    else    // Invalid argument
                    {
                        result = EINVAL;
                    }
                }
                else
                {
                    pTextColor = Py_BuildValue("(lll)", 255L, 255L, 255L);
                }
                
                // Check for success
                if (result == 0)
                {
                    // Convert back color argument
                    if (backColor != NULL)
                    {
                        if ((backColor->red >= 0) &&
                            (backColor->red <= 255) &&
                            (backColor->green >= 0) &&
                            (backColor->green <= 255) &&
                            (backColor->blue >= 0) &&
                            (backColor->blue <= 255))
                        {
                            pBackColor = Py_BuildValue("(lll)",
                                                       backColor->red,
                                                       backColor->green,
                                                       backColor->blue);
                            if (pBackColor == NULL)
                            {
                                result = Python_Error("Py_BuildValue failed!");
                            }
                        }
                        else    // Invalid argument
                        {
                            result = EINVAL;
                        }
                    }
                    else
                    {
                        pBackColor = Py_BuildValue("(lll)", 0L, 0L, 0L);
                    }
                }

                // Check for success
                if (result == 0)
                {
                    // Call the function
                    PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->showLetterFunction,
                                                                     instancePrivate->self, 
                                                                     pLetter,
                                                                     pTextColor,
                                                                     pBackColor,
                                                                     NULL);
                    if (pResult != NULL)
                    {
                        // Release reference
                        Py_DECREF(pResult);
                    }
                    else    // PyObject_CallFunctionObjArgs failed
                    {
                        result = Python_Error("PyObject_CallFunctionObjArgs failed!");
                    }
                }

                if (pBackColor != NULL)
                {
                    // Release reference
                    Py_DECREF(pBackColor);
                }

                if (pTextColor != NULL)
                {
                    // Release reference
                    Py_DECREF(pTextColor);
                }

                // Release reference
                Py_DECREF(pLetter);
            }
            else    // PyUnicode_DecodeUTF8 failed
            {
                result = Python_Error("PyUnicode_DecodeUTF8 failed!");
            }

            // Release our lock
            PyGILState_Release(state);
        }
        else    // Bad function pointer
        {
            result = EFAULT;
        }
    }
    else    // Invalid argument
    {
        result = EINVAL;
    }
    return result;
}

// =================================================================================================
//  SenseHAT_LEDShowMessage
// =================================================================================================
int32_t SenseHAT_LEDShowMessage (const tSenseHAT_Instance instance,
                                 const char* message,
                                 double scrollSpeed,
                                 const tSenseHAT_LEDPixel* textColor,
                                 const tSenseHAT_LEDPixel* backColor)
{
	int32_t result = 0;

    // Check arguments
    if ((instance != NULL) && 
        (message != NULL) && 
        (strlen(message) > 0) &&
        (scrollSpeed >= 0))
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;
        if (instancePrivate->showMessageFunction != NULL)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Convert message argument
            PyObject* pMessage = PyUnicode_DecodeUTF8(message, strlen(message), NULL);
            if (pMessage != NULL)
            {
                // Convert scroll speed argument
                PyObject* pScrollSpeed = PyFloat_FromDouble(scrollSpeed);
                if (pScrollSpeed != NULL)
                {
                    PyObject* pTextColor = NULL;
                    PyObject* pBackColor = NULL;

                    // Convert text color argument
                    if (textColor != NULL)
                    {
                        if ((textColor->red >= 0) &&
                            (textColor->red <= 255) &&
                            (textColor->green >= 0) &&
                            (textColor->green <= 255) &&
                            (textColor->blue >= 0) &&
                            (textColor->blue <= 255))
                        {
                            pTextColor = Py_BuildValue("(lll)",
                                                       textColor->red,
                                                       textColor->green,
                                                       textColor->blue);
                            if (pTextColor == NULL)
                            {
                                result = Python_Error("Py_BuildValue failed!");
                            }
                        }
                        else    // Invalid argument
                        {
                            result = EINVAL;
                        }
                    }
                    else
                    {
                        pTextColor = Py_BuildValue("(lll)", 255L, 255L, 255L);
                    }

                    // Check for success
                    if (result == 0)
                    {
                        // Convert back color argument
                        if (backColor != NULL)
                        {
                            if ((backColor->red >= 0) &&
                                (backColor->red <= 255) &&
                                (backColor->green >= 0) &&
                                (backColor->green <= 255) &&
                                (backColor->blue >= 0) &&
                                (backColor->blue <= 255))
                            {
                                pBackColor = Py_BuildValue("(lll)",
                                                           backColor->red,
                                                           backColor->green,
                                                           backColor->blue);
                                if (pBackColor == NULL)
                                {
                                    result = Python_Error("Py_BuildValue failed!");
                                }
                            }
                            else    // Invalid argument
                            {
                                result = EINVAL;
                            }
                        }
                        else
                        {
                            pBackColor = Py_BuildValue("(lll)", 0L, 0L, 0L);
                        }
                    }

                    // Check for success
                    if (result == 0)
                    {
                        // Call the function
                        PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->showMessageFunction,
                                                                         instancePrivate->self, 
                                                                         pMessage,
                                                                         pScrollSpeed,
                                                                         pTextColor,
                                                                         pBackColor,
                                                                         NULL);
                        if (pResult != NULL)
                        {
                            // Release reference
                            Py_DECREF(pResult);
                        }
                        else    // PyObject_CallFunctionObjArgs failed
                        {
                            result = Python_Error("PyObject_CallFunctionObjArgs failed!");
                        }
                    }

                    if (pBackColor != NULL)
                    {
                        // Release reference
                        Py_DECREF(pBackColor);
                    }

                    if (pTextColor != NULL)
                    {
                        // Release reference
                        Py_DECREF(pTextColor);
                    }

                    // Release reference
                    Py_DECREF(pScrollSpeed);
                }
                else    // PyFloat_FromDouble failed
                {
                    result = Python_Error("PyFloat_FromDouble failed!");
                }

                // Release reference
                Py_DECREF(pMessage);
            }
            else    // PyUnicode_DecodeUTF8 failed
            {
                result = Python_Error("PyUnicode_DecodeUTF8 failed!");
            }

            // Release our lock
            PyGILState_Release(state);
        }
        else    // Bad function pointer
        {
            result = EFAULT;
        }
    }
    else    // Invalid argument
    {
        result = EINVAL;
    }
    return result;
}

// =================================================================================================
//  SenseHAT_GetHumidity
// =================================================================================================
int32_t SenseHAT_GetHumidity (const tSenseHAT_Instance instance,
                              double* percentRelativeHumidity)
{
	int32_t result = 0;

    // Check arguments
    if ((instance != NULL) &&
        (percentRelativeHumidity != NULL))
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;

        // Setup
        *percentRelativeHumidity = 0;

        if (instancePrivate->getHumidityFunction != NULL)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Call the function
            PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->getHumidityFunction,
                                                             instancePrivate->self, NULL);
            if (pResult != NULL)
            {
                // Get the result
                if (PyFloat_Check(pResult))
                {
                    *percentRelativeHumidity = PyFloat_AsDouble(pResult);
                }
                else    // PyFloat_Check failed
                {
                    result = -1;
                }

                // Release reference
                Py_DECREF(pResult);
            }
            else    // PyObject_CallFunctionObjArgs failed
            {
                result = Python_Error("PyObject_CallFunctionObjArgs failed!");
            }

            // Release our lock
            PyGILState_Release(state);
        }
        else    // Bad function pointer
        {
            result = EFAULT;
        }
    }
    else    // Invalid argument
    {
        result = EINVAL;
    }
	return result;
}

// =================================================================================================
//  SenseHAT_GetTemperature
// =================================================================================================
int32_t SenseHAT_GetTemperature (const tSenseHAT_Instance instance,
                                 double* degreesCelsius)
{
	int32_t result = 0;

    // Check arguments
    if ((instance != NULL) &&
        (degreesCelsius != NULL))
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;

        // Setup
        *degreesCelsius = 0;

        if (instancePrivate->getTemperatureFunction != NULL)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Call the function
            PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->getTemperatureFunction,
                                                             instancePrivate->self, NULL);
            if (pResult != NULL)
            {
                // Get the result
                if (PyFloat_Check(pResult))
                {
                    *degreesCelsius = PyFloat_AsDouble(pResult);
                }
                else    // PyFloat_Check failed
                {
                    result = -1;
                }

                // Release reference
                Py_DECREF(pResult);
            }
            else    // PyObject_CallFunctionObjArgs failed
            {
                result = Python_Error("PyObject_CallFunctionObjArgs failed!");
            }

            // Release our lock
            PyGILState_Release(state);
        }
        else    // Bad function pointer
        {
            result = EFAULT;
        }
    }
    else    // Invalid argument
    {
        result = EINVAL;
    }
	return result;
}

// =================================================================================================
//  SenseHAT_GetPressure
// =================================================================================================
int32_t SenseHAT_GetPressure (const tSenseHAT_Instance instance,
                              double* millibars)
{
	int32_t result = 0;

    // Check arguments
    if ((instance != NULL) &&
        (millibars != NULL))
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;

        // Setup
        *millibars = 0;

        if (instancePrivate->getPressureFunction != NULL)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Call the function
            PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->getPressureFunction,
                                                             instancePrivate->self, NULL);
            if (pResult != NULL)
            {
                // Get the result
                if (PyFloat_Check(pResult))
                {
                    *millibars = PyFloat_AsDouble(pResult);
                }
                else    // PyFloat_Check failed
                {
                    result = -1;
                }

                // Release reference
                Py_DECREF(pResult);
            }
            else    // PyObject_CallFunctionObjArgs failed
            {
                result = Python_Error("PyObject_CallFunctionObjArgs failed!");
            }

            // Release our lock
            PyGILState_Release(state);
        }
        else    // Bad function pointer
        {
            result = EFAULT;
        }
    }
    else    // Invalid argument
    {
        result = EINVAL;
    }
	return result;
}

// =================================================================================================
//  SenseHAT_GetCompass
// =================================================================================================
int32_t SenseHAT_GetCompass (const tSenseHAT_Instance instance,
                             double* degrees)
{
	int32_t result = 0;

    // Check arguments
    if ((instance != NULL) &&
        (degrees != NULL))
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;

        // Setup
        *degrees = 0;

        if (instancePrivate->getCompassFunction != NULL)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Call the function
            PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->getCompassFunction,
                                                             instancePrivate->self, NULL);
            if (pResult != NULL)
            {
                // Get the result
                if (PyFloat_Check(pResult))
                {
                    *degrees = PyFloat_AsDouble(pResult);
                }
                else    // PyFloat_Check failed
                {
                    result = -1;
                }

                // Release reference
                Py_DECREF(pResult);
            }
            else    // PyObject_CallFunctionObjArgs failed
            {
                result = Python_Error("PyObject_CallFunctionObjArgs failed!");
            }

            // Release our lock
            PyGILState_Release(state);
        }
        else    // Bad function pointer
        {
            result = EFAULT;
        }
    }
    else    // Invalid argument
    {
        result = EINVAL;
    }
	return result;
}

// =================================================================================================
//  SenseHAT_GetAccelerometer
// =================================================================================================
int32_t SenseHAT_GetAccelerometer (const tSenseHAT_Instance instance,
                                   tSenseHAT_Orientation* orientation)
{
	int32_t result = 0;

    // Check arguments
    if ((instance != NULL) &&
        (orientation != NULL))
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;

        // Setup
        memset((void*)orientation, 0, sizeof(tSenseHAT_Orientation));

        if (instancePrivate->getAccelerometerFunction != NULL)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Call the function
            PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->getAccelerometerFunction,
                                                             instancePrivate->self, NULL);
            if (pResult != NULL)
            {
                // Convert the result
                result = SenseHAT_ConvertDictToOrientation(pResult, orientation);

                // Release reference
                Py_DECREF(pResult);
            }
            else    // PyObject_CallFunctionObjArgs failed
            {
                result = Python_Error("PyObject_CallFunctionObjArgs failed!");
            }

            // Release our lock
            PyGILState_Release(state);
        }
        else    // Bad function pointer
        {
            result = EFAULT;
        }
    }
    else    // Invalid argument
    {
        result = EINVAL;
    }
	return result;
}

// =================================================================================================
//  SenseHAT_GetAccelerometerRaw
// =================================================================================================
int32_t SenseHAT_GetAccelerometerRaw (const tSenseHAT_Instance instance,
                                      tSenseHAT_RawData* rawData)
{
	int32_t result = 0;

    // Check arguments
    if ((instance != NULL) &&
        (rawData != NULL))
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;

        // Setup
        memset((void*)rawData, 0, sizeof(tSenseHAT_RawData));

        if (instancePrivate->getAccelerometerRawFunction != NULL)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Call the function
            PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->getAccelerometerRawFunction,
                                                             instancePrivate->self, NULL);
            if (pResult != NULL)
            {
                // Convert the result
                result = SenseHAT_ConvertDictToRawData(pResult, rawData);

                // Release reference
                Py_DECREF(pResult);
            }
            else    // PyObject_CallFunctionObjArgs failed
            {
                result = Python_Error("PyObject_CallFunctionObjArgs failed!");
            }

            // Release our lock
            PyGILState_Release(state);
        }
        else    // Bad function pointer
        {
            result = EFAULT;
        }
    }
    else    // Invalid argument
    {
        result = EINVAL;
    }
	return result;
}

// =================================================================================================
//  SenseHAT_GetCompassRaw
// =================================================================================================
int32_t SenseHAT_GetCompassRaw (const tSenseHAT_Instance instance,
                                tSenseHAT_RawData* rawData)
{
	int32_t result = 0;

    // Check arguments
    if ((instance != NULL) &&
        (rawData != NULL))
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;

        // Setup
        memset((void*)rawData, 0, sizeof(tSenseHAT_RawData));

        if (instancePrivate->getCompassRawFunction != NULL)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Call the function
            PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->getCompassRawFunction,
                                                             instancePrivate->self, NULL);
            if (pResult != NULL)
            {
                // Convert the result
                result = SenseHAT_ConvertDictToRawData(pResult, rawData);

                // Release reference
                Py_DECREF(pResult);
            }
            else    // PyObject_CallFunctionObjArgs failed
            {
                result = Python_Error("PyObject_CallFunctionObjArgs failed!");
            }

            // Release our lock
            PyGILState_Release(state);
        }
        else    // Bad function pointer
        {
            result = EFAULT;
        }
    }
    else    // Invalid argument
    {
        result = EINVAL;
    }
	return result;
}

// =================================================================================================
//  SenseHAT_GetGyroscope
// =================================================================================================
int32_t SenseHAT_GetGyroscope (const tSenseHAT_Instance instance,
                               tSenseHAT_Orientation* orientation)
{
	int32_t result = 0;

    // Check arguments
    if ((instance != NULL) &&
        (orientation != NULL))
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;

        // Setup
        memset((void*)orientation, 0, sizeof(tSenseHAT_Orientation));

        if (instancePrivate->getGyroscopeFunction != NULL)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Call the function
            PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->getGyroscopeFunction,
                                                             instancePrivate->self, NULL);
            if (pResult != NULL)
            {
                // Convert the result
                result = SenseHAT_ConvertDictToOrientation(pResult, orientation);

                // Release reference
                Py_DECREF(pResult);
            }
            else    // PyObject_CallFunctionObjArgs failed
            {
                result = Python_Error("PyObject_CallFunctionObjArgs failed!");
            }

            // Release our lock
            PyGILState_Release(state);
        }
        else    // Bad function pointer
        {
            result = EFAULT;
        }
    }
    else    // Invalid argument
    {
        result = EINVAL;
    }
	return result;
}

// =================================================================================================
//  SenseHAT_GetGyroscopeRaw
// =================================================================================================
int32_t SenseHAT_GetGyroscopeRaw (const tSenseHAT_Instance instance,
                                  tSenseHAT_RawData* rawData)
{
	int32_t result = 0;

    // Check arguments
    if ((instance != NULL) &&
        (rawData != NULL))
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;

        // Setup
        memset((void*)rawData, 0, sizeof(tSenseHAT_RawData));

        if (instancePrivate->getGyroscopeRawFunction != NULL)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Call the function
            PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->getGyroscopeRawFunction,
                                                             instancePrivate->self, NULL);
            if (pResult != NULL)
            {
                // Convert the result
                result = SenseHAT_ConvertDictToRawData(pResult, rawData);

                // Release reference
                Py_DECREF(pResult);
            }
            else    // PyObject_CallFunctionObjArgs failed
            {
                result = Python_Error("PyObject_CallFunctionObjArgs failed!");
            }

            // Release our lock
            PyGILState_Release(state);
        }
        else    // Bad function pointer
        {
            result = EFAULT;
        }
    }
    else    // Invalid argument
    {
        result = EINVAL;
    }
	return result;
}

// =================================================================================================
//  SenseHAT_GetOrientation
// =================================================================================================
int32_t SenseHAT_GetOrientation (const tSenseHAT_Instance instance,
                                 tSenseHAT_Orientation* orientation)
{
	int32_t result = 0;

    // Check arguments
    if ((instance != NULL) &&
        (orientation != NULL))
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;

        // Setup
        memset((void*)orientation, 0, sizeof(tSenseHAT_Orientation));

        if (instancePrivate->getOrientationFunction != NULL)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Call the function
            PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->getOrientationFunction,
                                                             instancePrivate->self, NULL);
            if (pResult != NULL)
            {
                // Convert the result
                result = SenseHAT_ConvertDictToOrientation(pResult, orientation);

                // Release reference
                Py_DECREF(pResult);
            }
            else    // PyObject_CallFunctionObjArgs failed
            {
                result = Python_Error("PyObject_CallFunctionObjArgs failed!");
            }

            // Release our lock
            PyGILState_Release(state);
        }
        else    // Bad function pointer
        {
            result = EFAULT;
        }
    }
    else    // Invalid argument
    {
        result = EINVAL;
    }
	return result;
}

// =================================================================================================
//  SenseHAT_GetOrientationDegrees
// =================================================================================================
int32_t SenseHAT_GetOrientationDegrees (const tSenseHAT_Instance instance,
                                        tSenseHAT_Orientation* orientation)
{
	int32_t result = 0;

    // Check arguments
    if ((instance != NULL) &&
        (orientation != NULL))
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;

        // Setup
        memset((void*)orientation, 0, sizeof(tSenseHAT_Orientation));

        if (instancePrivate->getOrientationDegreesFunction != NULL)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Call the function
            PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->getOrientationDegreesFunction,
                                                             instancePrivate->self, NULL);
            if (pResult != NULL)
            {
                // Convert the result
                result = SenseHAT_ConvertDictToOrientation(pResult, orientation);

                // Release reference
                Py_DECREF(pResult);
            }
            else    // PyObject_CallFunctionObjArgs failed
            {
                result = Python_Error("PyObject_CallFunctionObjArgs failed!");
            }

            // Release our lock
            PyGILState_Release(state);
        }
        else    // Bad function pointer
        {
            result = EFAULT;
        }
    }
    else    // Invalid argument
    {
        result = EINVAL;
    }
	return result;
}

// =================================================================================================
//  SenseHAT_GetOrientationRadians
// =================================================================================================
int32_t SenseHAT_GetOrientationRadians (const tSenseHAT_Instance instance,
                                        tSenseHAT_Orientation* orientation)
{
	int32_t result = 0;

    // Check arguments
    if ((instance != NULL) &&
        (orientation != NULL))
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;

        // Setup
        memset((void*)orientation, 0, sizeof(tSenseHAT_Orientation));

        if (instancePrivate->getOrientationRadiansFunction != NULL)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Call the function
            PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->getOrientationRadiansFunction,
                                                             instancePrivate->self, NULL);
            if (pResult != NULL)
            {
                // Convert the result
                result = SenseHAT_ConvertDictToOrientation(pResult, orientation);

                // Release reference
                Py_DECREF(pResult);
            }
            else    // PyObject_CallFunctionObjArgs failed
            {
                result = Python_Error("PyObject_CallFunctionObjArgs failed!");
            }

            // Release our lock
            PyGILState_Release(state);
        }
        else    // Bad function pointer
        {
            result = EFAULT;
        }
    }
    else    // Invalid argument
    {
        result = EINVAL;
    }
	return result;
}

// =================================================================================================
//  SenseHAT_GetTemperatureFromHumidity
// =================================================================================================
int32_t SenseHAT_GetTemperatureFromHumidity (const tSenseHAT_Instance instance,
                                             double* degreesCelsius)
{
	int32_t result = 0;

    // Check arguments
    if ((instance != NULL) &&
        (degreesCelsius != NULL))
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;

        // Setup
        *degreesCelsius = 0;

        if (instancePrivate->getTemperatureFromHumidityFunction != NULL)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Call the function
            PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->getTemperatureFromHumidityFunction,
                                                             instancePrivate->self, NULL);
            if (pResult != NULL)
            {
                // Get the result
                if (PyFloat_Check(pResult))
                {
                    *degreesCelsius = PyFloat_AsDouble(pResult);
                }
                else    // PyFloat_Check failed
                {
                    result = -1;
                }

                // Release reference
                Py_DECREF(pResult);
            }
            else    // PyObject_CallFunctionObjArgs failed
            {
                result = Python_Error("PyObject_CallFunctionObjArgs failed!");
            }

            // Release our lock
            PyGILState_Release(state);
        }
        else    // Bad function pointer
        {
            result = EFAULT;
        }
    }
    else    // Invalid argument
    {
        result = EINVAL;
    }
	return result;
}

// =================================================================================================
//  SenseHAT_GetTemperatureFromPressure
// =================================================================================================
int32_t SenseHAT_GetTemperatureFromPressure (const tSenseHAT_Instance instance,
                                             double* degreesCelsius)
{
	int32_t result = 0;

    // Check arguments
    if ((instance != NULL) &&
        (degreesCelsius != NULL))
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;

        // Setup
        *degreesCelsius = 0;

        if (instancePrivate->getTemperatureFromPressureFunction != NULL)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Call the function
            PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->getTemperatureFromPressureFunction,
                                                             instancePrivate->self, NULL);
            if (pResult != NULL)
            {
                // Get the result
                if (PyFloat_Check(pResult))
                {
                    *degreesCelsius = PyFloat_AsDouble(pResult);
                }
                else    // PyFloat_Check failed
                {
                    result = -1;
                }
                
                // Release reference
                Py_DECREF(pResult);
            }
            else    // PyObject_CallFunctionObjArgs failed
            {
                result = Python_Error("PyObject_CallFunctionObjArgs failed!");
            }

            // Release our lock
            PyGILState_Release(state);
        }
        else    // Bad function pointer
        {
            result = EFAULT;
        }
    }
    else    // Invalid argument
    {
        result = EINVAL;
    }
	return result;
}

// =================================================================================================
//  SenseHAT_SetIMUConfiguration
// =================================================================================================
int32_t SenseHAT_SetIMUConfiguration (const tSenseHAT_Instance instance,
                                      bool enableCompass,
                                      bool enableGyroscope,
                                      bool enableAccelerometer)
{
	int32_t result = 0;

    // Check arguments
    if (instance != NULL)
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;

        if (instancePrivate->setIMUConfigFunction != NULL)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Convert enableCompass argument
            PyObject* pEnableCompass = PyBool_FromLong((int32_t)enableCompass);
            if (pEnableCompass != NULL)
            {
                // Convert enableGyroscope argument
                PyObject* pEnableGyroscope = PyBool_FromLong((int32_t)enableGyroscope);
                if (pEnableGyroscope != NULL)
                {
                    // Convert enableAccelerometer argument
                    PyObject* pEnableAccelerometer = PyBool_FromLong((int32_t)enableAccelerometer);
                    if (pEnableAccelerometer != NULL)
                    {
                        // Call the function
                        PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->setIMUConfigFunction,
                                                                         instancePrivate->self, 
                                                                         pEnableCompass,
                                                                         pEnableGyroscope,
                                                                         pEnableAccelerometer,
                                                                         NULL);
                        if (pResult != NULL)
                        {
                            // Release reference
                            Py_DECREF(pResult);
                        }
                        else    // PyObject_CallFunctionObjArgs failed
                        {
                            result = Python_Error("PyObject_CallFunctionObjArgs failed!");
                        }

                        // Release reference
                        Py_DECREF(pEnableAccelerometer);
                    }
                    else    // PyBool_FromLong failed
                    {
                        result = Python_Error("PyBool_FromLong failed!");
                    }

                    // Release reference
                    Py_DECREF(pEnableGyroscope);
                }
                else    // PyBool_FromLong failed
                {
                    result = Python_Error("PyBool_FromLong failed!");
                }

                // Release reference
                Py_DECREF(pEnableCompass);
            }
            else    // PyBool_FromLong failed
            {
                result = Python_Error("PyBool_FromLong failed!");
            }

            // Release our lock
            PyGILState_Release(state);
        }
        else    // Bad function pointer
        {
            result = EFAULT;
        }
    }
    else    // Invalid argument
    {
        result = EINVAL;
    }
	return result;
}

// =================================================================================================
//  SenseHAT_GetEvents
// =================================================================================================
int32_t SenseHAT_GetEvents (const tSenseHAT_Instance instance,
							int32_t* eventCount,
                            tSenseHAT_JoystickEvent** events)
{
	int32_t result = 0;

    // Check arguments
    if ((instance != NULL) &&
		(eventCount != NULL))
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;

        // Setup
		*eventCount = 0;

        if (instancePrivate->getEventsFunction != NULL)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Call the function
            PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->getEventsFunction,
                                                             NULL);
            if (pResult != NULL)
            {
                // The result should be a list of events
                if (PyList_Check(pResult))
                {
                    // Were there any events?
                    int32_t numEvents = PyList_Size(pResult);
                    if (numEvents > 0)
                    {
                        // Was a list of events requested by the caller?
                        if (events != NULL)
                        {
                            tSenseHAT_JoystickEvent* list = NULL;

                            // Setup
                            *events = NULL;

                            list = (tSenseHAT_JoystickEvent*)(sizeof(tSenseHAT_JoystickEvent) * numEvents);
                            if (list != NULL)
                            {
                                memset(list, 0, sizeof(tSenseHAT_JoystickEvent) * numEvents);

                                uint32_t i = 0;
                                PyObject* tuple = NULL;

                                for (i = 0; i < numEvents; i++)
                                {
                                    // Get an event from the list
                                    tuple = PyList_GetItem(pResult, i);
                                    if (tuple != NULL)
                                    {
                                        if (PyTuple_Check(tuple))
                                        {
                                            result = SenseHAT_ParseJoystickEvent(tuple, &(list[i]));
                                        }
                                        else    // PyTuple_Check failed
                                        {
                                            result = -1;
                                        }

                                        // Release reference
                                        Py_DECREF(tuple);
                                        tuple = NULL;
                                    }
                                    else    // PyList_GetItem failed
                                    {
                                        result = Python_Error("PyList_GetItem failed!");
                                        break;
                                    }

                                    if (result != 0)
                                    {
                                        break;
                                    }
                                }

                                if (result == 0)
                                {
                                    *eventCount = numEvents;
                                    *events = list;
                                }
                                else
                                {
                                    // Clean up
                                    free((void*)list);
                                }
                            }
                            else    // malloc failed
                            {
                                result = ENOMEM;
                            }
                        }
                    }
                }
                else    // PyList_Check failed
                {
                    result = -1;
                }

                // Release reference
                Py_DECREF(pResult);
            }
            else    // PyObject_CallFunctionObjArgs failed
            {
                result = Python_Error("PyObject_CallFunctionObjArgs failed!");
            }
            
            // Release our lock
            PyGILState_Release(state);
        }
        else    // Bad function pointer
        {
            result = EFAULT;
        }
    }
    else    // Invalid argument
    {
        result = EINVAL;
    }
    return result;
}

// =================================================================================================
//  SenseHAT_WaitForEvent
// =================================================================================================
int32_t SenseHAT_WaitForEvent (const tSenseHAT_Instance instance,
                               bool flushPendingEvents,
                               tSenseHAT_JoystickEvent* event)
{
	int32_t result = 0;

    // Check arguments
    if ((instance != NULL) &&
        (event != NULL))
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;

        memset(event, 0, sizeof(tSenseHAT_JoystickEvent));

        if (instancePrivate->waitForEventFunction != NULL)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();
            
            // Convert flush argument
            PyObject* pFlush = PyBool_FromLong((int32_t)flushPendingEvents);
            if (pFlush != NULL)
            {
                // Call the function
                PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->waitForEventFunction,
                                                                 pFlush,
                                                                 NULL);
                if (pResult != NULL)
                {
                    // The result should be an event tuple
                    if (PyTuple_Check(pResult))
                    {
                        result = SenseHAT_ParseJoystickEvent(pResult, event);
                    }
                    else    // PyTuple_Check failed
                    {
                        result = -1;
                    }

                    // Release reference
                    Py_DECREF(pResult);
                }
                else    // PyObject_CallFunctionObjArgs failed
                {
                    result = Python_Error("PyObject_CallFunctionObjArgs failed!");
                }

                // Release reference
                Py_DECREF(pFlush);
            }
            else    // PyBool_FromLong failed
            {
                result = Python_Error("PyBool_FromLong failed!");
            }

            // Release our lock
            PyGILState_Release(state);
        }
        else    // Bad function pointer
        {
            result = EFAULT;
        }
    }
    else    // Invalid argument
    {
        result = EINVAL;
    }
    return result;
}

// =================================================================================================
//  SenseHAT_ConvertPixelToLEDPixel
// =================================================================================================
int32_t SenseHAT_ConvertPixelToLEDPixel (const PyObject* pixel,
                                         tSenseHAT_LEDPixel* color)
{
    int32_t result = 0;
    PyObject* pChannel = NULL;

    pChannel = PyList_GetItem((PyObject*)pixel, 0);
    if (pChannel != NULL)
    {
        if (PYLONG_CHECK(pChannel))
        {
            color->red = PyLong_AsLong(pChannel);
        }
        else    // PyLong_Check failed
        {
            result = -1;
        }
        pChannel = NULL;
    }
    else    // PyList_GetItem failed
    {
        result = -1;
    }

    if (result == 0)
    {
        pChannel = PyList_GetItem((PyObject*)pixel, 1);
        if (pChannel != NULL)
        {
            if (PYLONG_CHECK(pChannel))
            {
                color->green = PyLong_AsLong(pChannel);
            }
            else    // PyLong_Check failed
            {
                result = -1;
            }
            pChannel = NULL;
        }
        else    // PyList_GetItem failed
        {
            result = -1;
        }
    }

    if (result == 0)
    {
        pChannel = PyList_GetItem((PyObject*)pixel, 2);
        if (pChannel != NULL)
        {
            if (PYLONG_CHECK(pChannel))
            {
                color->blue = PyLong_AsLong(pChannel);
            }
            else    // PyLong_Check failed
            {
                result = -1;
            }
            pChannel = NULL;
        }
        else    // PyList_GetItem
        {
            result = -1;
        }
    }
    return result;
}

// =================================================================================================
//  SenseHAT_ConvertPixelListToLEDPixelArray
// =================================================================================================
int32_t SenseHAT_ConvertPixelListToLEDPixelArray (const PyObject* pixelList,
                                                  tSenseHAT_LEDPixelArray pixelArray)
{
    int32_t result = 0;

    // Make sure we have a list
    if (PyList_Check((PyObject*)pixelList))
    {
        // Make sure the list is 64 elements long
        if (PyList_Size((PyObject*)pixelList) == 64)
        {
            uint32_t index = 0;
            PyObject* pPixel = NULL;

            // Iterate over the list for each pixel
            for (index = 0; index < 64; index++)
            {
                // Get the pixel
                pPixel = PyList_GetItem((PyObject*)pixelList, index);
                if (pPixel != NULL)
                {
                    // Make sure we got a list
                    if (PyList_Check(pPixel))
                    {
                        // Make sure the list is 3 elements long
                        if (PyList_Size(pPixel) == 3)
                        {
                            // Convert pixel to LED pixel
                            result = SenseHAT_ConvertPixelToLEDPixel(pPixel, &(pixelArray[index]));
                        }
                        else    // List isn't 3 elements long
                        {
                            result = -1;
                        }
                    }
                    else    // Not a list
                    {
                        result = 1;
                    }

                    // Release reference
                    Py_DECREF(pPixel);
                    pPixel = NULL;

                    if (result != 0)
                    {
                        break;
                    }
                }
                else    // PyList_GetItem failed
                {
                    result = Python_Error("PyList_GetItem failed!");
                    break;
                }
            }
        }
        else    // List isn't 64 elements long
        {
            result = -1;
        }
    }
    else    // Not a list
    {
        result = -1;
    }
    return result;
}

// =================================================================================================
//  SenseHAT_ConvertDictToOrientation
// =================================================================================================
int32_t SenseHAT_ConvertDictToOrientation (const PyObject* dict,
                                           tSenseHAT_Orientation* orientation)
{
    int32_t result = 0;

    // Make sure this is a dictionary
    if (PyDict_Check(dict))
    {
        // Get the pitch item
        PyObject* pValue = PyDict_GetItemString((PyObject*)dict, kOrientationPitch);
        if (pValue != NULL)
        {
            // Make sure it's a float
            if (PyFloat_Check(pValue))
            {
                // Get the value
                orientation->pitch = PyFloat_AsDouble(pValue);
            }
            else    // PyFloat_Check failed
            {
                result = -1;
            }

            // Release reference
            Py_DECREF(pValue);
            pValue = NULL;

            // Check for success
            if (result == 0)
            {
                // Get the roll item
                pValue = PyDict_GetItemString((PyObject*)dict, kOrientationRoll);
                if (pValue != NULL)
                {
                    // Make sure it's a float
                    if (PyFloat_Check(pValue))
                    {
                        // Get the value
                        orientation->roll = PyFloat_AsDouble(pValue);
                    }
                    else    // PyFloat_Check failed
                    {
                        result = -1;
                    }

                    // Release reference
                    Py_DECREF(pValue);
                    pValue = NULL;
                }
                else    // PyDict_GetItemString failed
                {
                    result = Python_Error("PyDict_GetItemString failed!");
                }
            }

            // Check for success
            if (result == 0)
            {
                // Get the yaw item
                pValue = PyDict_GetItemString((PyObject*)dict, kOrientationYaw);
                if (pValue != NULL)
                {
                    // Make sure it's a float
                    if (PyFloat_Check(pValue))
                    {
                        // Get the value
                        orientation->yaw = PyFloat_AsDouble(pValue);
                    }
                    else    // PyFloat_Check failed
                    {
                        result = -1;
                    }

                    // Release reference
                    Py_DECREF(pValue);
                    pValue = NULL;
                }
                else    // PyDict_GetItemString failed
                {
                    result = Python_Error("PyDict_GetItemString failed!");
                }
            }
        }
        else    // PyDict_GetItemString failed
        {
            result = Python_Error("PyDict_GetItemString failed!");
        }
    }
    else    // Not a dictionary
    {
        result = -1;
    }
    return result;
}

// =================================================================================================
//  SenseHAT_ConvertDictToRawData
// =================================================================================================
int32_t SenseHAT_ConvertDictToRawData (const PyObject* dict,
                                       tSenseHAT_RawData* rawData)
{
    int32_t result = 0;

    // Make sure this is a dictionary
    if (PyDict_Check(dict))
    {
        // Get the x item
        PyObject* pValue = PyDict_GetItemString((PyObject*)dict, kRawX);
        if (pValue != NULL)
        {
            // Make sure it's a float
            if (PyFloat_Check(pValue))
            {
                // Get the value
                rawData->x = PyFloat_AsDouble(pValue);
            }
            else    // PyFloat_Check failed
            {
                result = -1;
            }

            // Release reference
            Py_DECREF(pValue);
            pValue = NULL;

            // Check for success
            if (result == 0)
            {
                // Get the y item
                pValue = PyDict_GetItemString((PyObject*)dict, kRawY);
                if (pValue != NULL)
                {
                    // Make sure it's a float
                    if (PyFloat_Check(pValue))
                    {
                        // Get the value
                        rawData->y = PyFloat_AsDouble(pValue);
                    }
                    else    // PyFloat_Check failed
                    {
                        result = -1;
                    }

                    // Release reference
                    Py_DECREF(pValue);
                    pValue = NULL;
                }
                else    // PyDict_GetItemString failed
                {
                    result = Python_Error("PyDict_GetItemString failed!");
                }
            }

            // Check for success
            if (result == 0)
            {
                // Get the yaw item
                pValue = PyDict_GetItemString((PyObject*)dict, kRawZ);
                if (pValue != NULL)
                {
                    // Make sure it's a float
                    if (PyFloat_Check(pValue))
                    {
                        // Get the value
                        rawData->z = PyFloat_AsDouble(pValue);
                    }
                    else    // PyFloat_Check failed
                    {
                        result = -1;
                    }

                    // Release reference
                    Py_DECREF(pValue);
                    pValue = NULL;
                }
                else    // PyDict_GetItemString failed
                {
                    result = Python_Error("PyDict_GetItemString failed!");
                }
            }
        }
        else    // PyDict_GetItemString failed
        {
            result = Python_Error("PyDict_GetItemString failed!");
        }
    }
    else    // Not a dictionary
    {
        result = -1;
    }
    return result;
}

// =================================================================================================
//  SenseHAT_ParseJoystickEvent
// =================================================================================================
int32_t SenseHAT_ParseJoystickEvent (const PyObject* tuple,
                                     tSenseHAT_JoystickEvent* event)
{
    int32_t result = 0;
    PyObject* tupleItem = NULL;

    // Get the timestamp
    tupleItem = PyTuple_GetItem((PyObject*)tuple, 0);
    if (tupleItem != NULL)
    {
        if (PyFloat_Check(tupleItem))
        {
            event->timestamp = PyFloat_AsDouble(tupleItem);
        }
        else    // PyFloat_Check failed
        {
            result = -1;
        }

        //Py_DECREF(tupleItem);
        tupleItem = NULL;
    }
    else    // PyTuple_GetItem failed
    {
        result = Python_Error("PyTuple_GetItem failed!");
    }

    // Check for success
    if (result == 0)
    {
        // Get the direction
        tupleItem = PyTuple_GetItem((PyObject*)tuple, 1);
        if (tupleItem != NULL)
        {
            if (PyUnicode_Check(tupleItem))
            {
                char* directionStr = PYSTRING_AS_STRING(tupleItem);
                if (directionStr != NULL)
                {
                    if (strcmp(directionStr, "up") == 0)
                        event->direction = eSenseHAT_JoystickDirectionUp;
                    else if (strcmp(directionStr, "down") == 0)
                        event->direction = eSenseHAT_JoystickDirectionDown;
                    else if (strcmp(directionStr, "left") == 0)
                        event->direction = eSenseHAT_JoystickDirectionLeft;
                    else if (strcmp(directionStr, "right") == 0)
                        event->direction = eSenseHAT_JoystickDirectionRight;
                    else if (strcmp(directionStr, "middle") == 0)
						event->direction = eSenseHAT_JoystickDirectionPush;
                    else
                        event->direction = eSenseHAT_JoystickDirectionNone;
                }
                else    // Didn't get direction
                {
                    result = -1;
                }
            }
            else    // PyUnicode_Check failed
            {
                result = -1;
            }

            //Py_DECREF(tupleItem);
            tupleItem = NULL;
        }
        else    // PyTuple_GetItem failed
        {
            result = Python_Error("PyTuple_GetItem failed!");
        }
    }

    // Check for success
    if (result == 0)
    {
        // Get the action
        tupleItem = PyTuple_GetItem((PyObject*)tuple, 2);
        if (tupleItem != NULL)
        {
            if (PyUnicode_Check(tupleItem))
            {
                char* actionStr = PYSTRING_AS_STRING(tupleItem);
                if (actionStr != NULL)
                {
                    if (strcmp(actionStr, "pressed") == 0)
                    {
                        event->action = eSenseHAT_JoystickActionPressed;
                    }
                    else if (strcmp(actionStr, "released") == 0)
                    {
                        event->action = eSenseHAT_JoystickActionReleased;
                    }
                    else if (strcmp(actionStr, "held") == 0)
                    {
                        event->action = eSenseHAT_JoystickActionHeld;
                    }
                    else
                    {
                        event->action = eSenseHAT_JoystickActionNone;
                    }
                }
                else    // Didn't get action
                {
                    result = -1;
                }
            }
            else    // PyUnicode_Check failed
            {
                result = -1;
            }

            //Py_DECREF(tupleItem);
            tupleItem = NULL;
        }
        else    // PyTuple_GetItem failed
        { 
            result = Python_Error("PyTuple_GetItem failed!");
        }
    }
    return result;
}

// =================================================================================================
//  SenseHAT_Release
// =================================================================================================
int32_t SenseHAT_Release (tSenseHAT_InstancePrivate* instancePrivate)
{
    int32_t result = 0;

    // Check argument
    if (instancePrivate != NULL)
    {
        // Clean up
        if (instancePrivate->senseHATModule != NULL)
        {
            // Release stick submodule
            if (instancePrivate->stickSubModule != NULL)
            {
                Python_ReleaseFunctionReference(&(instancePrivate->getEventsFunction));
                Python_ReleaseFunctionReference(&(instancePrivate->waitForEventFunction));
                Python_ReleaseFunctionReference(&(instancePrivate->stickSubModule));                
            }
            
            // Release sensehat submodule
            if (instancePrivate->senseHATSubModule != NULL)
            {
                Python_ReleaseFunctionReference(&(instancePrivate->clearFunction));
                Python_ReleaseFunctionReference(&(instancePrivate->flipHorizontalFunction));
                Python_ReleaseFunctionReference(&(instancePrivate->flipVerticalFunction));
                #if 0
                Python_ReleaseFunctionReference(&(instancePrivate->gammaFunction));
                #endif
                Python_ReleaseFunctionReference(&(instancePrivate->gammaResetFunction));
                Python_ReleaseFunctionReference(&(instancePrivate->getAccelerometerFunction));
                Python_ReleaseFunctionReference(&(instancePrivate->getAccelerometerRawFunction));
                Python_ReleaseFunctionReference(&(instancePrivate->getCompassFunction));
                Python_ReleaseFunctionReference(&(instancePrivate->getCompassRawFunction));
                Python_ReleaseFunctionReference(&(instancePrivate->getGyroscopeFunction));
                Python_ReleaseFunctionReference(&(instancePrivate->getGyroscopeRawFunction));
                Python_ReleaseFunctionReference(&(instancePrivate->getHumidityFunction));
                Python_ReleaseFunctionReference(&(instancePrivate->getOrientationFunction));
                Python_ReleaseFunctionReference(&(instancePrivate->getOrientationDegreesFunction));
                Python_ReleaseFunctionReference(&(instancePrivate->getOrientationRadiansFunction));
                Python_ReleaseFunctionReference(&(instancePrivate->getPixelFunction));
                Python_ReleaseFunctionReference(&(instancePrivate->getPixelsFunction));
                Python_ReleaseFunctionReference(&(instancePrivate->getPressureFunction));
                Python_ReleaseFunctionReference(&(instancePrivate->getTemperatureFunction));
                Python_ReleaseFunctionReference(&(instancePrivate->getTemperatureFromHumidityFunction));
                Python_ReleaseFunctionReference(&(instancePrivate->getTemperatureFromPressureFunction));
                Python_ReleaseFunctionReference(&(instancePrivate->loadImageFunction));
                #if 0
                Python_ReleaseFunctionReference(&(instancePrivate->lowLightFunction));
                #endif
                Python_ReleaseFunctionReference(&(instancePrivate->setIMUConfigFunction));
                Python_ReleaseFunctionReference(&(instancePrivate->setPixelFunction));
                Python_ReleaseFunctionReference(&(instancePrivate->setPixelsFunction));
                Python_ReleaseFunctionReference(&(instancePrivate->setRotationFunction));
                Python_ReleaseFunctionReference(&(instancePrivate->showLetterFunction));
                Python_ReleaseFunctionReference(&(instancePrivate->showMessageFunction));
                Python_ReleaseFunctionReference(&(instancePrivate->self));
                Python_ReleaseFunctionReference(&(instancePrivate->senseHATSubModule));
            }

            Python_ReleaseFunctionReference(&(instancePrivate->senseHATModule));
        }
    }
    else    // Invalid argument
    {
        result = EINVAL;
    }
    return result;
}

// =================================================================================================
