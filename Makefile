# Makefile for bare-metal STM32F401RBT6 project

# Toolchain and Flags
CC = arm-none-eabi-g++
AS = arm-none-eabi-as
LD = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
GDB = arm-none-eabi-gdb

CFLAGS = -mcpu=cortex-m4 -mthumb -g -Wall -std=c++17 -ffunction-sections -fdata-sections
LDFLAGS = -T linker.ld -nostartfiles -nostdlib -nodefaultlibs
OBJDIR = build
SRCDIR = src
INCDIR = include
SYSTEMDIR = system

# Files and directories
SRC_FILES = $(wildcard $(SRCDIR)/*.cpp)
OBJ_FILES = $(SRC_FILES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
DEP_FILES = $(OBJ_FILES:.o=.d)

TARGET = kernal.elf

# Default target
all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) $(LDFLAGS) -o $(TARGET)
	$(OBJCOPY) -O binary $(TARGET) kernal.bin
	$(OBJDUMP) -D $(TARGET) > kernal.lst

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) $(CFLAGS) -I$(INCDIR) -I$(SYSTEMDIR) -c $< -o $@

-include $(DEP_FILES)

clean:
	rm -rf $(OBJDIR)/*.o $(OBJDIR)/*.d $(TARGET) kernal.bin kernal.lst

.PHONY: all clean

