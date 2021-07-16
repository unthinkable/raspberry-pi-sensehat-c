#!/bin/bash
# =================================================================================================
#
#   bash_unit_test_utils.sh
#
#   Copyright (c) 2019 Unthinkable Research LLC. All rights reserved.
#
#	Author: Gary Woodcock
#
#   Supported host operating systems:
#       *nix systems capable of running bash shell.
#
#	Description:
#		This file contains a collection of bash unit test support functions.
#
# =================================================================================================

source "./bash_console_utils.sh"
source "./bash_string_utils.sh"
source "./bash_misc_utils.sh"
source "./bash_file_system_utils.sh"

# Function to check for CUnit
function hasCUnit () {
	if hasMacPorts
	then
		if directoryExists "/opt/local/include/CUnit"
		then
			true
		else
			false
		fi
	else
		CUNIT_INSTALLED=$(ldconfig -p | grep libcunit)
		if stringHasSubstring "$CUNIT_INSTALLED" "libcunit"; then
			true
		else
			false
		fi
	fi
}

# Function to parse CUnit test results XML file
function parseCUnitResults()
{
	SUITES_TAG_FOUND=0
	SUITES_COUNT=-1
	SUITES_RUN=-1
	TEST_CASES_TAG_FOUND=0
	TEST_CASES_COUNT=-1
	TEST_CASES_RUN=-1
	TEST_CASES_SUCCEEDED=-1
	TEST_CASES_FAILED=-1
	ASSERTIONS_TAG_FOUND=0
	ASSERTIONS_COUNT=-1
	ASSERTIONS_RUN=-1
	ASSERTIONS_SUCCEEDED=-1
	ASSERTIONS_FAILED=-1
	MSG=""

	while read line; do
        echo "$line"
		# if [ $SUITES_TAG_FOUND == 0 ]
		# then
        #     echo "***SUITES TAG NOT FOUND"
		# 	SUITES_TAG="$(echo $line | grep "Suites" | awk -F">" '{print $2}' | awk -F"<" '{print $1}')"
		# 	if [ "$SUITES_TAG" == " Suites " ]
		# 	then
		# 		SUITES_TAG_FOUND=1
        #         echo "***SUITES TAG FOUND"
		# 	fi
		# elif [ $SUITES_TAG_FOUND == 1 ]
		# then
        #     echo "***SUITES TAG FOUND"
		# 	if [ $SUITES_COUNT == -1 ]
		# 	then
		# 		SUITES_COUNT="$(echo $line | grep '<TOTAL>' | awk -F">" '{print $2}' | awk -F"<" '{print $1}')"
		# 		SUITES_COUNT="$(echo -e "${SUITES_COUNT}" | tr -d '[:space:]')"
		# 		if [ $SUITES_COUNT == 1 ]
		# 		then
		# 			MSG="$SUITES_COUNT CUnit test suite total."
		# 		else
		# 			MSG="$SUITES_COUNT CUnit test suites total."
		# 		fi
		# 		printIt "$MSG"
		# 	elif [ $SUITES_COUNT != -1 ]
		# 	then
		# 		if [ $SUITES_RUN == -1 ]
		# 		then
		# 			SUITES_RUN="$(echo $line | grep '<RUN>' | awk -F">" '{print $2}' | awk -F"<" '{print $1}')"
		# 			SUITES_RUN="$(echo -e "${SUITES_RUN}" | tr -d '[:space:]')"
		# 			if [ $SUITES_RUN == 1 ]
		# 			then
		# 				MSG="$SUITES_RUN CUnit test suite run."
		# 			else
		# 				MSG="$SUITES_RUN CUnit test suites run."
		# 			fi
		# 			printIt "$MSG"
		# 		fi
		# 	fi
		# fi
		
		# if [ $TEST_CASES_TAG_FOUND == 0 ]
		# then
        #     echo "***TEST CASES TAG NOT FOUND"
		# 	TEST_CASES_TAG="$(echo $line | grep "Test Cases" | awk -F">" '{print $2}' | awk -F"<" '{print $1}')"
		# 	if [ "$TEST_CASES_TAG" == " Test Cases " ]
		# 	then
		# 		TEST_CASES_TAG_FOUND=1
        #         echo "***TEST CASES TAG FOUND"
		# 	fi
		# elif [ $TEST_CASES_TAG_FOUND == 1 ]
		# then
        #     echo "***TEST CASES TAG FOUND"
		# 	if [ $TEST_CASES_COUNT == -1 ]
		# 	then
		# 		TEST_CASES_COUNT="$(echo $line | grep '<TOTAL>' | awk -F">" '{print $2}' | awk -F"<" '{print $1}')"
		# 		TEST_CASES_COUNT="$(echo -e "${TEST_CASES_COUNT}" | tr -d '[:space:]')"
		# 		if [ $TEST_CASES_COUNT == 1 ]
		# 		then
		# 			MSG="$TEST_CASES_COUNT CUnit test case total."
		# 		else
		# 			MSG="$TEST_CASES_COUNT CUnit test cases total."
		# 		fi
		# 		printIt "$MSG"
		# 	elif [ $TEST_CASES_COUNT != -1 ]
		# 	then
		# 		if [ $TEST_CASES_RUN == -1 ]
		# 		then
		# 			TEST_CASES_RUN="$(echo $line | grep '<RUN>' | awk -F">" '{print $2}' | awk -F"<" '{print $1}')"
		# 			TEST_CASES_RUN="$(echo -e "${TEST_CASES_RUN}" | tr -d '[:space:]')"
		# 			if [ $TEST_CASES_RUN == 1 ]
		# 			then
		# 				MSG="$TEST_CASES_RUN CUnit test case run."
		# 			else
		# 				MSG="$TEST_CASES_RUN CUnit test cases run."
		# 			fi
		# 			printIt "$MSG"
		# 		elif [ $TEST_CASES_RUN != -1 ]
		# 		then
		# 			if [ $TEST_CASES_SUCCEEDED == -1 ]
		# 			then
		# 				TEST_CASES_SUCCEEDED="$(echo $line | grep '<SUCCEEDED>' | awk -F">" '{print $2}' | awk -F"<" '{print $1}')"
		# 				TEST_CASES_SUCCEEDED="$(echo -e "${TEST_CASES_SUCCEEDED}" | tr -d '[:space:]')"
		# 				if [ $TEST_CASES_SUCCEEDED == 1 ]
		# 				then
		# 					MSG="$TEST_CASES_SUCCEEDED CUnit test case succeeded."
		# 				else
		# 					MSG="$TEST_CASES_SUCCEEDED CUnit test cases succeeded."
		# 				fi
		# 				printSuccess "$MSG"
		# 			elif [ $TEST_CASES_SUCCEEDED != -1 ]
		# 			then
		# 				if [ $TEST_CASES_FAILED == -1 ]
		# 				then
		# 					TEST_CASES_FAILED="$(echo $line | grep '<FAILED>' | awk -F">" '{print $2}' | awk -F"<" '{print $1}')"
		# 					TEST_CASES_FAILED="$(echo -e "${TEST_CASES_FAILED}" | tr -d '[:space:]')"
		# 					if [ $TEST_CASES_FAILED == 1 ]
		# 					then
		# 						MSG="$TEST_CASES_FAILED CUnit test case failed."
		# 					else
		# 						MSG="$TEST_CASES_FAILED CUnit test cases failed."
		# 					fi
		# 					if [ $TEST_CASES_FAILED != 0 ]
		# 					then
		# 						printError "$MSG"
		# 					fi
		# 				fi
		# 			fi
		# 		fi
		# 	fi
		# fi 

		# if [ $ASSERTIONS_TAG_FOUND == 0 ]
		# then
        #     echo "***ASSERTIONS TAG NOT FOUND"
		# 	ASSERTIONS_TAG="$(echo $line | grep "Assertions" | awk -F">" '{print $2}' | awk -F"<" '{print $1}')"
		# 	if [ "$ASSERTIONS_TAG" == " Assertions " ]
		# 	then
		# 		ASSERTIONS_TAG_FOUND=1
        #         echo "***ASSERTIONS TAG FOUND"
		# 	fi
		# elif [ $ASSERTIONS_TAG_FOUND == 1 ]
		# then
        #     echo "***ASSERTIONS TAG FOUND"
		# 	if [ $ASSERTIONS_COUNT == -1 ]
		# 	then
		# 		ASSERTIONS_COUNT="$(echo $line | grep '<TOTAL>' | awk -F">" '{print $2}' | awk -F"<" '{print $1}')"
		# 		ASSERTIONS_COUNT="$(echo -e "${ASSERTIONS_COUNT}" | tr -d '[:space:]')"
		# 		if [ $ASSERTIONS_COUNT == 1 ]
		# 		then
		# 			MSG="$ASSERTIONS_COUNT CUnit assertion total."
		# 		else
		# 			MSG="$ASSERTIONS_COUNT CUnit assertions total."
		# 		fi
		# 		printIt "$MSG"
		# 	elif [ $ASSERTIONS_COUNT != -1 ]
		# 	then
		# 		if [ $ASSERTIONS_RUN == -1 ]
		# 		then
		# 			ASSERTIONS_RUN="$(echo $line | grep '<RUN>' | awk -F">" '{print $2}' | awk -F"<" '{print $1}')"
		# 			ASSERTIONS_RUN="$(echo -e "${ASSERTIONS_RUN}" | tr -d '[:space:]')"
		# 			if [ $ASSERTIONS_RUN == 1 ]
		# 			then
		# 				MSG="$ASSERTIONS_RUN CUnit assertion run."
		# 			else
		# 				MSG="$ASSERTIONS_RUN CUnit assertions run."
		# 			fi
		# 			printIt "$MSG"
		# 		elif [ $ASSERTIONS_RUN != -1 ]
		# 		then
		# 			if [ $ASSERTIONS_SUCCEEDED == -1 ]
		# 			then
		# 				ASSERTIONS_SUCCEEDED="$(echo $line | grep '<SUCCEEDED>' | awk -F">" '{print $2}' | awk -F"<" '{print $1}')"
		# 				ASSERTIONS_SUCCEEDED="$(echo -e "${ASSERTIONS_SUCCEEDED}" | tr -d '[:space:]')"
		# 				if [ $ASSERTIONS_SUCCEEDED == 1 ]
		# 				then
		# 					MSG="$ASSERTIONS_SUCCEEDED CUnit assertion succeeded."
		# 				else
		# 					MSG="$ASSERTIONS_SUCCEEDED CUnit assertions succeeded."
		# 				fi
		# 				printSuccess "$MSG"
		# 			elif [ $ASSERTIONS_SUCCEEDED != -1 ]
		# 			then
		# 				if [ $ASSERTIONS_FAILED == -1 ]
		# 				then
		# 					ASSERTIONS_FAILED="$(echo $line | grep '<FAILED>' | awk -F">" '{print $2}' | awk -F"<" '{print $1}')"
		# 					ASSERTIONS_FAILED="$(echo -e "${ASSERTIONS_FAILED}" | tr -d '[:space:]')"
		# 					if [ $ASSERTIONS_FAILED == 1 ]
		# 					then
		# 						MSG="$ASSERTIONS_FAILED CUnit assertion failed."
		# 					else
		# 						MSG="$ASSERTIONS_FAILED CUnit assertions failed."
		# 					fi
		# 					if [ $ASSERTIONS_FAILED != 0 ]
		# 					then
		# 						printError "$MSG"
		# 					fi
		# 				fi
		# 			fi
		# 		fi
		# 	fi
		# fi
	done < "$1"
}

# =================================================================================================
