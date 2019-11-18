#!/bin/bash
# =================================================================================================
#
#   bash-shell-utils.sh
#
#   Copyright (c) 2019 Unthinkable Research LLC. All rights reserved.
#
#   Supported host operating systems:
#       *nix systems capable of running bash shell.
#
#	Description:
#		This file contains a collection of bash shell support functions.
#
# =================================================================================================

# Function to test for definition of shell variable
function shellVarDefined () {
	if [ -z ${1+x} ]; then
		false
	else
		if [ "$1" = "" ]; then
			false
		else
			true
		fi
	fi
}

# Function to check for installation of command
function cmdInstalled () {
	if command -v "$1" >/dev/null 2>&1; then
		true 
	else
		false
	fi
}

# Function to check for existence of command
function cmdExists () {
	if [ -x "$1" ] >/dev/null 2>&1; then
		true
	else
		false
	fi
}

# =================================================================================================
