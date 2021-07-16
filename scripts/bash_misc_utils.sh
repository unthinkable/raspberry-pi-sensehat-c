#!/bin/bash
# =================================================================================================
#
#   bash_misc_utils.sh
#
#   Copyright (c) 2019 Unthinkable Research LLC. All rights reserved.
#
#	Author: Gary Woodcock
#
#   Supported host operating systems:
#       *nix systems capable of running bash shell.
#
#	Description:
#		This file contains a collection of miscellaneous bash support functions.
#
# =================================================================================================

source "./bash_shell_utils.sh"
source "./bash_os_utils.sh"
source "./bash_string_utils.sh"

# Function to check for MacPorts
function hasMacPorts () {
	if isDarwin
	then
		if cmdInstalled "port"; then
			true
		else
			false
		fi
	else
		false
	fi
}

# Function to check for notify-send
function hasNotifySend () {
	if cmdInstalled "notify-send"; then
		true
	else
		false
	fi
}

# Function to check for jq
function hasJq () {
    if cmdInstalled "jq"; then
        true 
    else
        false
    fi
}

# Function to get jq version
function jqVersion () {
    if hasJq
    then
        JQ_VER="$(jq --version)"
        if stringBeginsWithSubstring "$JQ_VER" "jq-"
        then
            JQ_VER=${JQ_VER#"jq-"}
            printf $JQ_VER
        else
            printf "unknown"
        fi
    else
        printf "N/A"
    fi
}

# =================================================================================================

