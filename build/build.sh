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

# Stash start date/time
SECONDS=0
BUILD_START="$(date)"

# Include bash utilities
source "./bash_architecture_utils.sh"
source "./bash_console_utils.sh"
source "./bash_c_build_utils.sh"
source "./bash_c_code_quality_utils.sh"
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
#	Functions
# =================================================================================================

# Print summary
function printSummary () {
	BUILD_DURATION=$SECONDS
	BUILD_STOP="$(date)"
	printBanner "SENSE HAT C LIBRARY BUILD SUMMARY"
	printIt " Build started at: $BUILD_START."
	printIt "Build finished at: $BUILD_STOP."

	if [ $(($BUILD_DURATION / 60)) -eq 0 ]
	then
		printIt "     Elapsed time: $(($BUILD_DURATION % 60)) seconds."
	elif [ $(($BUILD_DURATION / 60)) -eq 1 ]
	then
		printIt "     Elapsed time: $(($BUILD_DURATION / 60)) minute and $(($BUILD_DURATION % 60)) seconds."
	else
		printIt "     Elapsed time: $(($BUILD_DURATION / 60)) minutes and $(($BUILD_DURATION % 60)) seconds."
	fi

	printIt "    Configuration: Linux armhf"

	if [ $BUILD_DEBUG -eq 1 ]
	then
		printIt "                   Debug"
	else
		printIt "                   Release"
	fi

	if [ $BUILD_CLEAN -eq 1 ]
	then
		printIt "                   Clean build"
	else
		printIt "                   Incremental build"
	fi

	if [ $BUILD_TEST -eq 1 ]
	then
		printIt "                   With unit test"
	else
		printIt "                   Without unit test"
	fi

	if [ $BUILD_DOCS -eq 1 ]
	then
		printIt "		   With documenation"
	else
		printIt "		   Without documentation"
	fi

	printIt " "
	resetConsoleAttributes
}

# Check host enviroment
function checkHostEnvironment () {
	printBanner "HOST ENVIRONMENT CHECK"

	printf "Sense HAT Python module: "
	if hasPythonModule "sense_hat"
	then
		printColor $CONSOLE_GREEN "Installed"
	else
		printColor $CONSOLE_RED "Not installed"
	fi

	printf "             Python 2.x: "
	if hasPython2 
	then	
		PYTHON2_VERSION=$(pythonVersion python2)
		printColor $CONSOLE_GREEN "Installed (v$PYTHON2_VERSION)"
	else
		printColor $CONSOLE_RED "Not installed"
	fi

	printf "             Python 3.x: "
	if hasPython3
	then
		PYTHON3_VERSION=$(pythonVersion python3)
		printColor $CONSOLE_GREEN "Installed (v$PYTHON3_VERSION)"
	else
		printColor $CONSOLE_RED "Not installed"
	fi

	printf "                   make: "
	if hasMake
	then
		MAKE_VERSION=$(makeVersion)
		printColor $CONSOLE_GREEN "Installed (v$MAKE_VERSION)"
	else
		printColor $CONSOLE_RED "Not installed"
	fi

	printf "                    gcc: "
	if hasGcc
	then
		GCC_VERSION=$(gccVersion)
		printColor $CONSOLE_GREEN "Installed (v$GCC_VERSION)"
	else
		printColor $CONSOLE_RED "Not installed"
	fi

	printf "            C99 support: "
	if gccSupportsC99
	then
		printColor $CONSOLE_GREEN "Available"
	else
		printColor $CONSOLE_RED "Not available"
	fi

	printf "       clang scan-build: "
	if hasClang
	then
		printColor $CONSOLE_GREEN "Installed"
	else
		printColor $CONSOLE_RED "Not installed"
	fi

	printf "               cppcheck: "
	if hasCppcheck 
	then
		CPPCHECK_VERSION=$(cppcheckVersion)
		printColor $CONSOLE_GREEN "Installed (v$CPPCHECK_VERSION)"
	else
		printColor $CONSOLE_RED "Not installed"
	fi

	printf "                  CUnit: "
	if hasCUnit
	then
		printColor $CONSOLE_GREEN "Installed"
	else
		printColor $CONSOLE_RED "Not installed"
	fi

	printf "                Doxygen: "
	if hasDoxygen
	then
		DOXYGEN_VERSION=$(doxygenVersion)
		printColor $CONSOLE_GREEN "Installed (v$DOXYGEN_VERSION)"
	else
		printColor $CONSOLE_RED "Not installed"
	fi

	printIt " "
	exit 1
}

# Function to handle error
function handleError () {
	printIt " "
	printError "$1"
	printIt " "
	printSummary
	exit 1
}

# Function to print usage
function printUsage {
	printIt " "
	printColor $CONSOLE_GREEN "USAGE:  build.sh <args>"
	printIt " "
	printIt "	All arguments are optional. With no arguments, the default behavior is:"
	printIt " "
	printIt "	• Incremental debug build of libraries and programs"
	printIt "	• No unit testing"
	printIt "	• Minimal logging to console"
	printIt "	• No documentation build"
	printIt " "
	printIt "	Possible argument values are:"
	printIt " "
	printIt "	--check-env Checks the build support on the host environment."
	printIt "	--clean     Forces a clean build instead of an incremental build."
	printIt " 	--debug     Builds debug version."
	printIt "	--docs	    Builds documentation using Doxygen."
	printIt "	--help      Prints this usage notice."
	printIt "	--release   Builds release version."
	printIt "	--test      Runs unit tests after build."
	printIt "	--verbose   Prints all log output to console."
	printIt " "
	printIt "	Prerequisites for running this build script include:"
	printIt " "
	printIt "	• clang or clang-tools (for scan-build, used with --debug builds)"
	printIt "	• cppcheck"
	printIt "	• CUnit (used with --test builds)"
	printIt "	• Python (version 2 or 3)"
	printIt " "
	exit 1
}

# Function to parse command line arguments
function parseCommandLineArgument {
	if [ "$CMD_LINE_ARG" == "--check-env" ]
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

# Define build root
if ! shellVarDefined "$BUILD_ROOT"
then
	BUILD_ROOT="$HOME"
fi

# Set build defaults
BUILD_CFG=Debug
BUILD_CLEAN=0
BUILD_DEBUG=1
BUILD_DOCS=0
BUILD_RELEASE=0
BUILD_TEST=0
BUILD_VERBOSE=0
BUILD_PRODUCTS_DIR_NAME="raspberry-pi-sensehat-c"
BUILD_PRODUCTS_BIN_DIR="bin"
BUILD_PRODUCTS_OBJ_DIR="obj"
BUILD_LOG="/tmp/build-log.txt"

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

BUILD_ARCH_DIR="linux/armhf"

# Export build environment
export BUILD_CFG
export BUILD_ROOT
export BUILD_PRODUCTS_DIR_NAME
export BUILD_PRODUCTS_BIN_DIR
export BUILD_PRODUCTS_OBJ_DIR
export BUILD_ARCH_DIR

printIt " "

# Make sure there are no out-of-date libs that could be accidentally linked to
rm -f "$BUILD_PRODUCTS_HOME/$BUILD_PRODUCTS_DIR_NAME/$BUILD_PRODUCTS_BIN_DIR/$BUILD_ARCH_DIR/$BUILD_CFG"/*.*

# =================================================================================================
#	Clean
# =================================================================================================

# Clean?
if [ $BUILD_CLEAN -eq 1 ]
then
	printBanner "CLEAN"

	export BUILD_SHARED_LIB=1
	cleanIt "libsensehat.so" "../src" makefile $BUILD_VERBOSE $BUILD_LOG
	export BUILD_SHARED_LIB=0
	cleanIt "libsensehat.a" "../src" makefile $BUILD_VERBOSE $BUILD_LOG
	cleanIt "sensehat-example" "../example" makefile $BUILD_VERBOSE $BUILD_LOG
	if [ $BUILD_TEST -eq 1 ]
	then
		cleanIt "sensehat-test" "../test" makefile $BUILD_VERBOSE $BUILD_LOG
		if fileExists "../$BUILD_PRODUCTS_BIN_DIR/$BUILD_ARCH_DIR/$BUILD_CFG/Sense-HAT-C-Library-Unit-Test-Results.xml"
		then
			rm -f "../$BUILD_PRODUCTS_BIN_DIR/$BUILD_ARCH_DIR/$BUILD_CFG/Sense-HAT-C-Library-Unit-Test-Results.xml"
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
	printIt " "
fi

# =================================================================================================
#	Code quality analysis
# =================================================================================================

printBanner "ANALYZE"

if [ $BUILD_DEBUG -eq 1 ]
then

	printIt "Checking for clang scan-build..."
	SCANBUILD_AVAILABLE=0
	if hasScanBuild
	then
		SCANBUILD_AVAILABLE=1
	fi

	if [ $SCANBUILD_AVAILABLE -eq 0 ]
	then
		printWarning "clang scan-build not available."
	else
		export BUILD_SHARED_LIB=1
		scanIt "libsensehat" "../src" makefile $BUILD_VERBOSE $BUILD_LOG
		printIt "Cleaning up after clang scan-build runs..."
		cleanIt "libsensehat" "../src" makefile $BUILD_VERBOSE $BUILD_LOG
	fi
	printIt " "
fi

printIt "Checking for cppcheck..."
CPPCHECK_AVAILABLE=0
if cmdInstalled "cppcheck"
then
	CPPCHECK_AVAILABLE=1
fi

if [ $CPPCHECK_AVAILABLE -eq 0 ]
then
	printWarning "cppcheck not available."
else
	checkIt "libsensehat" "../src" $BUILD_VERBOSE $BUILD_LOG
fi

# =================================================================================================
#	Build
# =================================================================================================

printBanner "BUILD"

# Libraries
export BUILD_SHARED_LIB=1
buildIt "libsensehat.so" "../src" makefile $BUILD_VERBOSE $BUILD_LOG
export BUILD_SHARED_LIB=0
buildIt "libsensehat.a" "../src" makefile $BUILD_VERBOSE $BUILD_LOG

# Example
buildIt "sensehat-example" "../example" makefile $BUILD_VERBOSE $BUILD_LOG

# Unit tests
if [ $BUILD_TEST -eq 1 ]
then
	buildIt "sensehat-test" "../test" makefile $BUILD_VERBOSE $BUILD_LOG
	printIt " "
fi

# =================================================================================================
#	Unit test
# =================================================================================================

if [ $BUILD_TEST -eq 1 ]
then

	printBanner "UNIT TEST"

	pushPath "$BUILD_ROOT/$BUILD_PRODUCTS_DIR_NAME/$BUILD_PRODUCTS_BIN_DIR/$BUILD_ARCH_DIR/$BUILD_CFG" $BUILD_VERBOSE
	if fileExists "./Sense-HAT-C-Library-Unit-Test-Results.xml"
	then
		rm -f "./Sense-HAT-C-Library-Unit-Test-Results.xml"
	fi
	./sensehat-test
	printIt "Parsing libsensehat unit test results..."
	parseCUnitResults "./Sense-HAT-C-Library-Unit-Test-Results.xml"
	popPath $BUILD_VERBOSE
	printIt " "

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
	printIt " "
else
	printIt " "
fi

# =================================================================================================
#	Summary
# =================================================================================================

printSummary

# =================================================================================================
fi
# =================================================================================================
