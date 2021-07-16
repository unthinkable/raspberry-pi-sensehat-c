#!/bin/bash
# =================================================================================================
#
#   bash_check_env.sh
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

source "./bash_console_utils.sh"
source "./bash_python_utils.sh"

function bashCheck () {
    printWithRightJustification "bash: " "${1}"
    if hasBash
    then
        BASH_VERSION=$(bashVersion)
        printColor $CONSOLE_GREEN "Installed (v$BASH_VERSION)"
    else
        printColor $CONSOLE_RED "Not installed"
    fi
}

function clangCheck () {
	printWithRightJustification "clang: " "${1}"
	if hasClang
	then
		CLANG_VERSION=$(clangVersion)
		printColor $CONSOLE_GREEN "Installed (v$CLANG_VERSION)"
	else
		printColor $CONSOLE_RED "Not installed"
	fi
}

function clangC99Check () {
	printWithRightJustification "clang C99 support: " "${1}"
    if hasClang
    then
        if clangSupportsC99
        then
            printColor $CONSOLE_GREEN "Available"
        else
            printColor $CONSOLE_RED "Not available"
        fi
    else
        printColor $CONSOLE_RED "Not applicable"
    fi
}

function clangC11Check () {
	printWithRightJustification "clang C11 support: " "${1}"
    if hasClang
    then
        if clangSupportsC11
        then
            printColor $CONSOLE_GREEN "Available"
        else
            printColor $CONSOLE_RED "Not available"
        fi
    else
        printColor $CONSOLE_RED "Not applicable"
    fi
}

function clangC17Check () {
	printWithRightJustification "clang C17 support: " "${1}"
    if hasClang
    then
        if clangSupportsC17
        then
            printColor $CONSOLE_GREEN "Available"
        else
            printColor $CONSOLE_RED "Not available"
        fi
    else
        printColor $CONSOLE_RED "Not applicable"
    fi
}

function cppcheckCheck () {
	printWithRightJustification "cppcheck: " "${1}"
	if hasCppcheck 
	then
		CPPCHECK_VERSION=$(cppcheckVersion)
		printColor $CONSOLE_GREEN "Installed (v$CPPCHECK_VERSION)"
	else
		printColor $CONSOLE_RED "Not installed"
	fi
}

function cunitCheck () {
	printWithRightJustification "CUnit: " "${1}"
	if hasCUnit
	then
		printColor $CONSOLE_GREEN "Installed"
	else
		printColor $CONSOLE_RED "Not installed"
	fi
}

function doxygenCheck () {
	printWithRightJustification "Doxygen: " "${1}"
	if hasDoxygen
	then
		DOXYGEN_VERSION=$(doxygenVersion)
		printColor $CONSOLE_GREEN "Installed (v$DOXYGEN_VERSION)"
	else
		printColor $CONSOLE_RED "Not installed"
	fi
}

function gccCheck () {
	printWithRightJustification "gcc: " "${1}"
	if hasGcc
	then
		GCC_VERSION=$(gccVersion)
		printColor $CONSOLE_GREEN "Installed (v$GCC_VERSION)"
	else
		printColor $CONSOLE_RED "Not installed"
	fi
}

function gccC99Check () {
	printWithRightJustification "gcc C99 support: " "${1}"
    if hasGcc 
    then
        if gccSupportsC99
        then
            printColor $CONSOLE_GREEN "Available"
        else
            printColor $CONSOLE_RED "Not available"
        fi
    else
        printColor $CONSOLE_RED "Not applicable"
    fi
}

function gccC11Check () {
	printWithRightJustification "gcc C11 support: " "${1}"
    if hasGcc 
    then
        if gccSupportsC11
        then
            printColor $CONSOLE_GREEN "Available"
        else
            printColor $CONSOLE_RED "Not available"
        fi
    else
        printColor $CONSOLE_RED "Not applicable"
    fi
}

function gccC17Check () {
	printWithRightJustification "gcc C17 support: " "${1}"
    if hasGcc 
    then
        if gccSupportsC17
        then
            printColor $CONSOLE_GREEN "Available"
        else
            printColor $CONSOLE_RED "Not available"
        fi
    else
        printColor $CONSOLE_RED "Not applicable"
    fi
}

function gprofCheck () {
	printWithRightJustification "gprof: " "${1}"
	if hasGprof
	then
        GPROF_VERSION=$(gprofVersion)
		printColor $CONSOLE_GREEN "Installed (v$GPROF_VERSION)"
	else
		printColor $CONSOLE_RED "Not installed"
	fi
}

function jqCheck () {
    printWithRightJustification "jq: " "${1}"
    if hasJq
    then
        JQ_VERSION=$(jqVersion)
        printColor $CONSOLE_GREEN "Installed (v$JQ_VERSION)"
    else
        printColor $CONSOLE_RED "Not installed"
    fi
}

function makeCheck () {
	printWithRightJustification "make: " "${1}"
	if hasMake
	then
		MAKE_VERSION=$(makeVersion)
		printColor $CONSOLE_GREEN "Installed (v$MAKE_VERSION)"
	else
		printColor $CONSOLE_RED "Not installed"
	fi
}

function python2Check () {
    printWithRightJustification "Python 2.x: " "${1}"
    if hasPython2
    then
		PYTHON2_VERSION=$(pythonVersion python2)
		printColor $CONSOLE_GREEN "Installed (v$PYTHON2_VERSION)"
    else
        printColor $CONSOLE_RED "Not installed"
    fi
}

function python3Check () {
    printWithRightJustification "Python 3.x: " "${1}"
    if hasPython3
    then
		PYTHON3_VERSION=$(pythonVersion python3)
		printColor $CONSOLE_GREEN "Installed (v$PYTHON3_VERSION)"
    else
		printColor $CONSOLE_RED "Not installed"
    fi
}

function scanBuildCheck () {
	printWithRightJustification "scan-build: " "${1}"
	if hasScanBuild
	then
		printColor $CONSOLE_GREEN "Installed"
	else
		printColor $CONSOLE_RED "Not installed"
	fi
}

function senseHATPythonModuleCheck () {
    if hasPythonModule "sense_hat"
    then
        printColor $CONSOLE_GREEN "Installed"
    else
        printColor $CONSOLE_RED "Not installed"
    fi
}

function valgrindCheck () {
    printWithRightJustification "valgrind: " "${1}"
    if hasValgrind
    then
        VALGRIND_VERSION=$(valgrindVersion)
        printColor $CONSOLE_GREEN "Installed (v$VALGRIND_VERSION)"
    else
        printColor $CONSOLE_RED "Not installed"
    fi
}

# =================================================================================================
