// =================================================================================================
//! @file unthink_types.h
//! @author Gary Woodcock (gary.woodcock@unthinkable.com)
//! @brief This file defines frequently used types (or type includes).
//! @remarks Requires ANSI C99 (or better) compliant compilers.
//! @remarks Supported host operating systems: Any *nix
//! @date 2021-02-15
//! @copyright Copyright (c) 2021 Unthinkable Research LLC. All rights reserved.
//! 
//  Includes
// =================================================================================================
#ifdef __cplusplus
    #pragma once
#endif

#ifndef __UNTHINK_TYPES_H__
#define __UNTHINK_TYPES_H__

#include <errno.h>
#include <getopt.h>
#include <inttypes.h>
#include <math.h>
#include <memory.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// =================================================================================================
//  Constants
// =================================================================================================

//  Success and failure codes
#define UNTHINK_SUCCESS   EXIT_SUCCESS
#define UNTHINK_FAILURE   -1

// =================================================================================================
#endif	// __UNTHINK_TYPES_H__
// =================================================================================================
