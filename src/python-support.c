// ==================================================================================================
//
//  python-support.c
//
//  Copyright (c) 2019 Unthinkable Research LLC. All rights reserved.
//
//  Supported host operating systems:
//      Raspbian Stretch or later
//
//  Description:
//      This file contains function implementations for the Python utility functions.
//
//  Notes:
//      1)  Requires ANSI C99 (or better) compliant compilers.
//      2)  This library requires Python 2.x/3.x or later.
//  
// =================================================================================================
//! @file python-support.c
//! @author Gary Woodcock (gary.woodcock@unthinkable.com)
//! @brief This file contains function implementations for the Python utility functions.
//! @date 2019-09-25
//! @copyright Copyright (c) 2019 Unthinkable Research LLC. All rights reserved.
//! 
//  Includes
// =================================================================================================
#include "python-support.h"
#include <stdlib.h>

// =================================================================================================
//  Python_GetFunctionReference
// =================================================================================================
int32_t Python_GetFunctionReference (const PyObject* subModule,
                                     const char* functionName,
                                     PyObject** const functionReference)
{
    int32_t result = 0;

    // Check arguments
    if ((subModule != NULL) &&
        (functionName != NULL) &&
        (functionReference != NULL))
    {
        // Setup
        *functionReference = NULL;

        // Get a reference to the function
        PyObject* pyFunc = PyObject_GetAttrString((PyObject*) subModule, functionName);
        if (pyFunc != NULL)
        {
            // Is the function callable?
            if (PyCallable_Check(pyFunc))
            {
                // Return the function reference
                *functionReference = pyFunc;
            }
            else    // The function isn't callable
            {
                // Release reference
                Py_DECREF(pyFunc);
                result = ENOTSUP;
            }
        }
        else    // PyObject_GetAttrString failed
        {
            result = Python_Error("PyObject_GetAttrString failed!");
        }
    }
    else    // Invalid argument
    {
        result = EINVAL;
    }
    return result;
}

// =================================================================================================
//  Python_ReleaseFunctionReference
// =================================================================================================
void Python_ReleaseFunctionReference (PyObject** const functionReference)
{
    // Check arguments
    if ((functionReference != NULL) && (*functionReference != NULL))
    {
        // Release reference
        Py_DECREF(*functionReference);
        *functionReference = NULL;
    }
    else    // Invalid argument
    {
        fprintf(stderr, "Python_ReleaseFunctionReference: Invalid argument!\n");
    }
    return;
}

// =================================================================================================
//  Python_Error
// =================================================================================================
int32_t Python_Error (const char* context)
{
    int32_t err = -1;

    // Get the Python interpreter error object (note this is a borrowed reference)
    PyObject* pyErr = PyErr_Occurred();
    if (pyErr != NULL)
    {
        // Print it
        PyErr_Print();
    }

    // Print error context to stderr
    if (strlen(context) > 0)
    {
        fprintf(stderr, "%s\n", context);
    }
    return err;
}

// =================================================================================================
//  Python_CloseInterpreter
// =================================================================================================
void Python_CloseInterpreter (void)
{
    // Get a lock
    PyGILState_STATE state = PyGILState_Ensure();
    (void)(state);

    // Release the Python interpreter
    PY_FINALIZE();

    return;
}

// =================================================================================================