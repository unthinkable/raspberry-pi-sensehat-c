// =================================================================================================
//! @file unthink_utils.h
//! @author Gary Woodcock (gary.woodcock@unthinkable.com)
//! @brief This file defines some useful general utilities.
//! @remarks Requires ANSI C99 (or better) compliant compilers.
//! @remarks Supported host operating systems: Any *nix
//! @date 2021-03-28
//! @copyright Copyright (c) 2021 Unthinkable Research LLC. All rights reserved.
//! 
//  Includes
// =================================================================================================
#ifdef __cplusplus
    #pragma once
#endif

#ifndef __UNTHINK_UTILS_H__
#define __UNTHINK_UTILS_H__

#include "unthink_types.h"

// =================================================================================================
//  Macros
// =================================================================================================

#if UNTHINK_ENABLE_CONSOLE_LOGGING
    //! @brief Use the UNTHINK_CHECK_CONDITION macro to assert a conditional check.
    //!
    #define UNTHINK_CHECK_CONDITION(condition,errcode)  \
    ({                                                  \
        int32_t status = UNTHINK_SUCCESS;               \
        if (condition == false)                         \
        {                                               \
            status = errcode;                           \
            fprintf(stdout,                             \
                    "At line %d in file %s: %s\n",      \
                    __LINE__,                           \
                    __FILE__,                           \
                    strerror(status));                  \
        }                                               \
        status;                                         \
    })

    //! @brief Use the UNTHINK_CHECK_ERROR macro to log an error.
    //!
    #define UNTHINK_CHECK_ERROR(errcode)                \
    ({                                                  \
        if (errcode != UNTHINK_SUCCESS)                 \
        {                                               \
            fprintf(stdout,                             \
                    "At line %d in file %s: %s\n",      \
                    __LINE__,                           \
                    __FILE__,                           \
                    strerror(errcode));                 \
        }                                               \
        errcode;                                        \
    })
#else
    //! @brief Use the UNTHINK_CHECK_CONDITION macro to assert a conditional check.
    //!
    #define UNTHINK_CHECK_CONDITION(condition,errcode)  \
    ({                                                  \
        int32_t status = UNTHINK_SUCCESS;               \
        if (condition == false)                         \
            status = errcode;                           \
        status;                                         \
    })

    //! @brief Use the UNTHINK_CHECK_ERROR macro to log an error.
    //!
    #define UNTHINK_CHECK_ERROR(errcode)                \
    ({                                                  \
        errcode;                                        \
    })
#endif

// =================================================================================================
//  Prototypes
// =================================================================================================
#ifdef __cplusplus
extern "C"
{
#endif

    //! @brief Call Unthink_AllocateMemory to dynamically allocate a memory buffer.
    //! 
    //! @param[in] bufferSizeInBytes The number of bytes to allocate.
    //! @param[out] buffer A pointer to a buffer pointer. This argument must not be NULL.
    //! @return int32_t A status code indicating whether the function call succeeded. A value equal 
    //! to UNTHINK_SUCCESS indicates success.
    //!
    int32_t Unthink_AllocateMemory (size_t bufferSizeInBytes,
                                    void** buffer);

    //! @brief Call Unthink_FreeMemory to release a dynamically allocated memory buffer.
    //! 
    //! @param[in] buffer A pointer to a buffer pointer. This argument must not be NULL, but the 
    //! value it points to can be NULL.
    //! @return int32_t A status code indicating whether the function call succeeded. A value equal 
    //! to UNTHINK_SUCCESS indicates success.
    //!
    int32_t Unthink_FreeMemory (void** buffer);

#ifdef __cplusplus
}
#endif

// =================================================================================================
#endif	// __UNTHINK_UTILS_H__
// =================================================================================================
