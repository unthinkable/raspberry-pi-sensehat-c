#!/bin/bash
# =================================================================================================
#
#   build.sh
#
#   Copyright (c) 2019 Unthinkable Research LLC. All rights reserved.
#
#   Supported host operating systems:
#       Raspbian Stretch or later
#
# =================================================================================================

# Clear screen
clear

# Set built-ins
set -o errexit
set -o pipefail
set -o nounset

# Stash start date/time
SECONDS=0
BUILD_START="$(date)"

# Include bash utilities
source "./bash_architecture_utils.sh"
source "./bash_console_utils.sh"
source "./bash_c_build_utils.sh"
source "./bash_c_code_quality_utils.sh"
source "./bash_check_env.sh"
source "./bash_docs_utils.sh"
source "./bash_file_system_utils.sh"
source "./bash_os_utils.sh"
source "./bash_python_utils.sh"
source "./bash_shell_utils.sh"
source "./bash_string_utils.sh"
source "./bash_unit_test_utils.sh"

# Set background color 
setBackgroundColor $NORMAL_BACKCOLOR

# Set foreground color 
setForegroundColor $NORMAL_FORECOLOR

# =================================================================================================
#	Constants
# =================================================================================================

# Commands
ANALYZE_CMD="--analyze"
CHECK_ENV_CMD="--check-env"
CLEAN_CMD="--clean"
DEBUG_CMD="--debug"
DOCS_CMD="--docs"
HELP_CMD="--help"
PROFILE_CMD="--profile"
RELEASE_CMD="--release"
ROOT_DIRECTORY_PATH_CMD="--root-directory-path"
TEST_CMD="--test"
VERBOSE_CMD="--verbose"
WITH_CONSOLE_LOGGING_CMD="--with-console-logging"
WITH_SHARED_LIB_CMD="--with-shared-lib"

# Analyze options
ANALYZE_OPTION_CPPCHECK="cppcheck"
ANALYZE_OPTION_FULL="full"
ANALYZE_OPTION_NONE="none"
ANALYZE_OPTION_SCAN_BUILD="scan-build"
ANALYZE_OPTION_VALGRIND="valgrind"

# =================================================================================================
#	Functions
# =================================================================================================

# Print summary
function printSummary () {
	BUILD_DURATION=$SECONDS
	BUILD_STOP="$(date)"
    INDENT_LEN=20

	printBanner "SENSE HAT C LIBRARY BUILD SUMMARY"
	printWithRightJustification "Build started at: " $INDENT_LEN
    printIt "$BUILD_START."
	printWithRightJustification "Build finished at: " $INDENT_LEN
    printIt "$BUILD_STOP."
    printDuration $BUILD_DURATION $INDENT_LEN

    printWithRightJustification "Configuration: " $INDENT_LEN
	printIt "Linux armhf"

	if [ $BUILD_DEBUG -eq 1 ]
	then
		if [ $BUILD_CLEAN -eq 1 ]
		then
			if [ $BUILD_SHARED_LIB -eq 1 ]
			then
                printWithIndent "Clean debug build with shared libraries\n" $INDENT_LEN
			else
				printWithIndent "Clean debug build with static libraries\n" $INDENT_LEN
			fi
		else
			if [ $BUILD_SHARED_LIB -eq 1 ]
			then
				printWithIndent "Incremental debug build with shared libraries\n" $INDENT_LEN
			else
				printWithIndent "Incremental debug build with static libraries\n" $INDENT_LEN
			fi
		fi
	else
		if [ $BUILD_CLEAN -eq 1 ]
		then
            if [ $BUILD_SHARED_LIB -eq 1 ]
            then
				printWithIndent "Clean release build with shared libraries\n" $INDENT_LEN
            else
				printWithIndent "Clean release build with static libraries\n" $INDENT_LEN
            fi
		else
            if [ $BUILD_SHARED_LIB -eq 1 ]
            then
				printWithIndent "Incremental release build with shared libraries\n" $INDENT_LEN
            else
				printWithIndent "Incremental release build with static libraries\n" $INDENT_LEN
            fi
		fi
	fi

    if [ $BUILD_ANALYZE_OPTION == $ANALYZE_OPTION_CPPCHECK ] || [ $BUILD_ANALYZE_OPTION == $ANALYZE_OPTION_FULL ]
    then
        if hasCppcheck
        then
            printWithIndent "With cppcheck code quality analysis\n" $INDENT_LEN
        else
            printWithIndent "Without cppcheck code quality analysis\n" $INDENT_LEN
        fi
    else
        printWithIndent "Without cppcheck code quality analysis\n" $INDENT_LEN
    fi

    if [ $BUILD_ANALYZE_OPTION == $ANALYZE_OPTION_SCAN_BUILD ] || [ $BUILD_ANALYZE_OPTION == $ANALYZE_OPTION_FULL ]
    then
        if hasScanBuild
        then
            printWithIndent "With scan-build code quality analysis\n" $INDENT_LEN
        else
            printWithIndent "Without scan-build code quality analysis\n" $INDENT_LEN
        fi
    else
        printWithIndent "Without scan-build code quality analysis\n" $INDENT_LEN
    fi

    if [ $BUILD_ANALYZE_OPTION == $ANALYZE_OPTION_VALGRIND ] || [ $BUILD_ANALYZE_OPTION == $ANALYZE_OPTION_FULL ]
    then
        if hasValgrind
        then
            printWithIndent "With valgrind memory usage analysis\n" $INDENT_LEN
        else
            printWithIndent "Without valgrind memory usage analysis\n" $INDENT_LEN
        fi
    else
        printWithIndent "Without valgrind memory usage analysis\n" $INDENT_LEN
    fi

	if [ $BUILD_PROFILE -eq 1 ]
	then
		printWithIndent "With profiling\n" $INDENT_LEN
	else
        printWithIndent "Without profiling\n" $INDENT_LEN
	fi

	if [ $BUILD_TEST -eq 1 ]
	then
		printWithIndent "With unit testing\n" $INDENT_LEN
	else
		printWithIndent "Without unit testing\n" $INDENT_LEN
	fi

	if [ $BUILD_DOCS -eq 1 ]
	then
		printWithIndent "With documenation\n" $INDENT_LEN
	else
		printWithIndent "Without documentation\n" $INDENT_LEN
	fi

	if [ $BUILD_WITH_CONSOLE_LOGGING -eq 1 ]
	then
		printWithIndent "With console logging\n" $INDENT_LEN
	else
		printWithIndent "Without console logging\n" $INDENT_LEN
	fi

	printIt " "
	resetConsoleAttributes
}

# Check host enviroment
function checkHostEnvironment () {
	printBanner "HOST ENVIRONMENT CHECK"

    LABEL_WIDTH=26

    bashCheck $LABEL_WIDTH
    clangCheck $LABEL_WIDTH
    clangC99Check $LABEL_WIDTH
    clangC11Check $LABEL_WIDTH
    clangC17Check $LABEL_WIDTH
    cppcheckCheck $LABEL_WIDTH
    cunitCheck $LABEL_WIDTH
    doxygenCheck $LABEL_WIDTH
    gccCheck $LABEL_WIDTH
    gccC99Check $LABEL_WIDTH
    gccC11Check $LABEL_WIDTH
    gccC17Check $LABEL_WIDTH
    gprofCheck $LABEL_WIDTH
    makeCheck $LABEL_WIDTH
    python2Check $LABEL_WIDTH
    python3Check $LABEL_WIDTH
    scanBuildCheck $LABEL_WIDTH
    senseHATPythonModuleCheck $LABEL_WIDTH
    valgrindCheck $LABEL_WIDTH

    printIt " "
	exit 1
}

# Function to handle error
function handleError () {
	printIt "\n"
	printError "$1"
	printIt "\n"
	if hasNotifySend
	then
		notify-send "SenseHAT C Library Build" "Build failed!"
	fi
	printSummary
	exit 1
}

# Function to print usage
function printUsage {
	printIt " "
	printColor $CONSOLE_GREEN "USAGE:  build.sh <args>"
	printIt " "
	printIt "\tAll arguments are optional. With no arguments, the default behavior is:"
	printIt " "
	printIt "\t• Incremental debug build of libraries and programs"
	printIt "\t• Root directory path is '$BUILD_ROOT'"
    printIt "\t• Analyze with cppcheck"
	printIt "\t• No profiling"
    printIt "\t• No unit testing"
	printIt "\t• Minimal logging to console"
	printIt "\t• No documentation build"
	printIt " "
	printIt "\tPossible argument values are:"
	printIt " "
    printIt "\t$ANALYZE_CMD=<$ANALYZE_OPTION_FULL|$ANALYZE_OPTION_CPPCHECK|$ANALYZE_OPTION_SCAN_BUILD|$ANALYZE_OPTION_VALGRIND>\tAnalyzes the source code with the specified tools."
	printIt "\t$CHECK_ENV_CMD\t\t\t\t\tChecks the build support on the host environment."
	printIt "\t$CLEAN_CMD\t\t\t\t\t\tForces a clean build instead of an incremental build."
	printIt "\t$DEBUG_CMD\t\t\t\t\t\tBuilds debug version."
	printIt "\t$DOCS_CMD\t\t\t\t\t\tBuilds documentation using Doxygen."
	printIt "\t$HELP_CMD\t\t\t\t\t\tPrints this usage notice."
	printIt "\t$RELEASE_CMD\t\t\t\t\tBuilds release version."
	printIt "\t$ROOT_DIRECTORY_PATH_CMD=<path>\t\t\tSets the path to the root directory containing the"
    printIt "\t\t\t\t\t\t\tSenseHAT C library source code directory (defaults to"
    printIt "\t\t\t\t\t\t\tthe user's home directory)."
	printIt "\t$TEST_CMD\t\t\t\t\t\tRuns unit tests after build."
	printIt "\t$VERBOSE_CMD\t\t\t\t\tPrints all log output to console."
    printIt "\t$WITH_CONSOLE_LOGGING_CMD\t\t\t\tLog progress and debug information to the console."
	printIt "\t$WITH_SHARED_LIB_CMD\t\t\t\tBuild and link with a shared library instead of a"
    printIt "\t\t\t\t\t\t\tstatic library."
	printIt " "
	printIt "\tPrerequisites for running this build script include:"
	printIt " "
	printIt "\t• bash shell"
    printIt "\t• clang or gcc with C99 support"
	printIt "\t• cppcheck"
	printIt "\t• CUnit (used with --test option)"
	printIt "\t• Doxygen"
    printIt "\t• gprof (used with --profile option)"
    printIt "\t• make"
 	printIt "\t• Python (version 2 or 3)"
    printIt "\t• scan-build"
    printIt "\t• SenseHAT Python module"
    printIt "\t• valgrind"
	printIt " "
	exit 1
}

# Function to parse command line arguments
function parseCommandLineArgument {
    if stringBeginsWithSubstring "$CMD_LINE_ARG" "$ANALYZE_CMD"
    then
        BUILD_ANALYZE_OPTION=$(removeLeadingSubstring "$CMD_LINE_ARG" "$ANALYZE_CMD""=")
    elif stringBeginsWithSubstring "$CMD_LINE_ARG" "$ROOT_DIRECTORY_PATH_CMD"
	then
		BUILD_ROOT=$(removeLeadingSubstring "$CMD_LINE_ARG" $ROOT_DIRECTORY_PATH_CMD"=")
		if [ "$BUILD_ROOT" == "" ]
		then
			handleError "Root directory path must not be empty!"
		fi
		if ! stringEndsWithSubstring "$BUILD_ROOT" "/"
		then
			BUILD_ROOT="$BUILD_ROOT""/"
		fi
	elif [ "$CMD_LINE_ARG" == "--check-env" ]
	then
		checkHostEnvironment
	elif [ "$CMD_LINE_ARG" == "--clean" ]
	then
		BUILD_CLEAN=1
	elif [ "$CMD_LINE_ARG" == "--debug" ]
	then
		BUILD_DEBUG=1
		if [ $BUILD_RELEASE -eq 1 ]
		then
			BUILD_RELEASE=0
		fi
	elif [ "$CMD_LINE_ARG" == "--docs" ]
	then
		if hasDoxygen
		then
			BUILD_DOCS=1
		else
			BUILD_DOCS=0
			printWarning "Doxygen doesn't appear to be installed; overriding --docs."
		fi
	elif [ "$CMD_LINE_ARG" == "--help" ]
	then
		printUsage
	elif [ "$CMD_LINE_ARG" == "--release" ]
	then
		BUILD_RELEASE=1
		if [ $BUILD_DEBUG -eq 1 ]
		then
			BUILD_DEBUG=0
		fi
	elif [ "$CMD_LINE_ARG" == "--test" ]
	then
		if hasCUnit
		then
			BUILD_TEST=1
		else
			BUILD_TEST=0
			printWarning "CUnit doesn't appear to be installed; overriding --test."
		fi
	elif [ "$CMD_LINE_ARG" == "--verbose" ]
	then
		BUILD_VERBOSE=1
    elif [ "$CMD_LINE_ARG" == $WITH_CONSOLE_LOGGING_CMD ]
    then
        BUILD_WITH_CONSOLE_LOGGING=1
	elif [ "$CMD_LINE_ARG" == $WITH_SHARED_LIB_CMD ]
	then	
		BUILD_SHARED_LIB=1
	else
		printError "Unrecognized argument: '$CMD_LINE_ARG'."
		printIt ""
		printUsage
		exit 1
	fi
}

# =================================================================================================
#	Check host environment
# 	TODO: Check more specifically for Raspbian Stretch or later
# =================================================================================================
if ! isLinux || ! isARMArchitecture
then
	printError "This is not an ARM-based Linux environment!"
	printIt " "
else

# =================================================================================================
#	Setup
# =================================================================================================

# Set build defaults
BUILD_ROOT="$HOME"
BUILD_ANALYZE_OPTION="cppcheck"
BUILD_CFG=Debug
BUILD_CLEAN=0
BUILD_DEBUG=1
BUILD_DOCS=0
BUILD_LIB_EXTENSION=".a"
BUILD_PROFILE=0
BUILD_RELEASE=0
BUILD_TEST=0
BUILD_VERBOSE=0
BUILD_PRODUCTS_DIR_NAME="raspberry-pi-sensehat-c"
BUILD_PRODUCTS_BIN_DIR="bin"
BUILD_PRODUCTS_OBJ_DIR="obj"
BUILD_SHARED_LIB=0
BUILD_WITH_CONSOLE_LOGGING=0
CLEAN_LOG_PREFIX="_clean_log_"
BUILD_LOG_PREFIX="_build_log_"
CPPCHECK_LOG_PREFIX="_cppcheck_log_"
SCAN_BUILD_LOG_PREFIX="_scan_build_log_"
VALGRIND_LOG_PREFIX="_valgrind_log_"
LOG_POSTFIX=".txt"

# If there are build arguments, check them
for var in "$@"
do
	CMD_LINE_ARG=$var
	parseCommandLineArgument $CMD_LINE_ARG
done

printBanner "SENSE HAT C LIBRARY BUILD"

printIt "Setting up..."

# Sanity check debug vs. release
if [ $BUILD_DEBUG -eq 0 ]
then
	if [ $BUILD_RELEASE -eq 0 ]
	then
		BUILD_DEBUG=1
	fi
fi

# Export debug/release flag
if [ $BUILD_DEBUG -eq 1 ]
then
	BUILD_CFG=Debug
elif [ $BUILD_RELEASE -eq 1 ]
then
	BUILD_CFG=Release
fi

BUILD_LINUX=1
BUILD_OPERATING_ENV="linux"
BUILD_ARCH="armhf"

if fileExists "../include/unthink_console_logging.h"
then
	forceDeleteFile "../include/unthink_console_logging.h"
fi
echo "#define UNTHINK_ENABLE_CONSOLE_LOGGING $BUILD_WITH_CONSOLE_LOGGING" >> "../include/unthink_console_logging.h"

# Create build directory if necessary
if ! directoryExists "$BUILD_ROOT/$BUILD_PRODUCTS_DIR_NAME/"
then
	createDirectory "$BUILD_ROOT/$BUILD_PRODUCTS_DIR_NAME/"
fi

# Create the logs directory if necessary
BUILD_LOGS_DIR="$BUILD_ROOT/$BUILD_PRODUCTS_DIR_NAME/logs/"
if ! directoryExists "$BUILD_LOGS_DIR"
then
    createDirectory "$BUILD_LOGS_DIR"
fi

# Define program directory
PROG_DIR="$BUILD_ROOT/$BUILD_PRODUCTS_DIR_NAME/$BUILD_PRODUCTS_BIN_DIR/$BUILD_OPERATING_ENV/$BUILD_ARCH/$BUILD_CFG"

# Check library file extension
if [ $BUILD_SHARED_LIB -eq 1 ]
then
    BUILD_LIB_EXTENSION=".so"
fi

# Export build environment
export BUILD_CFG
export BUILD_ROOT
export BUILD_LOGS_DIR
export BUILD_PRODUCTS_DIR_NAME
export BUILD_PRODUCTS_BIN_DIR
export BUILD_PRODUCTS_OBJ_DIR
export BUILD_OPERATING_ENV
export BUILD_ARCH
export BUILD_PROFILE
export BUILD_SHARED_LIB
export BUILD_LIB_EXTENSION

printIt " "

# Make sure there are no out-of-date libs that could be accidentally linked to
forceDeleteDirectory "$PROG_DIR"/*.*

# =================================================================================================
#	Clean
# =================================================================================================

# Clean?
if [ $BUILD_CLEAN -eq 1 ]
then
	printBanner "CLEAN"

	CLEAN_TS=$(getDateTimeString)

	printIt "Deleting logs...\n"
	if directoryExists "$BUILD_LOGS_DIR"
	then
		deleteDirectory "$BUILD_LOGS_DIR"
		createDirectory "$BUILD_LOGS_DIR"
	fi

	cleanIt "libsensehat$BUILD_LIB_EXTENSION" "../src" makefile $BUILD_VERBOSE "$BUILD_LOGS_DIR/libsensehat$CLEAN_LOG_PREFIX$CLEAN_TS$LOG_POSTFIX"
	cleanIt "sensehat_example" "../example" makefile $BUILD_VERBOSE "$BUILD_LOGS_DIR/sensehat_example$CLEAN_LOG_PREFIX$CLEAN_TS$LOG_POSTFIX"
	if [ $BUILD_TEST -eq 1 ]
	then
		cleanIt "sensehat_test" "../test" makefile $BUILD_VERBOSE "$BUILD_LOGS_DIR/sensehat_test$CLEAN_LOG_PREFIX$CLEAN_TS$LOG_POSTFIX"
		if fileExists "../$BUILD_PRODUCTS_BIN_DIR/$BUILD_OPERATING_ENV/$BUILD_ARCH/$BUILD_CFG/Sense-HAT-C-Library-Unit-Test-Results.xml"
		then
			rm -f "../$BUILD_PRODUCTS_BIN_DIR/$BUILD_OPERATING_ENV/$BUILD_ARCH/$BUILD_CFG/Sense-HAT-C-Library-Unit-Test-Results.xml"
		fi
	fi
	if [ $BUILD_DOCS -eq 1 ]
	then
		printIt "Cleaning docs..."
		if directoryExists "../docs/html"
		then
			deleteDirectory "../docs/html"
		fi
		if directoryExists "../docs/latex"
		then
			deleteDirectory "../docs/latex"
		fi
	fi
fi

# =================================================================================================
#	Code quality analysis
# =================================================================================================

if [ $BUILD_ANALYZE_OPTION == $ANALYZE_OPTION_FULL ] || [ $BUILD_ANALYZE_OPTION == $ANALYZE_OPTION_CPPCHECK ]
then
    printBanner "CODE QUALITY ANALYSIS: CPPCHECK"

    printIt "Checking for cppcheck..."
    if ! hasCppcheck
    then
        printWarning "\tcppcheck not available.\n"
    else
        printSuccess "\tcppcheck available.\n"

        CPPCHECK_TS=$(getDateTimeString)

	    checkIt "src" "../src" $BUILD_VERBOSE "$BUILD_LOGS_DIR/src$CPPCHECK_LOG_PREFIX$CPPCHECK_TS$LOG_POSTFIX" ""
        checkIt "example" "../src" $BUILD_VERBOSE "$BUILD_LOGS_DIR/example$CPPCHECK_LOG_PREFIX$CPPCHECK_TS$LOG_POSTFIX" ""
        checkIt "test" "../test" $BUILD_VERBOSE "$BUILD_LOGS_DIR/test$CPPCHECK_LOG_PREFIX$CPPCHECK_TS$LOG_POSTFIX" ""
    fi
fi

if [ $BUILD_ANALYZE_OPTION == $ANALYZE_OPTION_FULL ] || [ $BUILD_ANALYZE_OPTION == $ANALYZE_OPTION_SCAN_BUILD ]
then
    printBanner "CODE QUALITY ANALYSIS: SCAN-BUILD"

    printIt "Checking for scan-build..."
    if ! hasScanBuild
    then
        printWarning "\tscan-build not available.\n"
    else
        printSuccess "\tscan-build available.\n"
        SCAN_BUILD_TS=$(getDateTimeString)

        scanIt "libsensehat" "../src" makefile $BUILD_VERBOSE "$BUILD_LOGS_DIR/libsensehat$SCAN_BUILD_LOG_PREFIX$SCAN_BUILD_TS$LOG_POSTFIX"
        scanIt "sensehat_example" "../example" makefile $BUILD_VERBOSE "$BUILD_LOGS_DIR/sensehat_example$SCAN_BUILD_LOG_PREFIX$SCAN_BUILD_TS$LOG_POSTFIX"
        scanIt "sensehat_test" "../src" makefile $BUILD_VERBOSE "$BUILD_LOGS_DIR/sensehat_test$SCAN_BUILD_LOG_PREFIX$SCAN_BUILD_TS$LOG_POSTFIX"
    fi
fi

# =================================================================================================
#	Build
# =================================================================================================

printBanner "BUILD"

BUILD_TS=$(getDateTimeString)

# Library
buildIt "libsensehat$BUILD_LIB_EXTENSION" "../src" makefile $BUILD_VERBOSE "$BUILD_LOGS_DIR/libsensehat$BUILD_LOG_PREFIX$BUILD_TS$LOG_POSTFIX" ""

# Example
buildIt "sensehat_example" "../example" makefile $BUILD_VERBOSE "$BUILD_LOGS_DIR/sensehat_example$BUILD_LOG_PREFIX$BUILD_TS$LOG_POSTFIX" ""

# Unit tests
if [ $BUILD_TEST -eq 1 ]
then
    buildIt "sensehat_test" "../test" makefile $BUILD_VERBOSE "$BUILD_LOGS_DIR/sensehat_test$BUILD_LOG_PREFIX$BUILD_TS$LOG_POSTFIX" ""
fi

# =================================================================================================
#	Memory usage analysis
# =================================================================================================

if [ $BUILD_ANALYZE_OPTION == $ANALYZE_OPTION_FULL ] || [ $BUILD_ANALYZE_OPTION == $ANALYZE_OPTION_VALGRIND ]
then
    printBanner "MEMORY USAGE ANALYSIS: VALGRIND"

    printIt "Checking for valgrind..."
    if ! hasValgrind
    then
        printWarning "\tvalgrind not available.\n"
    else
        printSuccess "\tvalgrind available.\n"

        VALGRIND_TS=$(getDateTimeString)
        
        pushPath "$BUILD_ROOT/$BUILD_PRODUCTS_DIR_NAME" $BUILD_VERBOSE

        printIt "Checking sensehat_example color cycle example with valgrind..."
        valgrind \
            --leak-check=full \
            --show-leak-kinds=all \
            --track-origins=yes \
            --log-file="$BUILD_LOGS_DIR/sensehat_example""$VALGRIND_LOG_PREFIX$VALGRIND_TS$LOG_POSTFIX" \
            "$PROG_DIR/sensehat_example" \
            -h 
        LOG_CONTENTS=$(<"$BUILD_LOGS_DIR/sensehat_example_color_cycle$VALGRIND_LOG_PREFIX$VALGRIND_TS$LOG_POSTFIX")
        SUMMARY=${LOG_CONTENTS#*ERROR SUMMARY: *}
        SUMMARY=${SUMMARY#*ERROR SUMMARY: *}
        SUMMARY=${SUMMARY% error*}
        if [ $SUMMARY == "0" ]
        then
            printSuccess "\tNo memory issues found in sensehat_example."
        else
            printWarning "\t$SUMMARY memory issues found in sensehat_example."
        fi
        popPath $BUILD_VERBOSE
    fi
fi

# =================================================================================================
#	Unit test
# =================================================================================================

if [ $BUILD_TEST -eq 1 ]
then

	printBanner "UNIT TEST"

    UNIT_TEST_TS=$(getDateTimeString)

	pushPath "$BUILD_ROOT/$BUILD_PRODUCTS_DIR_NAME/$BUILD_PRODUCTS_BIN_DIR/$BUILD_OPERATING_ENV/$BUILD_ARCH/$BUILD_CFG" $BUILD_VERBOSE
	if fileExists "./libsensehat_unit_test-Results.xml"
	then
		rm -f "./libsensehat_unit_test-Results.xml"
	fi
	"./sensehat_test"
	printIt "Parsing libsensehat unit test results..."
	parseCUnitResults "./libsensehat_unit_test-Results.xml"
    mv "./libsensehat_unit_test-Results.xml" "$BUILD_LOGS_DIR/libsensehat_unit_test_results_$UNIT_TEST_TS.xml"
	popPath $BUILD_VERBOSE

else
	printIt " "
fi

# =================================================================================================
#   Documenation
# =================================================================================================

if [ $BUILD_DOCS -eq 1 ]
then
	printBanner "DOCUMENTATION"

	printIt "Building HTML documentation at $BUILD_ROOT/$BUILD_PRODUCTS_DIR_NAME/docs/html..."

	pushPath "$BUILD_ROOT/$BUILD_PRODUCTS_DIR_NAME/docs" $BUILD_VERBOSE
	doxygen "./Doxyfile"
	if directoryExists "./html"
	then
		printIt "Opening index.html in browser..."
		xdg-open "./html/index.html"
	else
		printError "Failed to generate HTML documentation!"
	fi
	popPath $BUILD_VERBOSE
else
	printIt " "
fi

# =================================================================================================
#	Summary
# =================================================================================================

if hasNotifySend
then
	notify-send "SenseHAT C Library Build" "Build succeeded."
fi
printSummary

# =================================================================================================
fi
# =================================================================================================
