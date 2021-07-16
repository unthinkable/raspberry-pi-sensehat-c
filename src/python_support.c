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
#include "unthink_utils.h"
#include "python_support.h"
#include <stdlib.h>

// =================================================================================================
//  Python_GetFunctionReference
// =================================================================================================
int32_t Python_GetFunctionReference (const PyObject* subModule,
                                     const char* functionName,
                                     PyObject** const functionReference)
{
    int32_t result = UNTHINK_SUCCESS;

    // Check arguments
    result = UNTHINK_CHECK_CONDITION((subModule != NULL), EINVAL);
    if (result == UNTHINK_SUCCESS)
    {
        result = UNTHINK_CHECK_CONDITION((functionName != NULL), EINVAL);
        if (result == UNTHINK_SUCCESS)
        {
            result = UNTHINK_CHECK_CONDITION((strlen(functionName) > 0), EINVAL);
            if (result == UNTHINK_SUCCESS)
                result = UNTHINK_CHECK_CONDITION((functionReference != NULL), EINVAL);
        }
    }

    // Check status
    if (result == UNTHINK_SUCCESS)
    {
        // Setup
        *functionReference = NULL;

        // Get a reference to the function
        PyObject* pyFunc = PyObject_GetAttrString((PyObject*) subModule, functionName);
        result = UNTHINK_CHECK_CONDITION((pyFunc != NULL), EFAULT);
        if (result == UNTHINK_SUCCESS)
        {
            // Is the function callable?
            if (PyCallable_Check(pyFunc))
                *functionReference = pyFunc; // Return the function reference

            else    // The function isn't callable
            {
                // Release reference
                Py_DECREF(pyFunc);
                result = ENOTSUP;
            }
        }
        else    // PyObject_GetAttrString failed
            (void)Python_Error("PyObject_GetAttrString failed!");
    }
    return result;
}

// =================================================================================================
//  Python_ReleaseFunctionReference
// =================================================================================================
int32_t Python_ReleaseFunctionReference (PyObject** const functionReference)
{
    int32_t result = UNTHINK_SUCCESS;

    // Check arguments
    result = UNTHINK_CHECK_CONDITION((functionReference != NULL), EINVAL);
    if (result == UNTHINK_SUCCESS)
        result = UNTHINK_CHECK_CONDITION((*functionReference != NULL), EINVAL);
    
    if (result == UNTHINK_SUCCESS)
    {
        // Release reference
        Py_DECREF(*functionReference);
        *functionReference = NULL;
    }
    return result;
}

// =================================================================================================
//  Python_Error
// =================================================================================================
int32_t Python_Error (const char* context)
{
    int32_t err = UNTHINK_FAILURE;

    // Check argument
    result = UNTHINK_CHECK_CONDITION((context != NULL), EINVAL);
    if (result == UNTHINK_SUCCESS)
        result = UNTHINK_CHECK_CONDITION((strlen(context) > 0), EINVAL);

    // Check status
    if (result == UNTHINK_SUCCESS)
    {
        // Get the Python interpreter error object (note this is a borrowed reference)
        PyObject* pyErr = PyErr_Occurred();
        if (pyErr != NULL)
        {
            // Print it
            PyErr_Print();
        }

        // Print error context to stderr
        if (strlen(context) > 0)
            fprintf(stdout, "%s\n", context);
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