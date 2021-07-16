// =================================================================================================
//! @file unthink_macros.h
//! @author Gary Woodcock (gary.woodcock@unthinkable.com)
//! @brief This file defines some useful debugging and logging macros.
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

#ifndef __UNTHINK_MACROS_H__
#define __UNTHINK_MACROS_H__

#include "unthink_types.h"

// =================================================================================================
//  Macros
// =================================================================================================

#if UNTHINK_ENABLE_CONSOLE_LOGGING
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
    #define UNTHINK_CHECK_CONDITION(condition,errcode)  \
    ({                                                  \
        int32_t status = UNTHINK_SUCCESS;               \
        if (condition == false)                         \
            status = errcode;                           \
        status;                                         \
    })

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

    int32_t Unthink_AllocateMemory (size_t bufferSizeInBytes,
                                    void** buffer);

    int32_t Unthink_FreeMemory (void** buffer);

#ifdef __cplusplus
}
#endif

// =================================================================================================
#endif	// __UNTHINK_MACROS_H__
// =================================================================================================
