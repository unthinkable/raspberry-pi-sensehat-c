#!/bin/bash
# =================================================================================================
#
#   bash-architecture-utils.sh
#
#   Copyright (c) 2019 Unthinkable Research LLC. All rights reserved.
#
#   Supported host operating systems:
#       *nix systems capable of running bash shell.
#
#	Description:
#		This file contains a collection of bash architecture support functions.
#
# =================================================================================================

# Function to check for ARM architecture
function isARMArchitecture () {
	if [ "$(expr substr $(uname -m) 1 3)" == "arm" ]
	then
		true
	else
		false
	fi
}

# Function to check for ARM 64-bit architecture
function isARM64Architecture () {
	if [ "$(expr substr $(uname -m) 1 7)" == "aarch64" ]
	then
		true
	else
		false
	fi
}

# Function to check for Intel 32-bit architecture
function isIntel32Architecture () {
	if [ "$(expr substr $(uname -m) 1 4)" == "i386" ] || [ "$(expr substr $(uname -m) 1 4)" == "i686" ]
	then
		true
	else
		false
	fi
}

# Function to check for Intel 64-bit architecture
function isIntel64Architecture () {
	if [ "$(expr substr $(uname -m) 1 6)" == "x86_64" ]
	then
		true
	else
		false
	fi
}

# =================================================================================================
