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
#      	This makefile builds the Raspberry Pi Sense HAT C library.
#
#   Notes:
#  		1)  This makefile assumes the use of ANSI C99 compliant compilers.
#
# =================================================================================================

# Command aliases
RM=rm
MKDIR=mkdir
CC=gcc
AR=ar

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

# If no architecture directory is specified, "linux/armhf" will be used
ifndef BUILD_ARCH_DIR
BUILD_ARCH_DIR=linux/armhf
endif

# If no configuration is specified, "Debug" will be used
ifndef BUILD_CFG
BUILD_CFG=Debug
endif

# If no library type is specified, "static" will be built
ifndef BUILD_SHARED_LIB
BUILD_SHARED_LIB=0
endif

# Define build and obj directories
BINDIR="$(BUILD_ROOT)/$(BUILD_PRODUCTS_DIR_NAME)/$(BUILD_PRODUCTS_BIN_DIR)/$(BUILD_ARCH_DIR)/$(BUILD_CFG)"
OBJDIR="$(BUILD_ROOT)/$(BUILD_PRODUCTS_DIR_NAME)/$(BUILD_PRODUCTS_OBJ_DIR)/$(BUILD_ARCH_DIR)/$(BUILD_CFG)"

# Define output library path/name
ifeq ($(BUILD_SHARED_LIB),0)
OUTFILE=$(BINDIR)/libsensehat.a
else
OUTFILE=$(BINDIR)/libsensehat.so
endif

# Create bin and obj directories
$(shell $(MKDIR) -p $(BINDIR))
$(shell $(MKDIR) -p $(OBJDIR))

# Define include directory paths
CFG_INC=-I../include \
	-I/usr/include \
	$(shell pkg-config --cflags python)

# Define library dependencies and directory paths
ifeq ($(BUILD_SHARED_LIB),0)
CFG_LIB=
else
CFG_LIB=-lpthread -ldl -lutil -lm $(shell pkg-config --libs python)
endif
CFG_LIB_INC=-L. \
	-L/usr/lib \
	-L/usr/local/lib \
	-L/usr/lib/arm-linux-gnueabihf

# Define object files
CFG_OBJ=
COMMON_OBJ=$(OBJDIR)/sensehat.o \
	$(OBJDIR)/unthink_utils.o \
	$(OBJDIR)/python-support.o 
OBJ=$(COMMON_OBJ) $(CFG_OBJ)

#
# Configuration: Debug
#
ifeq ($(BUILD_CFG),Debug)
COMPILE=$(CC) -Wall -c -g -o "$(OBJDIR)/$(*F).o" $(CFG_INC) "$<"
ifeq ($(BUILD_SHARED_LIB),0)
LINK=$(AR) cr "$(OUTFILE)" $(OBJ) $(CFG_LIB)
else
LINK=$(CC) -Wall "$(CFG_LIB_INC)" -g -o "$(OUTFILE)" -Xlinker -export-dynamic -Wl,-O1 -Wl,-Bsymbolic-functions $(OBJ) $(CFG_LIB) -shared -fPIC
endif
endif

#
# Configuration: Release
#
ifeq ($(BUILD_CFG),Release)
COMPILE=$(CC) -Wall -c -Os -DNDEBUG -o "$(OBJDIR)/$(*F).o" $(CFG_INC) "$<"
ifeq ($(BUILD_SHARED_LIB),0)
LINK=$(AR) cr "$(OUTFILE)" $(OBJ) $(CFG_LIB)
else
LINK=$(CC) -Wall "$(CFG_LIB_INC)" -o "$(OUTFILE)" -Xlinker -export-dynamic -Wl,-O1 -Wl,-Bsymbolic-functions $(OBJ) $(CFG_LIB) -shared -fPIC
endif
endif

# Pattern rule
$(OBJDIR)/%.o : %.c
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
