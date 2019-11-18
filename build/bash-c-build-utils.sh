#!/bin/bash
# =================================================================================================
#
#   bash-c-build-utils.sh
#
#   Copyright (c) 2019 Unthinkable Research LLC. All rights reserved.
#
#   Supported host operating systems:
#       *nix systems capable of running bash shell.
#
#	Description:
#		This file contains a collection of bash build support functions.
#
# =================================================================================================

source "./bash-shell-utils.sh"
source "./bash-console-utils.sh"
source "./bash-file-system-utils.sh"

# Function to check for clang
function hasClang () {
	if cmdInstalled "clang"; then
		true
	else
		false
	fi
}

# Function to check for G++
function hasG++ () {
	if cmdInstalled "g++"; then
		true
	else
		false
	fi
}

# Function to check for GCC
function hasGcc () {
	if cmdInstalled "gcc"; then
		true
	else
		false
	fi
}

# Function to get GCC version
function gccVersion () {
	if hasGcc
	then
		GCC_VER="$(gcc --version)"
		if stringBeginsWithSubstring "$GCC_VER" "gcc "
		then
			GCC_VER="$(echo $GCC_VER | cut -d$' ' -f 4)"
			echo $GCC_VER
		else
			echo "unknown"
		fi
	else
		echo "N/A"
	fi
}

# Function to get GCC standard C version
function gccStdCVersion () {
	GCC_DEFS=$(gcc -dM -E - < /dev/null)
	C_STD_VERSION=${GCC_DEFS#*__STDC_VERSION__}
	C_STD_VERSION=${C_STD_VERSION:0:7}
}

# Function to check for C standard support
function gccSupportsCStandard () {
	C_STD_VERSION=0
	gccStdCVersion
	C_STD_YEAR=${C_STD_VERSION:0:5}
	C_STD_MONTH=${C_STD_VERSION:5:2}
	if [ "$C_STD_YEAR" -ge "$1" ]; then
		true
	else
		false
	fi
}

# Function to check for C99 support
function gccSupportsC99 () {
	GCC_DEFS=$(gcc -dM -E - < /dev/null)
	C_STD_VERSION=${GCC_DEFS#*__VERSION__}
	if [ "${C_STD_VERSION:2:1}" -ge "5" ]; then
		true
	elif [ "${C_STD_VERSION:2:1}" -eq "4" ] && [ "${C_STD_VERSION:4:1}" -ge "5" ]; then
		true
	else
		false
	fi
}

# Function to check for GNU ARM embedded toolchain
function hasGnuArmEmbeddedToolchain () {
	if cmdInstalled "arm-none-eabi-gcc"; then
		true
	else
		false
	fi
}

# Function to get GNU ARM embedded GCC version
function gnuArmEmbeddedGccStdCVersion () {
	GNU_ARM_DEFS=$(arm-none-eabi-gcc -dM -E - < /dev/null)
	GNU_ARM_STD_C_VERSION=${GNU_ARM_DEFS#*__STDC_VERSION__}
	GNU_ARM_STD_C_VERSION=${GNU_ARM_STD_C_VERSION:0:7}
}

# Function to check for GNU ARM embedded C standard support
function gnuArmEmbeddedGccSupportsCStandard () {
	GNU_ARM_STD_C_VERSION=0
	gnuArmEmbeddedGccStdCVersion
	GNU_ARM_STD_C_YEAR=${GNU_ARM_STD_C_VERSION:0:5}
	GNU_ARM_STD_C_MONTH=${GNU_ARM_STD_C_VERSION:5:7}
	if [ "$GNU_ARM_STD_C_YEAR" -ge "$1" ]; then
		true
	else
		false
	fi
}

# Function to check for make
function hasMake () {
	if cmdInstalled "make"; then
		true
	else
		false
	fi
}

# Function to get make version
function makeVersion () {
	if hasMake
	then
		MAKE_VER="$(make --version)"
		if stringBeginsWithSubstring "$MAKE_VER" "GNU Make "
		then
			MAKE_VER="$(echo $MAKE_VER | cut -d$' ' -f 3)"
			echo $MAKE_VER
		else
			echo "unknown"
		fi
	else
		echo "N/A"
	fi
}

# Function to clean 
# <name> <path> <makefilename> <verbose> <log>
function cleanIt () {
	printIt "Cleaning $1..."
	pushPath "$2" "$4"
	make clean --makefile="$3" > "$5" 2>&1
	if [ "$?" -ne 0 ]
	then
		cat "$5"
		popPath "$4"
		printIt " "
		printError "Clean of $1 failed!"
		printIt " "
		exit 1
	else
		if [ "$4" -eq 1 ]
		then
			cat "$5"
		fi
	fi
	popPath "$4"
}

# Function to build
# <name> <path> <makefilename> <verbose> <log> <defines>
function buildIt () {
	printIt "Building $1..."
	pushPath "$2" "$4"
	if [ "$6" == "" ]
	then
		make --makefile="$3" > "$5" 2>&1
	else
		make "$6" --makefile="$3" > "$5" 2>&1
	fi
	if [ "$?" -ne 0 ]
	then	
		cat "$5"
		popPath "$4"
		printIt " "
		printError "Build of $1 failed!"
		printIt " "
		exit 1
	else
		if [ "$4" -eq 1 ]
		then
			cat "$5"
		fi
	fi
	popPath "$4"
}

# Function to check for Git
function hasGit () {
	if cmdInstalled "git"; then
		true
	else
		false
	fi
}

# =================================================================================================
