#!/bin/bash
# =================================================================================================
#
#   bash_string_utils.sh
#
#   Copyright (c) 2019 Unthinkable Research LLC. All rights reserved.
#
#	Author: Gary Woodcock
#
#   Supported host operating systems:
#       *nix systems capable of running bash shell.
#
#	Description:
#		This file contains a collection of bash string functions.
#
# =================================================================================================

# Function to check string for substring
function stringHasSubstring () {
	if [[ "$1" == *"$2"* ]]; then
		true 
	else
		false
	fi
}

# Function to check if string begins with substring
function stringBeginsWithSubstring () {
	if [[ "$1" == "$2"* ]]; then
		true
	else
		false
	fi
}

# Function to check if string ends with substring
function stringEndsWithSubstring () {
	if [[ "$1" == *"$2" ]]; then
		true
	else
		false
	fi
}

# Function to remove leading substring
function removeLeadingSubstring () {
	if stringBeginsWithSubstring "$1" "$2"
	then
		shopt -s extglob
		echo "${1##$2}"
	fi
}

# Function to convert decimal byte to hex byte
function convertDecimalByteToHexByte {
	printf "%02X" $1
}


# Function to get date in month/day/year number format
function getDateString {
	date "+%m/%d/%y"
}

# Function to get date/time in month-day-year-hour-minute-second format
function getDateTimeString {
	DTS=$(date "+%m-%d-%y at %T")
	echo "${DTS//:/$'.'}"
}

# =================================================================================================

