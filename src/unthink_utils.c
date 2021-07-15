// =================================================================================================
//! @file unthink_utils.c
//! @author Gary Woodcock (gary.woodcock@unthinkable.com)
//! @brief This file defines utility functions.
//! @remarks Requires ANSI C99 (or better) compliant compilers.
//! @remarks Supported host operating systems: Any *nix
//! @date 2021-02-15
//! @copyright Copyright (c) 2021 Unthinkable Research LLC. All rights reserved.
//! 
//  Includes
// =================================================================================================
#include "unthink_utils.h"

// =================================================================================================
//  Unthink_AllocateMemory
// =================================================================================================
int32_t Unthink_AllocateMemory (size_t bufferSizeInBytes,
                                void** buffer)
{
    int32_t result = UNTHINK_SUCCESS;

    // Check arguments
    result = UNTHINK_CHECK_CONDITION((bufferSizeInBytes > 0), EINVAL);
    if (result == UNTHINK_SUCCESS)
        result = UNTHINK_CHECK_CONDITION((buffer != NULL), EINVAL);

    // Check status
    if (result == UNTHINK_SUCCESS)
    {   
        void* buf = NULL;

        // Setup
        *buffer = NULL;

        // Allocate the buffer
        buf = calloc(1, bufferSizeInBytes);
        result = UNTHINK_CHECK_CONDITION((buf != NULL), errno);
        if (result == UNTHINK_SUCCESS)
            *buffer = buf;
    }   // cppcheck-suppress memleak
    return result;
}

// =================================================================================================
//  Unthink_FreeMemory
// =================================================================================================
int32_t Unthink_FreeMemory (void** buffer)
{
    int32_t result = UNTHINK_SUCCESS;

    // Check argument
    result = UNTHINK_CHECK_CONDITION((buffer != NULL), EINVAL);
    if (result == UNTHINK_SUCCESS)
    {
        // Free the buffer
        if (*buffer != NULL)
        {
            free(*buffer);
            *buffer = NULL;
        }
    }
    return result;
}
                                  
// =================================================================================================
