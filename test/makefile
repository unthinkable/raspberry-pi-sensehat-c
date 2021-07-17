# =================================================================================================
#
#   makefile
#
#   Copyright (c) 2019 Unthinkable Research LLC. All rights reserved.
#
#   Supported host operating systems:
#       Raspbian Stretch or later
#
#   Description:
#      	This makefile builds the Raspberry Pi Sense HAT C library unit tests.
#
#   Notes:
#  		1)  This makefile assumes the use of ANSI C99 compliant compilers.
#
# =================================================================================================

# Command aliases
RM=rm
MKDIR=mkdir
CC=gcc

# If no build products root is specified, "$HOME" will be used
ifndef BUILD_ROOT
BUILD_ROOT="$(HOME)"
endif 

# If no build products directory name is specified, "raspberry-pi-sensehat-c" will be used
ifndef BUILD_PRODUCTS_DIR_NAME
BUILD_PRODUCTS_DIR_NAME=raspberry-pi-sensehat-c
endif

# If no binary directory is specified, "bin" will be used
ifndef BUILD_PRODUCTS_BIN_DIR
BUILD_PRODUCTS_BIN_DIR=bin
endif

# If no object directory is specified, "obj" will be used
ifndef BUILD_PRODUCTS_OBJ_DIR
BUILD_PRODUCTS_OBJ_DIR=obj
endif

# If no operating environment is specified, "darwin" will be used
ifndef BUILD_OPERATING_ENV
BUILD_OPERATING_ENV=linux
endif

# If no architecture is specified, "x64" will be used
ifndef BUILD_ARCH
BUILD_ARCH=armhf
endif

# If no configuration is specified, "Debug" will be used
ifndef BUILD_CFG
BUILD_CFG=Debug
endif

# If no library extension is specified, ".a" will be used
ifndef BUILD_LIB_EXTENSION
BUILD_LIB_EXTENSION=".a"
endif

# If no profiling is specified, profiling will be disabled
ifndef BUILD_PROFILE
BUILD_PROFILE=0
endif

# Define build and obj directories
BINDIR="$(BUILD_ROOT)/$(BUILD_PRODUCTS_DIR_NAME)/$(BUILD_PRODUCTS_BIN_DIR)/$(BUILD_OPERATING_ENV)/$(BUILD_ARCH)/$(BUILD_CFG)"
OBJDIR="$(BUILD_ROOT)/$(BUILD_PRODUCTS_DIR_NAME)/$(BUILD_PRODUCTS_OBJ_DIR)/$(BUILD_OPERATING_ENV)/$(BUILD_ARCH)/$(BUILD_CFG)"

# Define output executable path/name
OUTFILE=$(BINDIR)/sensehat_test

# Create bin and obj directories
$(shell $(MKDIR) -p $(BINDIR))
$(shell $(MKDIR) -p $(OBJDIR))

# Define include directory paths
CFG_INC=-I../include \
	-I/usr/include \
	-I/usr/include/CUnit \
	$(shell pkg-config --cflags python)

# Define library dependencies and directory paths
CFG_LIB=-lpthread -ldl -lutil -lm -lcunit $(shell pkg-config --libs python)
CFG_LIB_INC=-L. \
	-L/usr/lib \
	-L/usr/local/lib \
	-L/usr/lib/arm-linux-gnueabihf

# Define object files
CFG_OBJ=
COMMON_OBJ=$(OBJDIR)/sensehat_test.o 
OBJ=$(COMMON_OBJ) $(CFG_OBJ)

#
# Configuration: Debug
#
ifeq ($(BUILD_CFG),Debug)
ifeq ($(BUILD_PROFILE),0)
COMPILE=$(CC) -Wall -c -g -o "$(OBJDIR)/$(*F).o" $(CFG_INC) "$<"
LINK=$(CC) -Wall "$(CFG_LIB_INC)" -g -o "$(OUTFILE)" $(OBJ) "$(BINDIR)/libsensehat$(BUILD_LIB_EXTENSION)" $(CFG_LIB) 
else
COMPILE=$(CC) -Wall -pg -c -g -o "$(OBJDIR)/$(*F).o" $(CFG_INC) "$<"
LINK=$(CC) -Wall -pg "$(CFG_LIB_INC)" -g -o "$(OUTFILE)" $(OBJ) "$(BINDIR)/libsensehat$(BUILD_LIB_EXTENSION)" $(CFG_LIB) 
endif
endif

#
# Configuration: Release
#
ifeq ($(BUILD_CFG),Release)
ifeq ($(BUILD_PROFILE),0)
COMPILE=$(CC) -Wall -c -Os -DNDEBUG -o "$(OBJDIR)/$(*F).o" $(CFG_INC) "$<"
LINK=$(CC) -Wall "$(CFG_LIB_INC)" -o "$(OUTFILE)" $(OBJ) "$(BINDIR)/libsensehat$(BUILD_LIB_EXTENSION)"
else
COMPILE=$(CC) -Wall -pg -c -Os -DNDEBUG -o "$(OBJDIR)/$(*F).o" $(CFG_INC) "$<"
LINK=$(CC) -Wall -pg "$(CFG_LIB_INC)" -o "$(OUTFILE)" $(OBJ) "$(BINDIR)/libsensehat$(BUILD_LIB_EXTENSION)"
endif
endif

# Pattern rules
$(OBJDIR)/%.o : %.c
	$(COMPILE)

$(OBJDIR)/%.o : ../src/%.c
	$(COMPILE)

# Build rules
all: $(OUTFILE)

$(OUTFILE): $(OUTDIR)  $(OBJ)
	$(LINK)

# Rebuild this project
rebuild: cleanall all

# Clean this project
clean:
	$(RM) -f $(OUTFILE)
	$(RM) -f $(OBJ)

# Clean this project and all dependencies
cleanall: clean
