#!/bin/bash
# =================================================================================================
#
#   bash_console_utils.sh
#
#   Copyright (c) 2019 Unthinkable Research LLC. All rights reserved.
#
#	Author: Gary Woodcock
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
	printf "$1\n"
}

# Function to print with indent
# <string> <indent>
function printWithIndent () {
    resetConsoleColor
    for (( i=1; i<${2}; i++ ))
    do
        printf " "
    done
    printf "${1}"
}

# Function to print with right justification
# <string> <width>
function printWithRightJustification () {
    resetConsoleColor
    STR_LEN=`echo ${#1}`    
    PAD_LEN=$(( ${2}-$STR_LEN ))
    
    for (( i=1; i<$PAD_LEN; i++ ))
    do
        printf " "
    done
    printf "${1}"
}

# Function to print color
function printColor () {
	if isShellInteractive 
	then
		tput setab $NORMAL_BACKCOLOR
		tput setaf "$1"
	fi
	printf "$2\n"
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
	printf "$1\n"
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
	printf "$1\n"
	if isShellInteractive 
	then
		tput sgr0
	fi
	resetConsoleColor
}

# Function to print error
function printError () {
	if isShellInteractive 
	then
		tput bold
		tput setab $NORMAL_BACKCOLOR
		tput setaf $ERROR_COLOR
	fi
	printf "$1\n"
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
	printf "$1\n"
	if isShellInteractive 
	then
		tput sgr0	
	fi
	resetConsoleColor
}

# Function to print duration
function printDuration () {	
	DURATION=$1
	SECONDS_PER_HOUR=3600
	SECONDS_PER_MINUTE=60
	HOURS=$(( $DURATION / $SECONDS_PER_HOUR ))
	HOURS_IN_SECS=$(( $HOURS * $SECONDS_PER_HOUR ))
	DURATION=$(( $DURATION - $HOURS_IN_SECS ))
	MINUTES=$(( DURATION / $SECONDS_PER_MINUTE ))
	MINUTES_IN_SECS=$(( $MINUTES * $SECONDS_PER_MINUTE ))
	SECONDS=$(( $DURATION - $MINUTES_IN_SECS ))
	
    printWithRightJustification "Elapsed time: " "${2}"
	if [ $HOURS -eq 0 ]
	then
		if [ $MINUTES -eq 0 ]
		then
			if [ $SECONDS -eq 0 ]
			then
				printIt "0 seconds."
			elif [ $SECONDS -eq 1 ]
			then
				printIt "1 second."
			else
				printIt "$SECONDS seconds."
			fi
		elif [ $MINUTES -eq 1 ]
		then
			if [ $SECONDS -eq 0 ]
			then
				printIt "1 minute."
			elif [ $SECONDS -eq 1 ]
			then
				printIt "1 minute and 1 second."
			else
				printIt "1 minute and $SECONDS seconds."
			fi
		else
			if [ $SECONDS -eq 0 ]
			then
				printIt "$MINUTES minutes."
			elif [ $SECONDS -eq 1 ]
			then
				printIt "$MINUTES minutes and 1 second."
			else
				printIt "$MINUTES minutes and $SECONDS seconds."
			fi
		fi
	elif [ $HOURS -eq 1 ]
	then
		if [ $MINUTES -eq 0 ]
		then
			if [ $SECONDS -eq 0 ]
			then
				printIt "1 hour."
			elif [ $SECONDS -eq 1 ]
			then
				printIt "1 hour and 1 second."
			else
				printIt "1 hour and $SECONDS seconds."
			fi
		elif [ $MINUTES -eq 1 ]
		then
			if [ $SECONDS -eq 0 ]
			then
				printIt "1 hour and 1 minute."
			elif [ $SECONDS -eq 1 ]
			then
				printIt "1 hour, 1 minute and 1 second."
			else
				printIt "1 hour, 1 minute and $SECONDS seconds."
			fi
		else
			if [ $SECONDS -eq 0 ]
			then
				printIt "1 hour and $MINUTES minutes."
			elif [ $SECONDS -eq 1 ]
			then
				printIt "1 hour, $MINUTES minutes and 1 second."
			else
				printIt "1 hour, $MINUTES minutes and $SECONDS seconds."
			fi
		fi
	else
		if [ $MINUTES -eq 0 ]
		then
			if [ $SECONDS -eq 0 ]
			then
				printIt "$HOURS hours."
			elif [ $SECONDS -eq 1 ]
			then
				printIt "$HOURS hours and 1 second."
			else
				printIt "$HOURS hours and $SECONDS seconds."
			fi
		elif [ $MINUTES -eq 1 ]
		then
			if [ $SECONDS -eq 0 ]
			then
				printIt "$HOURS hours and 1 minute."
			elif [ $SECONDS -eq 1 ]
			then
				printIt "$HOURS hours, 1 minute and 1 second."
			else
				printIt "$HOURS hours, 1 minutes and $SECONDS seconds."
			fi
		else
			if [ $SECONDS -eq 0 ]
			then
				printIt "$HOURS hours and $MINUTES minutes."
			elif [ $SECONDS -eq 1 ]
			then
				printIt "$HOURS hours, $MINUTES minutes and 1 second."
			else
				printIt "$HOURS hours, $MINUTES minutes and $SECONDS seconds."
			fi
		fi
	fi
}

# =================================================================================================
