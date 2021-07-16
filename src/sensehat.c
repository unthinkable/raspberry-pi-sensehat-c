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
#include "python_support.h"
#include "unthink_utils.h"

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

// SenseHAT_ValidateLEDPixelPosition
static int32_t SenseHAT_ValidateLEDPixelPosition (int32_t xPosition,
                                                  int32_t yPosition);

// SenseHAT_ValidateLEDPixelValue
static int32_t SenseHAT_ValidateLEDPixelValue (tSenseHAT_LEDPixel* pixel);

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
    int32_t result = UNTHINK_SUCCESS;

    // Check arguments
    result = UNTHINK_CHECK_CONDITION((instance != NULL), EINVAL);
    if (result == UNTHINK_SUCCESS)
    {
        tSenseHAT_InstancePrivate* instancePrivate = NULL;

        // Setup
        *instance = NULL;

        // Allocate space
        result = Unthink_AllocateMemory(sizeof(tSenseHAT_InstancePrivate), 
                                        (void**)&instancePrivate);
        if (result == UNTHINK_SUCCESS)
        {
            // Initialize
            Py_Initialize();

            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Get the Sense HAT module name
            PyObject* pName = PyUnicode_FromString(kSenseHAT_ModuleName);
            result = UNTHINK_CHECK_CONDITION((pName != NULL), EFAULT);
            if (result == UNTHINK_SUCCESS)
            {
                // Import the module
                instancePrivate->senseHATModule = PyImport_Import(pName);
                result = UNTHINK_CHECK_CONDITION((instancePrivate->senseHATModule != NULL), EFAULT);
                if (result == UNTHINK_SUCCESS) 
                {
                     // Get a reference to the Sense HAT submodule
                    instancePrivate->senseHATSubModule = 
                        PyObject_GetAttrString (instancePrivate->senseHATModule, kSenseHAT_SubmoduleName);
                    result = UNTHINK_CHECK_CONDITION((instancePrivate->senseHATSubModule != NULL), EFAULT);
                    if (result == UNTHINK_SUCCESS)
                    {
                        // Initialize the submodule
                        instancePrivate->self = PyObject_CallObject(instancePrivate->senseHATSubModule, NULL);
                        result = UNTHINK_CHECK_CONDITION((instancePrivate->self != NULL), EFAULT);
                        if (result == UNTHINK_SUCCESS)
                        { 
                            // Get a reference to the clear function
                            result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                 kClearFunctionName,
                                                                 &(instancePrivate->clearFunction));

                            // Check status
                            if (result == UNTHINK_SUCCESS) 
                            {
                                // Get a reference to the flip horizontal function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kFlipHorizontalFunctionName,
                                                                     &(instancePrivate->flipHorizontalFunction));
                            }

                            // Check status
                            if (result == UNTHINK_SUCCESS) 
                            {
                                // Get a reference to the flip vertical function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kFlipVerticalFunctionName,
                                                                     &(instancePrivate->flipVerticalFunction));
                            }

                            // Check status
                            if (result == UNTHINK_SUCCESS)
                            {
                                // Get a reference to the gamma reset function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kGammaResetFunctionName,
                                                                     &(instancePrivate->gammaResetFunction));
                            }

                            // Check status
                            if (result == UNTHINK_SUCCESS)
                            {
                                // Get a reference to the get accelerometer function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kGetAccelerometerFunctionName,
                                                                     &(instancePrivate->getAccelerometerFunction));
                            }

                            // Check status
                            if (result == UNTHINK_SUCCESS)
                            {
                                // Get a reference to the get accelerometer raw function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kGetAccelerometerRawFunctionName,
                                                                     &(instancePrivate->getAccelerometerRawFunction));
                            }

                            // Check status
                            if (result == UNTHINK_SUCCESS) 
                            {
                                // Get a reference to the get compass function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kGetCompassFunctionName,
                                                                     &(instancePrivate->getCompassFunction));
                            }
        
                            // Check status
                            if (result == UNTHINK_SUCCESS) 
                            {
                                // Get a reference to the get compass raw function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kGetCompassRawFunctionName,
                                                                     &(instancePrivate->getCompassRawFunction));
                            }
        
                            // Check status
                            if (result == UNTHINK_SUCCESS) 
                            {
                                // Get a reference to the get gyroscope function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kGetGyroscopeFunctionName,
                                                                     &(instancePrivate->getGyroscopeFunction));
                            }
        
                            // Check status
                            if (result == UNTHINK_SUCCESS) 
                            {
                                // Get a reference to the get gyroscope raw function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kGetGyroscopeRawFunctionName,
                                                                     &(instancePrivate->getGyroscopeRawFunction));
                            }
        
                            // Check status
                            if (result == UNTHINK_SUCCESS) 
                            {
                                // Get a reference to the get humidity function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kGetHumidityFunctionName,
                                                                     &(instancePrivate->getHumidityFunction));
                            }

                            // Check status
                            if (result == UNTHINK_SUCCESS) 
                            {
                                // Get a reference to the get orientation function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kGetOrientationFunctionName,
                                                                     &(instancePrivate->getOrientationFunction));
                            }

                            // Check status
                            if (result == UNTHINK_SUCCESS) 
                            {
                                // Get a reference to the get orientation degrees function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kGetOrientationDegreesFunctionName,
                                                                     &(instancePrivate->getOrientationDegreesFunction));
                            }

                            // Check status
                            if (result == UNTHINK_SUCCESS) 
                            {
                                // Get a reference to the get orientation radians function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kGetOrientationRadiansFunctionName,
                                                                     &(instancePrivate->getOrientationRadiansFunction));
                            }

                            // Check status
                            if (result == UNTHINK_SUCCESS) 
                            {
                                // Get a reference to the get pixel function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kGetPixelFunctionName,
                                                                     &(instancePrivate->getPixelFunction));
                            }

                            // Check status
                            if (result == UNTHINK_SUCCESS) 
                            {
                                // Get a reference to the get pixels function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kGetPixelsFunctionName,
                                                                     &(instancePrivate->getPixelsFunction));
                            }

                            // Check status
                            if (result == UNTHINK_SUCCESS) 
                            {
                                // Get a reference to the get pressure function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kGetPressureFunctionName,
                                                                     &(instancePrivate->getPressureFunction));
                            }

                            // Check status
                            if (result == UNTHINK_SUCCESS) 
                            {
                                // Get a reference to the get temperature function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kGetTemperatureFunctionName,
                                                                     &(instancePrivate->getTemperatureFunction));
                            }

                            // Check status
                            if (result == UNTHINK_SUCCESS) 
                            {
                                // Get a reference to the get temperature from humidity function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kGetTemperatureFromHumidityFunctionName,
                                                                     &(instancePrivate->getTemperatureFromHumidityFunction));
                            }

                            // Check status
                            if (result == UNTHINK_SUCCESS) 
                            {
                                // Get a reference to the get temperature from pressure function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kGetTemperatureFromPressureFunctionName,
                                                                     &(instancePrivate->getTemperatureFromPressureFunction));
                            }

                            // Check status
                            if (result == UNTHINK_SUCCESS) 
                            {
                                // Get a reference to the load image function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kLoadImageFunctionName,
                                                                     &(instancePrivate->loadImageFunction));
                            }

                            // Check status
                            if (result == UNTHINK_SUCCESS) 
                            {
                                // Get a reference to the set IMU config function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kSetIMUConfigFunctionName,
                                                                     &(instancePrivate->setIMUConfigFunction));
                            }

                            // Check status
                            if (result == UNTHINK_SUCCESS) 
                            {
                                // Get a reference to the set pixel function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kSetPixelFunctionName,
                                                                     &(instancePrivate->setPixelFunction));
                            }

                            // Check status
                            if (result == UNTHINK_SUCCESS) 
                            {
                                // Get a reference to the set pixels function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kSetPixelsFunctionName,
                                                                     &(instancePrivate->setPixelsFunction));
                            }

                            // Check status
                            if (result == UNTHINK_SUCCESS) 
                            {
                                // Get a reference to the set rotation function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kSetRotationFunctionName,
                                                                     &(instancePrivate->setRotationFunction));
                            }

                            // Check status
                            if (result == UNTHINK_SUCCESS) 
                            {
                                // Get a reference to the show letter function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kShowLetterFunctionName,
                                                                     &(instancePrivate->showLetterFunction));
                            }

                            // Check status
                            if (result == UNTHINK_SUCCESS) 
                            {
                                // Get a reference to the show message function
                                result = Python_GetFunctionReference(instancePrivate->senseHATSubModule,
                                                                     kShowMessageFunctionName,
                                                                     &(instancePrivate->showMessageFunction));
                            }

                            // Check status
                            if (result == UNTHINK_SUCCESS)
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
                                    if (result == UNTHINK_SUCCESS) 
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
                            (void)Python_Error("PyObject_CallObject failed!");
                    }
                    else    // PyObject_GetAttrString failed
                        (void)Python_Error("PyObject_GetAttrString failed!");
                }
                else	// PyImport_Import failed 
                    (void)Python_Error("PyImport_Import failed!");

                // Clean up
                Py_DECREF(pName);
            }
            else    // PyUnicode_FromString faield
                (void)Python_Error("PyUnicode_FromString failed!");

            // Restore current thread's Python state
            PyGILState_Release(state);

            if (result == UNTHINK_SUCCESS)
                *instance = (tSenseHAT_Instance)instancePrivate;

            else    // There was an error
            {
                // Clean up
                SenseHAT_Release(instancePrivate);
                Unthink_FreeMemory((void**)instancePrivate);

                // Close down the interpreter
                Python_CloseInterpreter();
            }
        }
    }
    return result;
}

// =================================================================================================
//  SenseHAT_Close
// =================================================================================================
int32_t SenseHAT_Close (tSenseHAT_Instance* instance)
{
    int32_t result = UNTHINK_SUCCESS;

    // Check arguments
    result = UNTHINK_CHECK_CONDITION((instance != NULL), EINVAL);
    if (result == UNTHINK_SUCCESS)
    {
        // Get private data
	    tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)(*instance);

        // Clean up 
        if (instancePrivate != NULL)
        {
            SenseHAT_Release(instancePrivate);
            (void)Unthink_FreeMemory((void**)&instancePrivate);
        }

        // Close down the interpreter
        Python_CloseInterpreter();
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
	int32_t result = UNTHINK_SUCCESS;

    // Check arguments
    result = UNTHINK_CHECK_CONDITION((instance != NULL), EINVAL);
    if (result == UNTHINK_SUCCESS)
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;
        result = UNTHINK_CHECK_CONDITION((instancePrivate->setRotationFunction != NULL), EFAULT);
        if (result == UNTHINK_SUCCESS)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Convert rotation argument
            PyObject* pRotation = Py_BuildValue("i", (int32_t)rotation);
            result = UNTHINK_CHECK_CONDITION((pRotation != NULL), ENOTSUP);
            if (result == UNTHINK_SUCCESS)
            {
                // Convert redraw argument
                PyObject* pRedraw = PyBool_FromLong((int32_t)redraw);
                result = UNTHINK_CHECK_CONDITION((pRedraw != NULL), ENOTSUP);
                if (result == UNTHINK_SUCCESS)
                {
                    // Call the function
                    PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->setRotationFunction,
                                                                     instancePrivate->self, 
                                                                     pRotation,
                                                                     pRedraw,
                                                                     NULL);
                    // Check status
                    result = UNTHINK_CHECK_CONDITION((pResult != NULL), UNTHINK_FAILURE);
                    if (result == UNTHINK_SUCCESS)
                    {
                        // Release reference
                        Py_DECREF(pResult);
                    }
                    else    // PyObject_CallFunctionObjArgs failed
                        (void)Python_Error("PyObject_CallFunctionObjArgs failed!");

                    // Release reference
                    Py_DECREF(pRedraw);
                }
                else    // PyBool_FromLong failed
                    (void)Python_Error("PyBool_FromLong failed!");

                // Release reference
                Py_DECREF(pRotation);
            }
            else    // Py_BuildValue failed  
                (void)Python_Error("Py_BuildValue failed!");

            // Release our lock
            PyGILState_Release(state);
        }
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
	int32_t result = UNTHINK_SUCCESS;

    // Check arguments
    result = UNTHINK_CHECK_CONDITION((instance != NULL), EINVAL);
    if (result == UNTHINK_SUCCESS)
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;
        result = UNTHINK_CHECK_CONDITION((instancePrivate->flipHorizontalFunction != NULL), EFAULT);
        if (result == UNTHINK_SUCCESS)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Convert redraw argument
            PyObject* pRedraw = PyBool_FromLong((int32_t)redraw);
            result = UNTHINK_CHECK_CONDITION((pRedraw != NULL), ENOTSUP);
            if (result == UNTHINK_SUCCESS)
            {
                // Call the function
                PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->flipHorizontalFunction,
                                                                 instancePrivate->self, 
                                                                 pRedraw,
                                                                 NULL);
                // Check status
                result = UNTHINK_CHECK_CONDITION((pResult != NULL), UNTHINK_FAILURE);
                if (result == UNTHINK_SUCCESS)
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
                    (void)Python_Error("PyObject_CallFunctionObjArgs failed!");

                // Release reference
                Py_DECREF(pRedraw);
            }
            else    // PyBool_FromLong failed  
                (void)Python_Error("PyBool_FromLong failed!");

            // Release our lock
            PyGILState_Release(state);
        }
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
	int32_t result = UNTHINK_SUCCESS;

    // Check arguments
    result = UNTHINK_CHECK_CONDITION((instance != NULL), EINVAL);
    if (result == UNTHINK_SUCCESS)
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;
        result = UNTHINK_CHECK_CONDITION((instancePrivate->flipVerticalFunction != NULL), EFAULT);
        if (result == UNTHINK_SUCCESS)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Convert redraw argument
            PyObject* pRedraw = PyBool_FromLong((int32_t)redraw);
            result = UNTHINK_CHECK_CONDITION((pRedraw != NULL), ENOTSUP);
            if (result == UNTHINK_SUCCESS)
            {
                // Call the function
                PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->flipVerticalFunction,
                                                                 instancePrivate->self, 
                                                                 pRedraw,
                                                                 NULL);
                // Check status
                result = UNTHINK_CHECK_CONDITION((pResult != NULL), ENOTSUP);
                if (result == UNTHINK_SUCCESS)
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
                    (void)Python_Error("PyObject_CallFunctionObjArgs failed!");

                // Release reference
                Py_DECREF(pRedraw);
            }
            else    // PyBool_FromLong failed  
                (void)Python_Error("PyBool_FromLong failed!");

            // Release our lock
            PyGILState_Release(state);
        }
    }
    return result;
}

// =================================================================================================
//  SenseHAT_LEDGammaReset
// =================================================================================================
int32_t SenseHAT_LEDGammaReset (const tSenseHAT_Instance instance)
{
    int32_t result = UNTHINK_SUCCESS;

    // Check arguments
    result = UNTHINK_CHECK_CONDITION((instance != NULL), EINVAL);
    if (result == UNTHINK_SUCCESS)
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;

        result = UNTHINK_CHECK_CONDITION((instancePrivate->gammaResetFunction != NULL), EFAULT);
        if (result == UNTHINK_SUCCESS)
        {
             // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Call the function
            PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->gammaResetFunction,
                                                             instancePrivate->self,
                                                             NULL);
            result = UNTHINK_CHECK_CONDITION((pResult != NULL), UNTHINK_FAILURE);
            if (result == UNTHINK_SUCCESS)
            {
                // Release reference
                Py_DECREF(pResult);
            }

            // Release our lock
            PyGILState_Release(state);
        }
    }
    return result;
}

// =================================================================================================
//  SenseHAT_LEDSetPixels
// =================================================================================================
int32_t SenseHAT_LEDSetPixels (const tSenseHAT_Instance instance,
                               const tSenseHAT_LEDPixelArray pixels)
{
	int32_t result = UNTHINK_SUCCESS;

    // Check argument
    result = UNTHINK_CHECK_CONDITION((instance != NULL), EINVAL);
    if (result == UNTHINK_SUCCESS)
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;

        result = UNTHINK_CHECK_CONDITION((instancePrivate->setPixelsFunction != NULL), EFAULT);
        if (result == UNTHINK_SUCCESS)
        {
             // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Create pixels list
            PyObject* pPixels = PyList_New(0);
            result = UNTHINK_CHECK_CONDITION((pPixels != NULL), UNTHINK_FAILURE);
            if (result == UNTHINK_SUCCESS)
            {
                uint_fast32_t index = 0;
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
                        result = SenseHAT_ValidateLEDPixelValue((tSenseHAT_LEDPixel*)&pixel);
                        if (result == UNTHINK_SUCCESS)
                        {
                            pixel.red = pixels[index].red;
                            pixel.green = pixels[index].green;
                            pixel.blue = pixels[index].blue;
                        }
                    }

                    // Check status
                    if (result == UNTHINK_SUCCESS)
                    {
                        // Create list of pixel color components
                        PyObject* pPixel = PyList_New(0);
                        result = UNTHINK_CHECK_CONDITION((pPixel != NULL), UNTHINK_FAILURE);
                        if (result == UNTHINK_SUCCESS)
                        {
                            // Add red component
                            PyObject* pRed = Py_BuildValue("i", pixel.red);

                            // Check status
                            result = UNTHINK_CHECK_CONDITION((pRed != NULL), UNTHINK_FAILURE);
                            if (result == UNTHINK_SUCCESS)
                            {
                                // Append red component to pixel component list
                                res = PyList_Append(pPixel, pRed);
                                result = UNTHINK_CHECK_CONDITION((res == UNTHINK_SUCCESS), res);

                                // Release reference
                                Py_DECREF(pRed);
                            }

                            // Check status
                            if (result == UNTHINK_SUCCESS)
                            {
                                // Add green component
                                PyObject* pGreen = Py_BuildValue("i", pixel.green);

                                // Check status
                                result = UNTHINK_CHECK_CONDITION((pGreen != NULL), UNTHINK_FAILURE);
                                if (result == UNTHINK_SUCCESS)
                                {
                                    // Append green component to pixel component list
                                    res = PyList_Append(pPixel, pGreen);
                                    result = UNTHINK_CHECK_CONDITION((res == UNTHINK_SUCCESS), res);

                                    // Release reference
                                    Py_DECREF(pGreen);
                                }
                            }
                        
                            // Check status
                            if (result == UNTHINK_SUCCESS)
                            {
                                // Add blue component
                                PyObject* pBlue = Py_BuildValue("i", pixel.blue);

                                // Check status
                                result = UNTHINK_CHECK_CONDITION((pBlue != NULL), UNTHINK_FAILURE);
                                if (result == UNTHINK_SUCCESS)
                                {
                                    // Append blue component to pixel component list
                                    res = PyList_Append(pPixel, pBlue);
                                    result = UNTHINK_CHECK_CONDITION((res == UNTHINK_SUCCESS), res);

                                    // Release reference
                                    Py_DECREF(pBlue);
                                }
                            }

                            // Check status
                            if (result == UNTHINK_SUCCESS)
                            {
                                // Make sure the list is the correct size
                                result = UNTHINK_CHECK_CONDITION((PyList_Size(pPixel) == 3), UNTHINK_FAILURE);
                                if (result == UNTHINK_SUCCESS)
                                {
                                    // Append pixel component list to pixel list
                                    res = PyList_Append(pPixels, pPixel);
                                    result = UNTHINK_CHECK_CONDITION((res == UNTHINK_SUCCESS), res);
                                }
                            }

                            // Release reference
                            Py_DECREF(pPixel);
                        }
                        else    // PyList_New failed
                            (void)Python_Error("PyList_New failed!");
                    }

                    // If there's an error, break
                    if (result != UNTHINK_SUCCESS)  
                        break;
                }       

                // Check status
                if (result == UNTHINK_SUCCESS)
                {
                    // Make sure the pixel list is the correct size
                    if (PyList_Size(pPixels) == 64)
                    {
                        // Call the function
                        PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->setPixelsFunction,
                                                                         instancePrivate->self, 
                                                                         pPixels,
                                                                         NULL);
                        result = UNTHINK_CHECK_CONDITION((pResult != NULL), UNTHINK_FAILURE);
                        if (result == UNTHINK_SUCCESS)
                        {
                            // Release reference
                            Py_DECREF(pResult);
                        }
                        else    // PyObject_CallFunctionObjArgs failed
                            (void)Python_Error("PyObject_CallFunctionObjArgs failed!");
                    }
                }

                // Release reference
                Py_DECREF(pPixels);
            }
            else    // PyList_New failed
                (void)Python_Error("PyList_New failed!");

            // Release our lock
            PyGILState_Release(state);
        }
    }
    return result;
}

// =================================================================================================
//  SenseHAT_LEDGetPixels
// =================================================================================================
int32_t SenseHAT_LEDGetPixels (const tSenseHAT_Instance instance,
                               tSenseHAT_LEDPixelArray pixels)
{
	int32_t result = UNTHINK_SUCCESS;

    // Check arguments
    result = UNTHINK_CHECK_CONDITION((instance != NULL), EINVAL);
    if (result == UNTHINK_SUCCESS)
        result = UNTHINK_CHECK_CONDITION((pixels != NULL), EINVAL);

    // Check status
    if (result == UNTHINK_SUCCESS)
    {
        // Initialize array
        memset((void*)pixels, 0, sizeof(tSenseHAT_LEDPixelArray));

        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;

        result = UNTHINK_CHECK_CONDITION((instancePrivate->getPixelsFunction != NULL), EFAULT);
        if (result == UNTHINK_SUCCESS)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();
            
            // Call the function
            PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->getPixelsFunction,
                                                             instancePrivate->self, NULL);
            
            // Check status
            result = UNTHINK_CHECK_CONDITION((pResult != NULL), UNTHINK_FAILURE);
            if (result == UNTHINK_SUCCESS)
            {
                // Convert to pixel array
                result = SenseHAT_ConvertPixelListToLEDPixelArray(pResult, pixels);

                // Release reference
                Py_DECREF(pResult);
            }
            else    // PyObject_CallFunctionObjArgs failed
                (void)Python_Error("PyObject_CallFunctionObjArgs failed!");

            // Release our lock
            PyGILState_Release(state);
        }
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
	int32_t result = UNTHINK_SUCCESS;

    // Check arguments
    result = UNTHINK_CHECK_CONDITION((instance != NULL), EINVAL);
    if (result == UNTHINK_SUCCESS)
        result = SenseHAT_ValidateLEDPixelPosition(xPosition,
                                                   yPosition);

    // Check status
    if (result == UNTHINK_SUCCESS)
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;

        result = UNTHINK_CHECK_CONDITION((instancePrivate->setPixelFunction != NULL), EFAULT);
        if (result == UNTHINK_SUCCESS)
        {
             // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Convert x argument
            PyObject* pXPos = Py_BuildValue("i", xPosition);
            result = UNTHINK_CHECK_CONDITION((pXPos != NULL), UNTHINK_FAILURE);
            if (result == UNTHINK_SUCCESS)
            {
                // Convert y argument
                PyObject* pYPos = Py_BuildValue("i", yPosition);
                result = UNTHINK_CHECK_CONDITION((pYPos != NULL), UNTHINK_FAILURE);
                if (result == UNTHINK_SUCCESS)
                {
                    // Convert color argument
                    PyObject* pColor = NULL;

                    // Was a user specified pixel provided?
                    if (color != NULL)
                    {
                        // Check pixel for validity
                        result = SenseHAT_ValidateLEDPixelValue((tSenseHAT_LEDPixel*)color);
                        if (result == UNTHINK_SUCCESS)
                        {
                            // Convert pixel to pixel component list
                            pColor = Py_BuildValue("(lll)",
                                                   color->red,
                                                   color->green,
                                                   color->blue);
                        }
                    }
                    else    // Use default values for pixel component list
                        pColor = Py_BuildValue("(lll)", 0, 0, 0);

                    // Check status
                    if (result == UNTHINK_SUCCESS)
                    {
                        // Check for failure
                        result = UNTHINK_CHECK_CONDITION((pColor != NULL), UNTHINK_FAILURE);
                        if (result != UNTHINK_SUCCESS)
                            (void)Python_Error("Py_BuildValue failed!");
                    }

                    // Make sure we got a color object
                    if (result == UNTHINK_SUCCESS)
                    {
                        // Call the function
                        PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->setPixelFunction,
                                                                         instancePrivate->self, 
                                                                         pXPos,
                                                                         pYPos,
                                                                         pColor,
                                                                         NULL);
                        result = UNTHINK_CHECK_CONDITION((pResult != NULL), UNTHINK_FAILURE);
                        if (result == UNTHINK_SUCCESS)
                        {
                            // Release reference
                            Py_DECREF(pResult);
                        }
                        else    // PyObject_CallFunctionObjArgs failed
                            (void)Python_Error("PyObject_CallFunctionObjArgs failed!");

                        // Release reference
                        Py_DECREF(pColor);
                    }
                    
                    // Release reference
                    Py_DECREF(pYPos);
                }
                else    // Py_BuildValue failed  
                    (void)Python_Error("Py_BuildValue failed!");

                // Release reference
                Py_DECREF(pXPos);
            }
            else    // Py_BuildValue failed
                (void)Python_Error("Py_BuildValue failed!");

            // Release our lock
            PyGILState_Release(state);
       }
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

	int32_t result = UNTHINK_SUCCESS;

    // Check arguments
    result = UNTHINK_CHECK_CONDITION((instance != NULL), EINVAL);
    if (result == UNTHINK_SUCCESS)
    {
        result = SenseHAT_ValidateLEDPixelPosition(xPosition,
                                                   yPosition);
        if (result == UNTHINK_SUCCESS)
            result = UNTHINK_CHECK_CONDITION((color != NULL), EINVAL);
    }

    // Check status
    if (result == UNTHINK_SUCCESS)
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;

        // Initialize pixel color
        memset(color, 0, sizeof(tSenseHAT_LEDPixel));

        result = UNTHINK_CHECK_CONDITION((instancePrivate->getPixelFunction != NULL), EFAULT);
        if (result == UNTHINK_SUCCESS)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Convert x argument
            PyObject* pXPos = Py_BuildValue("i", xPosition);
            result = UNTHINK_CHECK_CONDITION((pXPos != NULL), UNTHINK_FAILURE);
            if (result == UNTHINK_SUCCESS)
            {
                // Convert y argument
                PyObject* pYPos = Py_BuildValue("i", yPosition);
                result = UNTHINK_CHECK_CONDITION((pYPos != NULL), UNTHINK_FAILURE);
                if (result == UNTHINK_SUCCESS)
                {
                    // Call the function
                    PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->getPixelFunction,
                                                                     instancePrivate->self, 
                                                                     pXPos,
                                                                     pYPos,
                                                                     NULL);

                    // Check status
                    result = UNTHINK_CHECK_CONDITION((pResult != NULL), UNTHINK_FAILURE);
                    if (result == UNTHINK_SUCCESS)
                    {
                        // Result should be a list
                        result = UNTHINK_CHECK_CONDITION((PyList_Check(pResult)), UNTHINK_FAILURE);
                        if (result == UNTHINK_SUCCESS)
                        {
                            // The list should have 3 elements
                            result = UNTHINK_CHECK_CONDITION((PyList_Size(pResult) == 3), UNTHINK_FAILURE);
                            if (result == UNTHINK_SUCCESS)
                            {
                                // Convert pixel component list to pixel
                                result = SenseHAT_ConvertPixelToLEDPixel(pResult, color);
                            }
                        }
                        
                        // Release reference
                        Py_DECREF(pResult);
                    }
                    else    // PyObject_CallFunctionObjArgs failed
                        (void)Python_Error("PyObject_CallFunctionObjArgs failed!");

                    // Release reference
                    Py_DECREF(pYPos);
                }
                else    // Py_BuildValue failed  
                    (void)Python_Error("Py_BuildValue failed!");

                // Release reference
                Py_DECREF(pXPos);
            }
            else    // Py_BuildValue failed
                (void)Python_Error("Py_BuildValue failed!");

            // Release our lock
            PyGILState_Release(state);
        }
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
	int32_t result = UNTHINK_SUCCESS;

    // Check arguments
    result = UNTHINK_CHECK_CONDITION((instance != NULL), EINVAL);
    if (result == UNTHINK_SUCCESS)
    {
        result = UNTHINK_CHECK_CONDITION((imageFilePath != NULL), EINVAL);
        if (result == UNTHINK_SUCCESS)
            result = UNTHINK_CHECK_CONDITION((strlen(imageFilePath) > 0), EINVAL);
    }

    // Check status
    if (result == UNTHINK_SUCCESS)
    {
        // Make sure file path points to a file
        bool fileExists = false;
        FILE* fp = fopen(imageFilePath, "rb");
        result = UNTHINK_CHECK_CONDITION((fp != NULL), errno);
        if (result == UNTHINK_SUCCESS)
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

            result = UNTHINK_CHECK_CONDITION((instancePrivate->loadImageFunction != NULL), EFAULT);
            if (result == UNTHINK_SUCCESS)
            {
                // Get a lock
                PyGILState_STATE state = PyGILState_Ensure();

                // Convert path argument
                PyObject* pPath = PyUnicode_DecodeUTF8(imageFilePath, strlen(imageFilePath), NULL);
                result = UNTHINK_CHECK_CONDITION((pPath != NULL), UNTHINK_FAILURE);
                if (result == UNTHINK_SUCCESS)
                {
                    // Convert redraw argument
                    PyObject* pRedraw = PyBool_FromLong((int32_t)redraw);
                    result = UNTHINK_CHECK_CONDITION((pRedraw != NULL), UNTHINK_FAILURE);
                    if (result == UNTHINK_SUCCESS)
                    {
                        // Call the function
                        PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->loadImageFunction,
                                                                         instancePrivate->self, 
                                                                         pPath,
                                                                         pRedraw,
                                                                         NULL);

                        // Check status
                        result = UNTHINK_CHECK_CONDITION((pResult != NULL), UNTHINK_FAILURE);
                        if (result == UNTHINK_SUCCESS)
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
                            (void)Python_Error("PyObject_CallFunctionObjArgs failed!");

                        // Release reference
                        Py_DECREF(pRedraw);
                    }
                    else    // PyBool_FromLong failed 
                        (void)Python_Error("PyBool_FromLong failed!");

                    // Release reference
                    Py_DECREF(pPath);
                }
                else    // PyUnicode_DecodeUTF8 failed 
                    (void)Python_Error("PyUnicode_DecodeUTF8 failed!");

                // Release our lock
                PyGILState_Release(state);
            }
        }
    }
    return result;
}

// =================================================================================================
//  SenseHAT_LEDClear
// =================================================================================================
int32_t SenseHAT_LEDClear (const tSenseHAT_Instance instance,
                           const tSenseHAT_LEDPixel* color)
{
	int32_t result = UNTHINK_SUCCESS;

    // Check arguments
    result = UNTHINK_CHECK_CONDITION((instance != NULL), EINVAL);
    if (result == UNTHINK_SUCCESS)
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;

        result = UNTHINK_CHECK_CONDITION((instancePrivate->clearFunction != NULL), EFAULT);
        if (result == UNTHINK_SUCCESS)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Convert color argument
            PyObject* pColor = NULL;

            // Did caller provide pixel color?
            if (color != NULL)
            {
                // Check pixel validity
                result = SenseHAT_ValidateLEDPixelValue((tSenseHAT_LEDPixel*)color);
                if (result == UNTHINK_SUCCESS)
                {
                    // Build the pixel color component list
                    pColor = Py_BuildValue("(lll)",
                                           color->red,
                                           color->green,
                                           color->blue);
                }
            }
            else    // Use default pixel color (black)
            {
                // Build the pixel color component list
                pColor = Py_BuildValue("(lll)", 0L, 0L, 0L);
            }

            // Check status
            if (result == UNTHINK_SUCCESS)
            {
                // Make sure we got a color object
                result = UNTHINK_CHECK_CONDITION((pColor != NULL), UNTHINK_FAILURE);
                if (result == UNTHINK_SUCCESS)
                {
                    // Call the function
                    PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->clearFunction,
                                                                    instancePrivate->self, 
                                                                    pColor,
                                                                    NULL);
                    result = UNTHINK_CHECK_CONDITION((pResult != NULL), UNTHINK_FAILURE);
                    if (result == UNTHINK_SUCCESS)
                    {
                        // Release reference
                        Py_DECREF(pResult);
                    }
                    else    // PyObject_CallFunctionObjArgs failed
                        (void)Python_Error("PyObject_CallFunctionObjArgs failed!");

                    // Release reference
                    Py_DECREF(pColor);
                }
                else    // Py_BuildValue failed
                    (void)Python_Error("Py_BuildValue failed!");
            }

            // Release our lock
            PyGILState_Release(state);
        }
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
	int32_t result = UNTHINK_SUCCESS;

    // Check arguments
    result = UNTHINK_CHECK_CONDITION((instance != NULL), EINVAL);
    if (result == UNTHINK_SUCCESS)
    {
        result = UNTHINK_CHECK_CONDITION((letter != NULL), EINVAL);
        if (result == UNTHINK_SUCCESS)
            result = UNTHINK_CHECK_CONDITION((strlen(letter) == 1), EINVAL);
    }

    // Check status
    if (result == UNTHINK_SUCCESS)
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;

        result = UNTHINK_CHECK_CONDITION((instancePrivate->showLetterFunction != NULL), EFAULT);
        if (result == UNTHINK_SUCCESS)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Convert letter argument
            PyObject* pLetter = PyUnicode_DecodeUTF8(letter, strlen(letter), NULL);
            result = UNTHINK_CHECK_CONDITION((pLetter != NULL), UNTHINK_FAILURE);
            if (result == UNTHINK_SUCCESS)
            {
                PyObject* pTextColor = NULL;
                PyObject* pBackColor = NULL;

                // Convert text color argument
                if (textColor != NULL)
                {
                    result = SenseHAT_ValidateLEDPixelValue((tSenseHAT_LEDPixel*)textColor);
                    if (result == UNTHINK_SUCCESS)
                    {
                        pTextColor = Py_BuildValue("(lll)",
                                                   textColor->red,
                                                   textColor->green,
                                                   textColor->blue);
                    }
                }
                else    // Default to white
                    pTextColor = Py_BuildValue("(lll)", 255L, 255L, 255L);

                // Check status
                if (result == UNTHINK_SUCCESS)
                {
                    result = UNTHINK_CHECK_CONDITION((pTextColor != NULL), UNTHINK_FAILURE);
                    if (result != UNTHINK_SUCCESS)
                        (void)Python_Error("Py_BuildValue failed!");
                }
                
                // Check status
                if (result == UNTHINK_SUCCESS)
                {
                    // Convert back color argument
                    if (backColor != NULL)
                    {
                        result = SenseHAT_ValidateLEDPixelValue((tSenseHAT_LEDPixel*)backColor);
                        if (result == UNTHINK_SUCCESS)
                        {
                            pBackColor = Py_BuildValue("(lll)",
                                                       backColor->red,
                                                       backColor->green,
                                                       backColor->blue);
                        }
                    }
                    else    // Default to black
                        pBackColor = Py_BuildValue("(lll)", 0L, 0L, 0L);
                }

                // Check status
                if (result == UNTHINK_SUCCESS)
                {
                    result = UNTHINK_CHECK_CONDITION((pBackColor != NULL), UNTHINK_FAILURE);
                    if (result != UNTHINK_SUCCESS)
                        (void)Python_Error("Py_BuildValue failed!");

                }

                // Check status
                if (result == UNTHINK_SUCCESS)
                {
                    // Call the function
                    PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->showLetterFunction,
                                                                     instancePrivate->self, 
                                                                     pLetter,
                                                                     pTextColor,
                                                                     pBackColor,
                                                                     NULL);
                    result = UNTHINK_CHECK_CONDITION((pResult != NULL), UNTHINK_FAILURE);
                    if (result == UNTHINK_SUCCESS)
                    {
                        // Release reference
                        Py_DECREF(pResult);
                    }
                    else    // PyObject_CallFunctionObjArgs failed
                        (void)Python_Error("PyObject_CallFunctionObjArgs failed!");
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
                (void)Python_Error("PyUnicode_DecodeUTF8 failed!");

            // Release our lock
            PyGILState_Release(state);
        }
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
	int32_t result = UNTHINK_SUCCESS;

    // Check arguments
    result = UNTHINK_CHECK_CONDITION((instance != NULL), EINVAL);
    if (result == UNTHINK_SUCCESS)
    {
        result = UNTHINK_CHECK_CONDITION((message != NULL), EINVAL);
        if (result == UNTHINK_SUCCESS)
        {
            result = UNTHINK_CHECK_CONDITION((strlen(message) > 0), EINVAL);
            if (result == UNTHINK_SUCCESS)
                result = UNTHINK_CHECK_CONDITION((scrollSpeed >= 0), EINVAL);
        }
    }

    // Check status
    if (result == UNTHINK_SUCCESS)
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;

        result = UNTHINK_CHECK_CONDITION((instancePrivate->showMessageFunction != NULL), EFAULT);
        if (result == UNTHINK_SUCCESS)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Convert message argument
            PyObject* pMessage = PyUnicode_DecodeUTF8(message, strlen(message), NULL);
            result = UNTHINK_CHECK_CONDITION((pMessage != NULL), UNTHINK_FAILURE);
            if (result == UNTHINK_SUCCESS)
            {
                // Convert scroll speed argument
                PyObject* pScrollSpeed = PyFloat_FromDouble(scrollSpeed);
                result = UNTHINK_CHECK_CONDITION((pScrollSpeed != NULL), UNTHINK_FAILURE);
                if (result == UNTHINK_SUCCESS)
                {
                    PyObject* pTextColor = NULL;
                    PyObject* pBackColor = NULL;

                    // Convert text color argument
                    if (textColor != NULL)
                    {
                        // Check validity
                        result = SenseHAT_ValidateLEDPixelValue((tSenseHAT_LEDPixel*)textColor);
                        if (result == UNTHINK_SUCCESS)
                        {
                            pTextColor = Py_BuildValue("(lll)",
                                                       textColor->red,
                                                       textColor->green,
                                                       textColor->blue);
                        }
                    }
                    else    // Default to white
                        pTextColor = Py_BuildValue("(lll)", 255L, 255L, 255L);

                    // Check status
                    if (result == UNTHINK_SUCCESS)
                    {
                        result = UNTHINK_CHECK_CONDITION((pTextColor != NULL), UNTHINK_FAILURE);
                        if (result != UNTHINK_SUCCESS)
                            (void)Python_Error("Py_BuildValue failed!");
                    }

                    // Check status
                    if (result == UNTHINK_SUCCESS)
                    {
                        // Convert back color argument
                        if (backColor != NULL)
                        {
                            // Check validity
                            result = SenseHAT_ValidateLEDPixelValue((tSenseHAT_LEDPixel*)backColor);
                            if (result == UNTHINK_SUCCESS)
                            {
                                pBackColor = Py_BuildValue("(lll)",
                                                           backColor->red,
                                                           backColor->green,
                                                           backColor->blue);
                            }
                        }
                        else    // Default to black
                            pBackColor = Py_BuildValue("(lll)", 0L, 0L, 0L);

                        // Check status
                        if (result == UNTHINK_SUCCESS)
                        {
                            result = UNTHINK_CHECK_CONDITION((pBackColor != NULL), UNTHINK_FAILURE);
                            if (result != UNTHINK_SUCCESS)
                                (void)Python_Error("Py_BuildValue failed!");
                        }
                    }

                    // Check status
                    if (result == UNTHINK_SUCCESS)
                    {
                        // Call the function
                        PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->showMessageFunction,
                                                                         instancePrivate->self, 
                                                                         pMessage,
                                                                         pScrollSpeed,
                                                                         pTextColor,
                                                                         pBackColor,
                                                                         NULL);
                        result = UNTHINK_CHECK_CONDITION((pResult != NULL), UNTHINK_FAILURE);
                        if (result == UNTHINK_SUCCESS)
                        {
                            // Release reference
                            Py_DECREF(pResult);
                        }
                        else    // PyObject_CallFunctionObjArgs failed
                            (void)Python_Error("PyObject_CallFunctionObjArgs failed!");
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
                    (void)Python_Error("PyFloat_FromDouble failed!");

                // Release reference
                Py_DECREF(pMessage);
            }
            else    // PyUnicode_DecodeUTF8 failed
                (void)Python_Error("PyUnicode_DecodeUTF8 failed!");

            // Release our lock
            PyGILState_Release(state);
        }
    }
    return result;
}

// =================================================================================================
//  SenseHAT_GetHumidity
// =================================================================================================
int32_t SenseHAT_GetHumidity (const tSenseHAT_Instance instance,
                              double* percentRelativeHumidity)
{
	int32_t result = UNTHINK_SUCCESS;

    // Check arguments
    result = UNTHINK_CHECK_CONDITION((instance != NULL), EINVAL);
    if (result == UNTHINK_SUCCESS)
        result = UNTHINK_CHECK_CONDITION((percentRelativeHumidity != NULL), EINVAL);

    // Check status
    if (result == UNTHINK_SUCCESS)
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;

        // Setup
        *percentRelativeHumidity = 0;

        result = UNTHINK_CHECK_CONDITION((instancePrivate->getHumidityFunction != NULL), EFAULT);
        if (result == UNTHINK_SUCCESS)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Call the function
            PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->getHumidityFunction,
                                                             instancePrivate->self, NULL);
            result = UNTHINK_CHECK_CONDITION((pResult != NULL), UNTHINK_FAILURE);
            if (result == UNTHINK_SUCCESS)
            {
                // Get the result
                result = UNTHINK_CHECK_CONDITION((PyFloat_Check(pResult)), UNTHINK_FAILURE);
                if (result == UNTHINK_SUCCESS)
                    *percentRelativeHumidity = PyFloat_AsDouble(pResult);
                
                // Release reference
                Py_DECREF(pResult);
            }
            else    // PyObject_CallFunctionObjArgs failed
                (void)Python_Error("PyObject_CallFunctionObjArgs failed!");

            // Release our lock
            PyGILState_Release(state);
        }
    }
	return result;
}

// =================================================================================================
//  SenseHAT_GetTemperature
// =================================================================================================
int32_t SenseHAT_GetTemperature (const tSenseHAT_Instance instance,
                                 double* degreesCelsius)
{
	int32_t result = UNTHINK_SUCCESS;

    // Check arguments
    result = UNTHINK_CHECK_CONDITION((instance != NULL), EINVAL);
    if (result == UNTHINK_SUCCESS)
        result = UNTHINK_CHECK_CONDITION((degreesCelsius != NULL), EINVAL);

    // Check status
    if (result == UNTHINK_SUCCESS)
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;

        // Setup
        *degreesCelsius = 0;

        result = UNTHINK_CHECK_CONDITION((instancePrivate->getTemperatureFunction != NULL), EFAULT);
        if (result == UNTHINK_SUCCESS)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Call the function
            PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->getTemperatureFunction,
                                                             instancePrivate->self, NULL);
            result = UNTHINK_CHECK_CONDITION((pResult != NULL), UNTHINK_FAILURE);
            if (result == UNTHINK_SUCCESS)
            {
                // Get the result
                result = UNTHINK_CHECK_CONDITION((PyFloat_Check(pResult)), UNTHINK_FAILURE);
                if (result == UNTHINK_SUCCESS)
                    *degreesCelsius = PyFloat_AsDouble(pResult);

                // Release reference
                Py_DECREF(pResult);
            }
            else    // PyObject_CallFunctionObjArgs failed
                (void)Python_Error("PyObject_CallFunctionObjArgs failed!");

            // Release our lock
            PyGILState_Release(state);
        }
    }
	return result;
}

// =================================================================================================
//  SenseHAT_GetPressure
// =================================================================================================
int32_t SenseHAT_GetPressure (const tSenseHAT_Instance instance,
                              double* millibars)
{
	int32_t result = UNTHINK_SUCCESS;

    // Check arguments
    result = UNTHINK_CHECK_CONDITION((instance != NULL), EINVAL);
    if (result == UNTHINK_SUCCESS)
        result = UNTHINK_CHECK_CONDITION((millibars != NULL), EINVAL);

    // Check status
    if (result == UNTHINK_SUCCESS)
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;

        // Setup
        *millibars = 0;

        result = UNTHINK_CHECK_CONDITION((instancePrivate->getPressureFunction != NULL), EFAULT);
        if (result == UNTHINK_SUCCESS)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Call the function
            PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->getPressureFunction,
                                                             instancePrivate->self, NULL);
            result = UNTHINK_CHECK_CONDITION((pResult != NULL), UNTHINK_FAILURE);
            if (result == UNTHINK_SUCCESS)
            {
                // Get the result
                result = UNTHINK_CHECK_CONDITION((PyFloat_Check(pResult)), UNTHINK_FAILURE);
                if (result == UNTHINK_SUCCESS)
                    *millibars = PyFloat_AsDouble(pResult);

                // Release reference
                Py_DECREF(pResult);
            }
            else    // PyObject_CallFunctionObjArgs failed
                (void)Python_Error("PyObject_CallFunctionObjArgs failed!");

            // Release our lock
            PyGILState_Release(state);
        }
    }
	return result;
}

// =================================================================================================
//  SenseHAT_GetCompass
// =================================================================================================
int32_t SenseHAT_GetCompass (const tSenseHAT_Instance instance,
                             double* degrees)
{
	int32_t result = UNTHINK_SUCCESS;

    // Check arguments
    result = UNTHINK_CHECK_CONDITION((instance != NULL), EINVAL);
    if (result == UNTHINK_SUCCESS)
        result = UNTHINK_CHECK_CONDITION((degrees != NULL), EINVAL);

    // Check status
    if (result == UNTHINK_SUCCESS)
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;

        // Setup
        *degrees = 0;

        result = UNTHINK_CHECK_CONDITION((instancePrivate->getCompassFunction != NULL), EFAULT);
        if (result == UNTHINK_SUCCESS)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Call the function
            PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->getCompassFunction,
                                                             instancePrivate->self, NULL);
            result = UNTHINK_CHECK_CONDITION((pResult != NULL), UNTHINK_FAILURE);
            if (result == UNTHINK_SUCCESS)
            {
                // Get the result
                result = UNTHINK_CHECK_CONDITION((PyFloat_Check(pResult)), UNTHINK_FAILURE);
                if (result == UNTHINK_SUCCESS)
                    *degrees = PyFloat_AsDouble(pResult);

                // Release reference
                Py_DECREF(pResult);
            }
            else    // PyObject_CallFunctionObjArgs failed
                (void)Python_Error("PyObject_CallFunctionObjArgs failed!");

            // Release our lock
            PyGILState_Release(state);
        }
    }
	return result;
}

// =================================================================================================
//  SenseHAT_GetAccelerometer
// =================================================================================================
int32_t SenseHAT_GetAccelerometer (const tSenseHAT_Instance instance,
                                   tSenseHAT_Orientation* orientation)
{
	int32_t result = UNTHINK_SUCCESS;

    // Check arguments
    result = UNTHINK_CHECK_CONDITION((instance != NULL), EINVAL);
    if (result == UNTHINK_SUCCESS)
        result = UNTHINK_CHECK_CONDITION((orientation != NULL), EINVAL);

    // Check status
    if (result == UNTHINK_SUCCESS)
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;

        // Setup
        memset((void*)orientation, 0, sizeof(tSenseHAT_Orientation));

        result = UNTHINK_CHECK_CONDITION((instancePrivate->getAccelerometerFunction != NULL), EFAULT);
        if (result == UNTHINK_SUCCESS)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Call the function
            PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->getAccelerometerFunction,
                                                             instancePrivate->self, NULL);
            result = UNTHINK_CHECK_CONDITION((pResult != NULL), UNTHINK_FAILURE);
            if (result == UNTHINK_SUCCESS)
            {
                // Convert the result
                result = SenseHAT_ConvertDictToOrientation(pResult, orientation);

                // Release reference
                Py_DECREF(pResult);
            }
            else    // PyObject_CallFunctionObjArgs failed
                (void)Python_Error("PyObject_CallFunctionObjArgs failed!");

            // Release our lock
            PyGILState_Release(state);
        }
    }
	return result;
}

// =================================================================================================
//  SenseHAT_GetAccelerometerRaw
// =================================================================================================
int32_t SenseHAT_GetAccelerometerRaw (const tSenseHAT_Instance instance,
                                      tSenseHAT_RawData* rawData)
{
	int32_t result = UNTHINK_SUCCESS;

    // Check arguments
    result = UNTHINK_CHECK_CONDITION((instance != NULL), EINVAL);
    if (result == UNTHINK_SUCCESS)
        result = UNTHINK_CHECK_CONDITION((rawData != NULL), EINVAL);

    // Check status
    if (result == UNTHINK_SUCCESS)
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;

        // Setup
        memset((void*)rawData, 0, sizeof(tSenseHAT_RawData));

        result = UNTHINK_CHECK_CONDITION((instancePrivate->getAccelerometerRawFunction != NULL), EFAULT);
        if (result == UNTHINK_SUCCESS)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Call the function
            PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->getAccelerometerRawFunction,
                                                             instancePrivate->self, NULL);
            result = UNTHINK_CHECK_CONDITION((pResult != NULL), UNTHINK_FAILURE);
            if (result == UNTHINK_SUCCESS)
            {
                // Convert the result
                result = SenseHAT_ConvertDictToRawData(pResult, rawData);

                // Release reference
                Py_DECREF(pResult);
            }
            else    // PyObject_CallFunctionObjArgs failed
                (void)Python_Error("PyObject_CallFunctionObjArgs failed!");

            // Release our lock
            PyGILState_Release(state);
        }
    }
	return result;
}

// =================================================================================================
//  SenseHAT_GetCompassRaw
// =================================================================================================
int32_t SenseHAT_GetCompassRaw (const tSenseHAT_Instance instance,
                                tSenseHAT_RawData* rawData)
{
	int32_t result = UNTHINK_SUCCESS;

    // Check arguments
    result = UNTHINK_CHECK_CONDITION((instance != NULL), EINVAL);
    if (result == UNTHINK_SUCCESS)
        result = UNTHINK_CHECK_CONDITION((rawData != NULL), EINVAL);

    // Check status
    if (result == UNTHINK_SUCCESS)
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;

        // Setup
        memset((void*)rawData, 0, sizeof(tSenseHAT_RawData));

        result = UNTHINK_CHECK_CONDITION((instancePrivate->getCompassRawFunction != NULL), EFAULT);
        if (result == UNTHINK_SUCCESS)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Call the function
            PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->getCompassRawFunction,
                                                             instancePrivate->self, NULL);
            result = UNTHINK_CHECK_CONDITION((pResult != NULL), UNTHINK_FAILURE);
            if (result == UNTHINK_SUCCESS)
            {
                // Convert the result
                result = SenseHAT_ConvertDictToRawData(pResult, rawData);

                // Release reference
                Py_DECREF(pResult);
            }
            else    // PyObject_CallFunctionObjArgs failed
                (void)Python_Error("PyObject_CallFunctionObjArgs failed!");

            // Release our lock
            PyGILState_Release(state);
        }
    }
	return result;
}

// =================================================================================================
//  SenseHAT_GetGyroscope
// =================================================================================================
int32_t SenseHAT_GetGyroscope (const tSenseHAT_Instance instance,
                               tSenseHAT_Orientation* orientation)
{
	int32_t result = UNTHINK_SUCCESS;

    // Check arguments
    result = UNTHINK_CHECK_CONDITION((instance != NULL), EINVAL);
    if (result == UNTHINK_SUCCESS)
        result = UNTHINK_CHECK_CONDITION((orientation != NULL), EINVAL);

    // Check status
    if (result == UNTHINK_SUCCESS)
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;

        // Setup
        memset((void*)orientation, 0, sizeof(tSenseHAT_Orientation));

        result = UNTHINK_CHECK_CONDITION((instancePrivate->getGyroscopeFunction != NULL), EFAULT);
        if (result == UNTHINK_SUCCESS)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Call the function
            PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->getGyroscopeFunction,
                                                             instancePrivate->self, NULL);
            result = UNTHINK_CHECK_CONDITION((pResult != NULL), UNTHINK_FAILURE);
            if (result == UNTHINK_SUCCESS)
            {
                // Convert the result
                result = SenseHAT_ConvertDictToOrientation(pResult, orientation);

                // Release reference
                Py_DECREF(pResult);
            }
            else    // PyObject_CallFunctionObjArgs failed
                (void)Python_Error("PyObject_CallFunctionObjArgs failed!");

            // Release our lock
            PyGILState_Release(state);
        }
    }
	return result;
}

// =================================================================================================
//  SenseHAT_GetGyroscopeRaw
// =================================================================================================
int32_t SenseHAT_GetGyroscopeRaw (const tSenseHAT_Instance instance,
                                  tSenseHAT_RawData* rawData)
{
	int32_t result = UNTHINK_SUCCESS;

    // Check arguments
    result = UNTHINK_CHECK_CONDITION((instance != NULL), EINVAL);
    if (result == UNTHINK_SUCCESS)
        result = UNTHINK_CHECK_CONDITION((rawData != NULL), EINVAL);

    // Check status
    if (result == UNTHINK_SUCCESS)
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;

        // Setup
        memset((void*)rawData, 0, sizeof(tSenseHAT_RawData));

        result = UNTHINK_CHECK_CONDITION((instancePrivate->getGyroscopeRawFunction != NULL), EFAULT);
        if (result == UNTHINK_SUCCESS)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Call the function
            PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->getGyroscopeRawFunction,
                                                             instancePrivate->self, NULL);
            result = UNTHINK_CHECK_CONDITION((pResult != NULL), UNTHINK_FAILURE);
            if (result == UNTHINK_SUCCESS)
            {
                // Convert the result
                result = SenseHAT_ConvertDictToRawData(pResult, rawData);

                // Release reference
                Py_DECREF(pResult);
            }
            else    // PyObject_CallFunctionObjArgs failed
                (void)Python_Error("PyObject_CallFunctionObjArgs failed!");

            // Release our lock
            PyGILState_Release(state);
        }
    }
	return result;
}

// =================================================================================================
//  SenseHAT_GetOrientation
// =================================================================================================
int32_t SenseHAT_GetOrientation (const tSenseHAT_Instance instance,
                                 tSenseHAT_Orientation* orientation)
{
	int32_t result = UNTHINK_SUCCESS;

    // Check arguments
    result = UNTHINK_CHECK_CONDITION((instance != NULL), EINVAL);
    if (result == UNTHINK_SUCCESS)
        result = UNTHINK_CHECK_CONDITION((orientation != NULL), EINVAL);

    // Check status
    if (result == UNTHINK_SUCCESS)
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;

        // Setup
        memset((void*)orientation, 0, sizeof(tSenseHAT_Orientation));

        result = UNTHINK_CHECK_CONDITION((instancePrivate->getOrientationFunction != NULL), EFAULT);
        if (result == UNTHINK_SUCCESS)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Call the function
            PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->getOrientationFunction,
                                                             instancePrivate->self, NULL);
            result = UNTHINK_CHECK_CONDITION((pResult != NULL), UNTHINK_FAILURE);
            if (result == UNTHINK_SUCCESS)
            {
                // Convert the result
                result = SenseHAT_ConvertDictToOrientation(pResult, orientation);

                // Release reference
                Py_DECREF(pResult);
            }
            else    // PyObject_CallFunctionObjArgs failed
                (void)Python_Error("PyObject_CallFunctionObjArgs failed!");

            // Release our lock
            PyGILState_Release(state);
        }
    }
	return result;
}

// =================================================================================================
//  SenseHAT_GetOrientationDegrees
// =================================================================================================
int32_t SenseHAT_GetOrientationDegrees (const tSenseHAT_Instance instance,
                                        tSenseHAT_Orientation* orientation)
{
	int32_t result = UNTHINK_SUCCESS;

    // Check arguments
    result = UNTHINK_CHECK_CONDITION((instance != NULL), EINVAL);
    if (result == UNTHINK_SUCCESS)
        result = UNTHINK_CHECK_CONDITION((orientation != NULL), EINVAL);

    // Check status
    if (result == UNTHINK_SUCCESS)
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;

        // Setup
        memset((void*)orientation, 0, sizeof(tSenseHAT_Orientation));

        result = UNTHINK_CHECK_CONDITION((instancePrivate->getOrientationDegreesFunction != NULL), EFAULT);
        if (result == UNTHINK_SUCCESS)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Call the function
            PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->getOrientationDegreesFunction,
                                                             instancePrivate->self, NULL);
            result = UNTHINK_CHECK_CONDITION((pResult != NULL), UNTHINK_FAILURE);
            if (result == UNTHINK_SUCCESS)
            {
                // Convert the result
                result = SenseHAT_ConvertDictToOrientation(pResult, orientation);

                // Release reference
                Py_DECREF(pResult);
            }
            else    // PyObject_CallFunctionObjArgs failed
                (void)Python_Error("PyObject_CallFunctionObjArgs failed!");

            // Release our lock
            PyGILState_Release(state);
        }
    }
	return result;
}

// =================================================================================================
//  SenseHAT_GetOrientationRadians
// =================================================================================================
int32_t SenseHAT_GetOrientationRadians (const tSenseHAT_Instance instance,
                                        tSenseHAT_Orientation* orientation)
{
	int32_t result = UNTHINK_SUCCESS;

    // Check arguments
    result = UNTHINK_CHECK_CONDITION((instance != NULL), EINVAL);
    if (result == UNTHINK_SUCCESS)
        result = UNTHINK_CHECK_CONDITION((orientation != NULL), EINVAL);

    // Check status
    if (result == UNTHINK_SUCCESS)
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;

        // Setup
        memset((void*)orientation, 0, sizeof(tSenseHAT_Orientation));

        result = UNTHINK_CHECK_CONDITION((instancePrivate->getOrientationRadiansFunction != NULL), EFAULT);
        if (result == UNTHINK_SUCCESS)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Call the function
            PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->getOrientationRadiansFunction,
                                                             instancePrivate->self, NULL);
            result = UNTHINK_CHECK_CONDITION((pResult != NULL), UNTHINK_FAILURE);
            if (result == UNTHINK_SUCCESS)
            {
                // Convert the result
                result = SenseHAT_ConvertDictToOrientation(pResult, orientation);

                // Release reference
                Py_DECREF(pResult);
            }
            else    // PyObject_CallFunctionObjArgs failed
                (void)Python_Error("PyObject_CallFunctionObjArgs failed!");

            // Release our lock
            PyGILState_Release(state);
        }
    }
	return result;
}

// =================================================================================================
//  SenseHAT_GetTemperatureFromHumidity
// =================================================================================================
int32_t SenseHAT_GetTemperatureFromHumidity (const tSenseHAT_Instance instance,
                                             double* degreesCelsius)
{
	int32_t result = UNTHINK_SUCCESS;

    // Check arguments
    result = UNTHINK_CHECK_CONDITION((instance != NULL), EINVAL);
    if (result == UNTHINK_SUCCESS)
        result = UNTHINK_CHECK_CONDITION((degreesCelsius != NULL), EINVAL);

    // Check status
    if (result == UNTHINK_SUCCESS)
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;

        // Setup
        *degreesCelsius = 0;

        result = UNTHINK_CHECK_CONDITION((instancePrivate->getTemperatureFromHumidityFunction != NULL), EFAULT);
        if (result == UNTHINK_SUCCESS)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Call the function
            PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->getTemperatureFromHumidityFunction,
                                                             instancePrivate->self, NULL);
            if (pResult != NULL)
            {
                // Get the result
                result = UNTHINK_CHECK_CONDITION((PyFloat_Check(pResult)), UNTHINK_FAILURE);
                if (result == UNTHINK_SUCCESS)
                    *degreesCelsius = PyFloat_AsDouble(pResult);

                // Release reference
                Py_DECREF(pResult);
            }
            else    // PyObject_CallFunctionObjArgs failed
                (void)Python_Error("PyObject_CallFunctionObjArgs failed!");

            // Release our lock
            PyGILState_Release(state);
        }
    }
	return result;
}

// =================================================================================================
//  SenseHAT_GetTemperatureFromPressure
// =================================================================================================
int32_t SenseHAT_GetTemperatureFromPressure (const tSenseHAT_Instance instance,
                                             double* degreesCelsius)
{
	int32_t result = UNTHINK_SUCCESS;

    // Check arguments
    result = UNTHINK_CHECK_CONDITION((instance != NULL), EINVAL);
    if (result == UNTHINK_SUCCESS)
        result = UNTHINK_CHECK_CONDITION((degreesCelsius != NULL), EINVAL);

    // Check status
    if (result == UNTHINK_SUCCESS)
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;

        // Setup
        *degreesCelsius = 0;

        result = UNTHINK_CHECK_CONDITION((instancePrivate->getTemperatureFromPressureFunction != NULL), EFAULT);
        if (result == UNTHINK_SUCCESS)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Call the function
            PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->getTemperatureFromPressureFunction,
                                                             instancePrivate->self, NULL);
            result = UNTHINK_CHECK_CONDITION((pResult != NULL), UNTHINK_FAILURE);
            if (result == UNTHINK_SUCCESS)
            {
                // Get the result
                result = UNTHINK_CHECK_CONDITION((PyFloat_Check(pResult)), UNTHINK_FAILURE);
                if (result == UNTHINK_SUCCESS)
                    *degreesCelsius = PyFloat_AsDouble(pResult);
                
                // Release reference
                Py_DECREF(pResult);
            }
            else    // PyObject_CallFunctionObjArgs failed
                (void)Python_Error("PyObject_CallFunctionObjArgs failed!");

            // Release our lock
            PyGILState_Release(state);
        }
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
	int32_t result = UNTHINK_SUCCESS;

    // Check arguments
    result = UNTHINK_CHECK_CONDITION((instance != NULL), EINVAL);
    if (result == UNTHINK_SUCCESS)
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;

        result = UNTHINK_CHECK_CONDITION((instancePrivate->setIMUConfigFunction != NULL), EFAULT);
        if (result == UNTHINK_SUCCESS)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Convert enableCompass argument
            PyObject* pEnableCompass = PyBool_FromLong((int32_t)enableCompass);
            result = UNTHINK_CHECK_CONDITION((pEnableCompass != NULL), UNTHINK_FAILURE);
            if (result == UNTHINK_SUCCESS)
            {
                // Convert enableGyroscope argument
                PyObject* pEnableGyroscope = PyBool_FromLong((int32_t)enableGyroscope);
                result = UNTHINK_CHECK_CONDITION((pEnableGyroscope != NULL), UNTHINK_FAILURE);
                if (result == UNTHINK_SUCCESS)
                {
                    // Convert enableAccelerometer argument
                    PyObject* pEnableAccelerometer = PyBool_FromLong((int32_t)enableAccelerometer);
                    result = UNTHINK_CHECK_CONDITION((pEnableAccelerometer != NULL), UNTHINK_FAILURE);
                    if (result == UNTHINK_SUCCESS)
                    {
                        // Call the function
                        PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->setIMUConfigFunction,
                                                                         instancePrivate->self, 
                                                                         pEnableCompass,
                                                                         pEnableGyroscope,
                                                                         pEnableAccelerometer,
                                                                         NULL);
                        result = UNTHINK_CHECK_CONDITION((pResult != NULL), UNTHINK_FAILURE);
                        if (result == UNTHINK_SUCCESS)
                        {
                            // Release reference
                            Py_DECREF(pResult);
                        }
                        else    // PyObject_CallFunctionObjArgs failed
                            (void)Python_Error("PyObject_CallFunctionObjArgs failed!");

                        // Release reference
                        Py_DECREF(pEnableAccelerometer);
                    }
                    else    // PyBool_FromLong failed
                        (void)Python_Error("PyBool_FromLong failed!");

                    // Release reference
                    Py_DECREF(pEnableGyroscope);
                }
                else    // PyBool_FromLong failed
                    (void)Python_Error("PyBool_FromLong failed!");

                // Release reference
                Py_DECREF(pEnableCompass);
            }
            else    // PyBool_FromLong failed
                (void)Python_Error("PyBool_FromLong failed!");

            // Release our lock
            PyGILState_Release(state);
        }
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
	int32_t result = UNTHINK_SUCCESS;

    // Check arguments
    result = UNTHINK_CHECK_CONDITION((instance != NULL), EINVAL);
    if (result == UNTHINK_SUCCESS)
        result = UNTHINK_CHECK_CONDITION((eventCount != NULL), EINVAL);

    // Check status
    if (result == UNTHINK_SUCCESS)
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;

        // Setup
		*eventCount = 0;

        result = UNTHINK_CHECK_CONDITION((instancePrivate->getEventsFunction != NULL), EFAULT);
        if (result == UNTHINK_SUCCESS)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();

            // Call the function
            PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->getEventsFunction,
                                                             NULL);
            result = UNTHINK_CHECK_CONDITION((pResult != NULL), UNTHINK_FAILURE);
            if (result == UNTHINK_SUCCESS)
            {
                // The result should be a list of events
                result = UNTHINK_CHECK_CONDITION((PyList_Check(pResult)), UNTHINK_FAILURE);
                if (result == UNTHINK_SUCCESS)
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

                            result = Unthink_AllocateMemory(sizeof(tSenseHAT_JoystickEvent) * numEvents,
                                                            (void**)&list);
                            if (result == UNTHINK_SUCCESS)
                            {
                                uint_fast32_t i = 0;
                                PyObject* tuple = NULL;

                                for (i = 0; i < numEvents; i++)
                                {
                                    // Get an event from the list
                                    tuple = PyList_GetItem(pResult, i);
                                    result = UNTHINK_CHECK_CONDITION((tuple != NULL), UNTHINK_FAILURE);
                                    if (result == UNTHINK_SUCCESS)
                                    {
                                        result = UNTHINK_CHECK_CONDITION((PyTuple_Check(tuple)), UNTHINK_FAILURE);
                                        if (result == UNTHINK_SUCCESS)
                                            result = SenseHAT_ParseJoystickEvent(tuple, &(list[i]));

                                        // Release reference
                                        Py_DECREF(tuple);
                                        tuple = NULL;
                                    }
                                    else    // PyList_GetItem failed
                                    {
                                        (void)Python_Error("PyList_GetItem failed!");
                                        break;
                                    }

                                    if (result != UNTHINK_SUCCESS)
                                        break;
                                }

                                if (result == UNTHINK_SUCCESS)
                                {
                                    *eventCount = numEvents;
                                    *events = list;
                                }
                                else    // Clean up
                                    (void)Unthink_FreeMemory((void**)&list);
                            }
                        }
                    }
                }

                // Release reference
                Py_DECREF(pResult);
            }
            else    // PyObject_CallFunctionObjArgs failed
                (void)Python_Error("PyObject_CallFunctionObjArgs failed!");
            
            // Release our lock
            PyGILState_Release(state);
        }
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
	int32_t result = UNTHINK_SUCCESS;

    // Check arguments
    result = UNTHINK_CHECK_CONDITION((instance != NULL), EINVAL);
    if (result == UNTHINK_SUCCESS)
        result = UNTHINK_CHECK_CONDITION((event != NULL), EINVAL);

    // Check status
    if (result == UNTHINK_SUCCESS)
    {
        // Get private data
        tSenseHAT_InstancePrivate* instancePrivate = (tSenseHAT_InstancePrivate*)instance;

        // Setup
        memset(event, 0, sizeof(tSenseHAT_JoystickEvent));

        result = UNTHINK_CHECK_CONDITION((instancePrivate->waitForEventFunction != NULL), EFAULT);
        if (result == UNTHINK_SUCCESS)
        {
            // Get a lock
            PyGILState_STATE state = PyGILState_Ensure();
            
            // Convert flush argument
            PyObject* pFlush = PyBool_FromLong((int32_t)flushPendingEvents);
            result = UNTHINK_CHECK_CONDITION((pFlush != NULL), UNTHINK_FAILURE);
            if (result == UNTHINK_SUCCESS)
            {
                // Call the function
                PyObject* pResult = PyObject_CallFunctionObjArgs(instancePrivate->waitForEventFunction,
                                                                 pFlush,
                                                                 NULL);
                result = UNTHINK_CHECK_CONDITION((pResult != NULL), UNTHINK_FAILURE);
                if (result == UNTHINK_SUCCESS)
                {
                    // The result should be an event tuple
                    result = UNTHINK_CHECK_CONDITION((PyTuple_Check(pResult)), UNTHINK_FAILURE);
                    if (result == UNTHINK_SUCCESS)
                        result = SenseHAT_ParseJoystickEvent(pResult, event);

                    // Release reference
                    Py_DECREF(pResult);
                }
                else    // PyObject_CallFunctionObjArgs failed
                    (void)Python_Error("PyObject_CallFunctionObjArgs failed!");

                // Release reference
                Py_DECREF(pFlush);
            }
            else    // PyBool_FromLong failed
                (void)Python_Error("PyBool_FromLong failed!");

            // Release our lock
            PyGILState_Release(state);
        }
    }
    return result;
}

// =================================================================================================
//  SenseHAT_ValidateLEDPixelValue
// =================================================================================================
int32_t SenseHAT_ValidateLEDPixelPosition (int32_t xPosition,
                                           int32_t yPosition)
{
    int32_t result = UNTHINK_SUCCESS;

    // Check arguments
    result = UNTHINK_CHECK_CONDITION((xPosition >= 0), EINVAL);
    if (result == UNTHINK_SUCCESS)
    {
        result = UNTHINK_CHECK_CONDITION((xPosition <= 7), EINVAL);
        if (result == UNTHINK_SUCCESS)
        {
            result = UNTHINK_CHECK_CONDITION((yPosition >= 0), EINVAL);
            if (result == UNTHINK_SUCCESS)
                result = UNTHINK_CHECK_CONDITION((yPosition <= 7), EINVAL);
        }
    }
    return result;
}

// =================================================================================================
//  SenseHAT_ValidateLEDPixelValue
// =================================================================================================
int32_t SenseHAT_ValidateLEDPixelValue (tSenseHAT_LEDPixel* pixel)
{
    int32_t result = UNTHINK_SUCCESS;

    // Check argument
    result = UNTHINK_CHECK_CONDITION((pixel != NULL), EINVAL);
    if (result == UNTHINK_SUCCESS)
    {
        result = UNTHINK_CHECK_CONDITION((pixel->red >= 0), EINVAL);
        if (result == UNTHINK_SUCCESS)
        {
            result = UNTHINK_CHECK_CONDITION((pixel->red <= 255), EINVAL);
            if (result == UNTHINK_SUCCESS)
            {
                result = UNTHINK_CHECK_CONDITION((pixel->green >= 0), EINVAL);
                if (result == UNTHINK_SUCCESS)
                {
                    result = UNTHINK_CHECK_CONDITION((pixel->green <= 255), EINVAL);
                    if (result == UNTHINK_SUCCESS)
                    {
                        result = UNTHINK_CHECK_CONDITION((pixel->blue >= 0), EINVAL);
                        if (result == UNTHINK_SUCCESS)
                            result = UNTHINK_CHECK_CONDITION((pixel->blue <= 255), EINVAL);
                    }
                }
            }
        }
    }
    return result;
}

// =================================================================================================
//  SenseHAT_ConvertPixelToLEDPixel
// =================================================================================================
int32_t SenseHAT_ConvertPixelToLEDPixel (const PyObject* pixel,
                                         tSenseHAT_LEDPixel* color)
{
    int32_t result = UNTHINK_SUCCESS;

    // Check arguments
    result = UNTHINK_CHECK_CONDITION((pixel != NULL), EINVAL);
    if (result == UNTHINK_SUCCESS)
        result = UNTHINK_CHECK_CONDITION((color != NULL), EINVAL);
    
    // Check status
    if (result == UNTHINK_SUCCESS)
    {
        PyObject* pChannel = NULL;

        pChannel = PyList_GetItem((PyObject*)pixel, 0);
        result = UNTHINK_CHECK_CONDITION((pChannel != NULL), UNTHINK_FAILURE);
        if (result == UNTHINK_SUCCESS)
        {
            result = UNTHINK_CHECK_CONDITION((PYLONG_CHECK(pChannel)), UNTHINK_FAILURE);
            if (result == UNTHINK_SUCCESS)
                color->red = PyLong_AsLong(pChannel);

            pChannel = NULL;
        }

        if (result == UNTHINK_SUCCESS)
        {
            pChannel = PyList_GetItem((PyObject*)pixel, 1);
            result = UNTHINK_CHECK_CONDITION((pChannel != NULL), UNTHINK_FAILURE);
            if (result == UNTHINK_SUCCESS)
            {
                result = UNTHINK_CHECK_CONDITION((PYLONG_CHECK(pChannel)), UNTHINK_FAILURE);
                if (result == UNTHINK_SUCCESS)
                    color->green = PyLong_AsLong(pChannel);

                pChannel = NULL;
            }
        }

        if (result == UNTHINK_SUCCESS)
        {
            pChannel = PyList_GetItem((PyObject*)pixel, 2);
            result = UNTHINK_CHECK_CONDITION((pChannel != NULL), UNTHINK_FAILURE);
            if (result == UNTHINK_SUCCESS)
            {
                result = UNTHINK_CHECK_CONDITION((PYLONG_CHECK(pChannel)), UNTHINK_FAILURE);
                if (result == UNTHINK_SUCCESS)
                    color->blue = PyLong_AsLong(pChannel);

                pChannel = NULL;
            }
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
    int32_t result = UNTHINK_SUCCESS;

    // Check arguments
    result = UNTHINK_CHECK_CONDITION((pixelList != NULL), EINVAL);
    if (result == UNTHINK_SUCCESS)
        result = UNTHINK_CHECK_CONDITION((pixelArray != NULL), EINVAL);

    // Check status
    if (result == UNTHINK_SUCCESS)
    {
        // Make sure we have a list
        result = UNTHINK_CHECK_CONDITION((PyList_Check((PyObject*)pixelList)), UNTHINK_FAILURE);
        if (result == UNTHINK_SUCCESS) 
        {
            // Make sure the list is 64 elements long
            result = UNTHINK_CHECK_CONDITION((PyList_Size((PyObject*)pixelList) == 64), UNTHINK_FAILURE);
            if (result == UNTHINK_SUCCESS)
            {
                uint_fast32_t index = 0;
                PyObject* pPixel = NULL;

                // Iterate over the list for each pixel
                for (index = 0; index < 64; index++)
                {
                    // Get the pixel
                    pPixel = PyList_GetItem((PyObject*)pixelList, index);
                    result = UNTHINK_CHECK_CONDITION((pPixel != NULL), UNTHINK_FAILURE);
                    if (result == UNTHINK_SUCCESS)
                    {
                        // Make sure we got a list
                        result = UNTHINK_CHECK_CONDITION((PyList_Check(pPixel)), UNTHINK_FAILURE);
                        if (result == UNTHINK_SUCCESS)
                        {
                            // Make sure the list is 3 elements long
                            result = UNTHINK_CHECK_CONDITION((PyList_Size(pPixel) == 3), UNTHINK_FAILURE);
                            if (result == UNTHINK_SUCCESS)
                            {
                                // Convert pixel to LED pixel
                                result = SenseHAT_ConvertPixelToLEDPixel(pPixel, &(pixelArray[index]));
                            }
                        }

                        // Release reference
                        Py_DECREF(pPixel);
                        pPixel = NULL;

                        if (result != UNTHINK_SUCCESS)
                            break;
                    }
                    else    // PyList_GetItem failed
                    {
                        (void)Python_Error("PyList_GetItem failed!");
                        break;
                    }
                }
            }
        }
    }
    return result;
}

// =================================================================================================
//  SenseHAT_ConvertDictToOrientation
// =================================================================================================
int32_t SenseHAT_ConvertDictToOrientation (const PyObject* dict,
                                           tSenseHAT_Orientation* orientation)
{
    int32_t result = UNTHINK_SUCCESS;

    // Check arguments
    result = UNTHINK_CHECK_CONDITION((dict != NULL), EINVAL);
    if (result == UNTHINK_SUCCESS)
        result = UNTHINK_CHECK_CONDITION((orientation != NULL), EINVAL);

    // Check status
    if (result == UNTHINK_SUCCESS)
    {
        // Make sure this is a dictionary
        result = UNTHINK_CHECK_CONDITION((PyDict_Check(dict)), UNTHINK_FAILURE);
        if (result == UNTHINK_SUCCESS)
        {
            // Get the pitch item
            PyObject* pValue = PyDict_GetItemString((PyObject*)dict, kOrientationPitch);
            result = UNTHINK_CHECK_CONDITION((pValue != NULL), UNTHINK_FAILURE);
            if (result == UNTHINK_SUCCESS)
            {
                // Make sure it's a float
                result = UNTHINK_CHECK_CONDITION((PyFloat_Check(pValue)), UNTHINK_FAILURE);
                if (result == UNTHINK_SUCCESS)
                {
                    // Get the value
                    orientation->pitch = PyFloat_AsDouble(pValue);
                }

                // Release reference
                Py_DECREF(pValue);
                pValue = NULL;

                // Check status
                if (result == UNTHINK_SUCCESS)
                {
                    // Get the roll item
                    pValue = PyDict_GetItemString((PyObject*)dict, kOrientationRoll);
                    result = UNTHINK_CHECK_CONDITION((pValue != NULL), UNTHINK_FAILURE);
                    if (result == UNTHINK_SUCCESS)
                    {
                        // Make sure it's a float
                        result = UNTHINK_CHECK_CONDITION((PyFloat_Check(pValue)), UNTHINK_FAILURE);
                        if (result == UNTHINK_SUCCESS)
                        {
                            // Get the value
                            orientation->roll = PyFloat_AsDouble(pValue);
                        }

                        // Release reference
                        Py_DECREF(pValue);
                        pValue = NULL;
                    }
                    else    // PyDict_GetItemString failed
                        (void)Python_Error("PyDict_GetItemString failed!");
                }

                // Check status
                if (result == UNTHINK_SUCCESS)
                {
                    // Get the yaw item
                    pValue = PyDict_GetItemString((PyObject*)dict, kOrientationYaw);
                    result = UNTHINK_CHECK_CONDITION((pValue != NULL), UNTHINK_FAILURE);
                    if (result == UNTHINK_SUCCESS)
                    {
                        // Make sure it's a float
                        result = UNTHINK_CHECK_CONDITION((PyFloat_Check(pValue)), UNTHINK_FAILURE);
                        if (result == UNTHINK_SUCCESS)
                        {
                            // Get the value
                            orientation->yaw = PyFloat_AsDouble(pValue);
                        }

                        // Release reference
                        Py_DECREF(pValue);
                        pValue = NULL;
                    }
                    else    // PyDict_GetItemString failed
                        (void)Python_Error("PyDict_GetItemString failed!");
                }
            }
            else    // PyDict_GetItemString failed
                (void)Python_Error("PyDict_GetItemString failed!");
        }
    }
    return result;
}

// =================================================================================================
//  SenseHAT_ConvertDictToRawData
// =================================================================================================
int32_t SenseHAT_ConvertDictToRawData (const PyObject* dict,
                                       tSenseHAT_RawData* rawData)
{
    int32_t result = UNTHINK_SUCCESS;

    // Check arguments
    result = UNTHINK_CHECK_CONDITION((dict != NULL), EINVAL);
    if (result == UNTHINK_SUCCESS)
        result = UNTHINK_CHECK_CONDITION((rawData != NULL), EINVAL);
    
    // Check status
    if (result == UNTHINK_SUCCESS)
    {
        // Make sure this is a dictionary
        result = UNTHINK_CHECK_CONDITION((PyDict_Check(dict)), UNTHINK_FAILURE);
        if (result == UNTHINK_SUCCESS)
        {
            // Get the x item
            PyObject* pValue = PyDict_GetItemString((PyObject*)dict, kRawX);
            result = UNTHINK_CHECK_CONDITION((pValue != NULL), UNTHINK_FAILURE);
            if (result == UNTHINK_SUCCESS)
            {
                // Make sure it's a float
                result = UNTHINK_CHECK_CONDITION((PyFloat_Check(pValue)), UNTHINK_FAILURE);
                if (result == UNTHINK_SUCCESS)
                {
                    // Get the value
                    rawData->x = PyFloat_AsDouble(pValue);
                }

                // Release reference
                Py_DECREF(pValue);
                pValue = NULL;

                // Check status
                if (result == UNTHINK_SUCCESS)
                {
                    // Get the y item
                    pValue = PyDict_GetItemString((PyObject*)dict, kRawY);
                    result = UNTHINK_CHECK_CONDITION((pValue != NULL), UNTHINK_FAILURE);
                    if (result == UNTHINK_SUCCESS)
                    {
                        // Make sure it's a float
                        result = UNTHINK_CHECK_CONDITION((PyFloat_Check(pValue)), UNTHINK_FAILURE);
                        if (result == UNTHINK_SUCCESS)
                        {
                            // Get the value
                            rawData->y = PyFloat_AsDouble(pValue);
                        }

                        // Release reference
                        Py_DECREF(pValue);
                        pValue = NULL;
                    }
                    else    // PyDict_GetItemString failed
                        (void) Python_Error("PyDict_GetItemString failed!");
                }

                // Check status
                if (result == UNTHINK_SUCCESS)
                {
                    // Get the yaw item
                    pValue = PyDict_GetItemString((PyObject*)dict, kRawZ);
                    result = UNTHINK_CHECK_CONDITION((pValue != NULL), UNTHINK_FAILURE);
                    if (result == UNTHINK_SUCCESS)
                    {
                        // Make sure it's a float
                        result = UNTHINK_CHECK_CONDITION((PyFloat_Check(pValue)), UNTHINK_FAILURE);
                        if (result == UNTHINK_SUCCESS)
                        {
                            // Get the value
                            rawData->z = PyFloat_AsDouble(pValue);
                        }

                        // Release reference
                        Py_DECREF(pValue);
                        pValue = NULL;
                    }
                    else    // PyDict_GetItemString failed
                        (void)Python_Error("PyDict_GetItemString failed!");
                }
            }
            else    // PyDict_GetItemString failed
                (void)Python_Error("PyDict_GetItemString failed!");
        }
    }
    return result;
}

// =================================================================================================
//  SenseHAT_ParseJoystickEvent
// =================================================================================================
int32_t SenseHAT_ParseJoystickEvent (const PyObject* tuple,
                                     tSenseHAT_JoystickEvent* event)
{
    int32_t result = UNTHINK_SUCCESS;
    PyObject* tupleItem = NULL;

    // Check arguments
    result = UNTHINK_CHECK_CONDITION((tuple != NULL), EINVAL);
    if (result == UNTHINK_SUCCESS)
        result = UNTHINK_CHECK_CONDITION((event != NULL), EINVAL);

    // Check status
    if (result == UNTHINK_SUCCESS)
    {
        // Get the timestamp
        tupleItem = PyTuple_GetItem((PyObject*)tuple, 0);
        result = UNTHINK_CHECK_CONDITION((tupleItem != NULL), UNTHINK_FAILURE);
        if (result == UNTHINK_SUCCESS)
        {
            result = UNTHINK_CHECK_CONDITION((PyFloat_Check(tupleItem)), UNTHINK_FAILURE);
            if (result == UNTHINK_SUCCESS)
                event->timestamp = PyFloat_AsDouble(tupleItem);

            //Py_DECREF(tupleItem);
            tupleItem = NULL;
        }
        else    // PyTuple_GetItem failed
            (void)Python_Error("PyTuple_GetItem failed!");
    }

    // Check status
    if (result == UNTHINK_SUCCESS)
    {
        // Get the direction
        tupleItem = PyTuple_GetItem((PyObject*)tuple, 1);
        result = UNTHINK_CHECK_CONDITION((tupleItem != NULL), UNTHINK_FAILURE);
        if (result == UNTHINK_SUCCESS)
        {
            result = UNTHINK_CHECK_CONDITION((PyUnicode_Check(tupleItem)), UNTHINK_FAILURE);
            if (result == UNTHINK_SUCCESS)
            {
                char* directionStr = PYSTRING_AS_STRING(tupleItem);
                result = UNTHINK_CHECK_CONDITION((directionStr != NULL), UNTHINK_FAILURE);
                if (result == UNTHINK_SUCCESS)
                {
                    result = UNTHINK_CHECK_CONDITION((strlen(directionStr) > 0), UNTHINK_FAILURE);
                    if (result == UNTHINK_SUCCESS)
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
                }
            }

            //Py_DECREF(tupleItem);
            tupleItem = NULL;
        }
        else    // PyTuple_GetItem failed
            (void)Python_Error("PyTuple_GetItem failed!");
    }

    // Check status
    if (result == UNTHINK_SUCCESS)
    {
        // Get the action
        tupleItem = PyTuple_GetItem((PyObject*)tuple, 2);
        result = UNTHINK_CHECK_CONDITION((tupleItem != NULL), UNTHINK_FAILURE);
        if (result == UNTHINK_SUCCESS)
        {
            result = UNTHINK_CHECK_CONDITION((PyUnicode_Check(tupleItem)), UNTHINK_FAILURE);
            if (result == UNTHINK_SUCCESS)
            {
                char* actionStr = PYSTRING_AS_STRING(tupleItem);
                result = UNTHINK_CHECK_CONDITION((actionStr != NULL), UNTHINK_FAILURE);
                if (result == UNTHINK_SUCCESS)
                {
                    result = UNTHINK_CHECK_CONDITION((strlen(actionStr) > 0), UNTHINK_FAILURE);
                    if (result == UNTHINK_SUCCESS)
                    {
                        if (strcmp(actionStr, "pressed") == 0)
                            event->action = eSenseHAT_JoystickActionPressed;

                        else if (strcmp(actionStr, "released") == 0)
                            event->action = eSenseHAT_JoystickActionReleased;

                        else if (strcmp(actionStr, "held") == 0)
                            event->action = eSenseHAT_JoystickActionHeld;

                        else
                            event->action = eSenseHAT_JoystickActionNone;
                    }
                }
            }

            //Py_DECREF(tupleItem);
            tupleItem = NULL;
        }
        else    // PyTuple_GetItem failed
            (void)Python_Error("PyTuple_GetItem failed!");
    }
    return result;
}

// =================================================================================================
//  SenseHAT_Release
// =================================================================================================
int32_t SenseHAT_Release (tSenseHAT_InstancePrivate* instancePrivate)
{
    int32_t result = UNTHINK_SUCCESS;

    // Check argument
    result = UNTHINK_CHECK_CONDITION((instancePrivate != NULL), EINVAL);
    if (result == UNTHINK_SUCCESS)
    {
        // Clean up
        if (instancePrivate->senseHATModule != NULL)
        {
            // Release stick submodule
            if (instancePrivate->stickSubModule != NULL)
            {
                (void)Python_ReleaseFunctionReference(&(instancePrivate->getEventsFunction));
                (void)Python_ReleaseFunctionReference(&(instancePrivate->waitForEventFunction));
                (void)Python_ReleaseFunctionReference(&(instancePrivate->stickSubModule));                
            }
            
            // Release sensehat submodule
            if (instancePrivate->senseHATSubModule != NULL)
            {
                (void)Python_ReleaseFunctionReference(&(instancePrivate->clearFunction));
                (void)Python_ReleaseFunctionReference(&(instancePrivate->flipHorizontalFunction));
                (void)Python_ReleaseFunctionReference(&(instancePrivate->flipVerticalFunction));
                #if 0
                Python_ReleaseFunctionReference(&(instancePrivate->gammaFunction));
                #endif
                (void)Python_ReleaseFunctionReference(&(instancePrivate->gammaResetFunction));
                (void)Python_ReleaseFunctionReference(&(instancePrivate->getAccelerometerFunction));
                (void)Python_ReleaseFunctionReference(&(instancePrivate->getAccelerometerRawFunction));
                (void)Python_ReleaseFunctionReference(&(instancePrivate->getCompassFunction));
                (void)Python_ReleaseFunctionReference(&(instancePrivate->getCompassRawFunction));
                (void)Python_ReleaseFunctionReference(&(instancePrivate->getGyroscopeFunction));
                (void)Python_ReleaseFunctionReference(&(instancePrivate->getGyroscopeRawFunction));
                (void)Python_ReleaseFunctionReference(&(instancePrivate->getHumidityFunction));
                (void)Python_ReleaseFunctionReference(&(instancePrivate->getOrientationFunction));
                (void)Python_ReleaseFunctionReference(&(instancePrivate->getOrientationDegreesFunction));
                (void)Python_ReleaseFunctionReference(&(instancePrivate->getOrientationRadiansFunction));
                (void)Python_ReleaseFunctionReference(&(instancePrivate->getPixelFunction));
                (void)Python_ReleaseFunctionReference(&(instancePrivate->getPixelsFunction));
                (void)Python_ReleaseFunctionReference(&(instancePrivate->getPressureFunction));
                (void)Python_ReleaseFunctionReference(&(instancePrivate->getTemperatureFunction));
                (void)Python_ReleaseFunctionReference(&(instancePrivate->getTemperatureFromHumidityFunction));
                (void)Python_ReleaseFunctionReference(&(instancePrivate->getTemperatureFromPressureFunction));
                (void)Python_ReleaseFunctionReference(&(instancePrivate->loadImageFunction));
                #if 0
                (void)Python_ReleaseFunctionReference(&(instancePrivate->lowLightFunction));
                #endif
                (void)Python_ReleaseFunctionReference(&(instancePrivate->setIMUConfigFunction));
                (void)Python_ReleaseFunctionReference(&(instancePrivate->setPixelFunction));
                (void)Python_ReleaseFunctionReference(&(instancePrivate->setPixelsFunction));
                (void)Python_ReleaseFunctionReference(&(instancePrivate->setRotationFunction));
                (void)Python_ReleaseFunctionReference(&(instancePrivate->showLetterFunction));
                (void)Python_ReleaseFunctionReference(&(instancePrivate->showMessageFunction));
                (void)Python_ReleaseFunctionReference(&(instancePrivate->self));
                (void)Python_ReleaseFunctionReference(&(instancePrivate->senseHATSubModule));
            }

            (void)Python_ReleaseFunctionReference(&(instancePrivate->senseHATModule));
        }
    }
    return result;
}

// =================================================================================================
