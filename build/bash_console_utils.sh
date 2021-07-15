#!/bin/bash
# =================================================================================================
#
#   bash_console_utils.sh
#
#   Copyright (c) 2019 Unthinkable Research LLC. All rights reserved.
#
#   Supported host operating systems:
#       *nix systems capable of running bash shell.
#
#	Description:
#		This file contains a collection of bash console support functions.
#
# =================================================================================================

# Colors
CONSOLE_BLACK=0
CONSOLE_RED=1
CONSOLE_GREEN=2
CONSOLE_YELLOW=3
CONSOLE_BLUE=4
CONSOLE_MAGENTA=5
CONSOLE_CYAN=6
CONSOLE_WHITE=7

# Text colors
BANNER_COLOR=$CONSOLE_CYAN
SUCCESS_COLOR=$CONSOLE_GREEN
WARNING_COLOR=$CONSOLE_YELLOW
NOTE_COLOR=$CONSOLE_CYAN
ERROR_COLOR=$CONSOLE_RED
NORMAL_FORECOLOR=$CONSOLE_WHITE
NORMAL_BACKCOLOR=$CONSOLE_BLACK

# Function to determine whether shell is interactive
function isShellInteractive {
	if [ -t 1 ]
	then
		NCOLORS=$(tput colors)
		if test -n "$NCOLORS" && test $NCOLORS -ge 8; then
			true
		else
			false
		fi
	else
		false
	fi
}

# Function to set background color
function setBackgroundColor () {
	if isShellInteractive
	then
		tput setab "$1"
	fi
}

# Function to set foreground color
function setForegroundColor () {
	if isShellInteractive
	then
		tput setaf "$1"
	fi
}

# Function to reset console attributes
function resetConsoleAttributes () {
	if isShellInteractive
	then
		tput sgr0	
	fi
}

# Function to reset console color
function resetConsoleColor () {
	if isShellInteractive 
	then
		tput setab $NORMAL_BACKCOLOR
		tput setaf $NORMAL_FORECOLOR
	fi	
}

# Function to print
function printIt () {
	resetConsoleColor
	echo "$1"
}

# Function to print color
function printColor () {
	if isShellInteractive 
	then
		tput setab $NORMAL_BACKCOLOR
		tput setaf "$1"
	fi
	echo "$2"
	resetConsoleColor
}

# Function to print banner
function printBanner () {
	if isShellInteractive 
	then
		tput bold
		tput setab $NORMAL_BACKCOLOR
		tput setaf $BANNER_COLOR
	fi
	CHARCOUNT=$(echo -n "$1" | wc -c)
	((CHARCOUNT+=8))
	COUNTER=$CHARCOUNT
	while [ $COUNTER -gt 0 ]
	do
		echo -n "*"
		((COUNTER--))
	done
	echo ""
	echo -n "*** "
	echo -n "$1"
	echo " ***"
	COUNTER=$CHARCOUNT
	while [ $COUNTER -gt 0 ]
	do
		echo -n "*"
		((COUNTER--))
	done
	echo " "
	echo " "
	if isShellInteractive 
	then
		tput sgr0
	fi
	resetConsoleColor
}

# Function to print success
function printSuccess () {
	if isShellInteractive 
	then
		tput bold
		tput setab $NORMAL_BACKCOLOR
		tput setaf $SUCCESS_COLOR
	fi
	echo "$1"
	if isShellInteractive 
	then
		tput sgr0
	fi
	resetConsoleColor
}

# Function to print warning
function printWarning () {
	if isShellInteractive 
	then
		tput bold
		tput setab $NORMAL_BACKCOLOR
		tput setaf $WARNING_COLOR
	fi
	echo "WARNING: $1"
	if isShellInteractive 
	then
		tput sgr0
	fi
	resetConsoleColor
	printIt " "
}

# Function to print error
function printError () {
	if isShellInteractive 
	then
		tput bold
		tput setab $NORMAL_BACKCOLOR
		tput setaf $ERROR_COLOR
	fi
	echo "ERROR: $1"
	if isShellInteractive 
	then
		tput sgr0	
	fi
	resetConsoleColor
}

# Function to print note
function printNote () {
	if isShellInteractive 
	then
		tput bold
		tput setab $NORMAL_BACKCOLOR
		tput setaf $NOTE_COLOR
	fi
	echo "NOTE: $1"
	if isShellInteractive 
	then
		tput sgr0	
	fi
	resetConsoleColor
}

# =================================================================================================
