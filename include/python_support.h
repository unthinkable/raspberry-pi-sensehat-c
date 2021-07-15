// ==================================================================================================
//
//  python_support.h
//
//  Copyright (c) 2019 Unthinkable Research LLC. All rights reserved.
//
//  Supported host operating systems:
//      Raspbian Stretch or later
//
//  Description:
//      This file contains public constants and function prototypes for the Python utility 
//      functions.
//
//  Notes:
//      1)  Requires ANSI C99 (or better) compliant compilers.
//      2)  This library requires Python 2.x/3.x or later.
//  
// =================================================================================================
//! @file python-support.h
//! @author Gary Woodcock (gary.woodcock@unthinkable.com)
//! @brief This file contains public constants and function prototypes for the Python utility 
//! functions.
//! @date 2019-09-25
//! @copyright Copyright (c) 2019 Unthinkable Research LLC. All rights reserved.
//! 
//  Includes
// =================================================================================================
#ifdef __cplusplus
    #pragma once
#endif

#ifndef __PYTHONSUPPORT_H__
#define __PYTHONSUPPORT_H__

#include "unthink_types"
#include <Python.h>

#if PY_MAJOR_VERSION == 2
#define PYSTRING_AS_STRING  PyString_AsString
#define PYLONG_CHECK PyInt_Check
#define PY_FINALIZE Py_Finalize
#elif PY_MAJOR_VERSION == 3
#define PYSTRING_AS_STRING  PyUnicode_AsUTF8
#define PYLONG_CHECK PyLong_Check
#define PY_FINALIZE Py_FinalizeEx
#endif

// =================================================================================================
//  Prototypes
// =================================================================================================

#ifdef __cplusplus
extern "C"
{
#endif

    //! @brief Call Python_GetFunctionReference to get a reference to a Python function.
    //!
    //! @param[in] module Python module or submodule name. This argument must not be NULL.
    //! @param[in] functionName Python module or submodule function name. This argument must not
    //! be NULL.
    //! @param[out] functionReference Python module or submodule function reference. This argument
    //! must not be NULL.
    //! @return int32_t A status code indicating whether the function call succeeded. A value equal 
    //! to 0 indicates success.
    //!
    int32_t Python_GetFunctionReference     (const PyObject*    module,
                                             const char*        functionName,
                                             PyObject** const   functionReference);

    //! @brief Call Python_ReleaseFunctionReference to release a Python function reference.
    //!
    //! @param[in] functionReference Python module or submodule function reference. This argument 
    //! must not be NULL.
    //! @return int32_t A status code indicating whether the function call succeeded. A value equal 
    //! to 0 indicates success.
    //!
    int32_t Python_ReleaseFunctionReference (PyObject** const   functionReference);

    //! @brief Call Python_Error to determine whether a Python/C API function call succeeded.
    //!
    //! @param[in] context The context in which the error occurred. This can be any string that is 
    //! useful for debugging purposes.
    //! @return int32_t A status code indicating whether the function call succeeded. A value equal 
    //! to 0 indicates success.
    //!
    int32_t Python_Error                    (const char*        context);

    //! @brief Call Python_CloseInterpreter to close the Python interpreter.
    //!
    void    Python_CloseInterpreter         (void);

#ifdef __cplusplus
}
#endif

// =================================================================================================
#endif	// __PYTHONSUPPORT_H__
// =================================================================================================
