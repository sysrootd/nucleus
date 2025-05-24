# Toolchain and flags
CC = arm-none-eabi-g++
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump

CFLAGS = -mcpu=cortex-m4 -mthumb -g -Wall -std=c++17 \
         -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti

LDFLAGS = -T linker.ld -nostartfiles -nostdlib -nodefaultlibs

# Directories
SRC_DIRS := src src/drivers system
INCLUDE_DIRS := include include/drivers system system/cmsis

# Include flags
INCDIRS := $(addprefix -I, $(INCLUDE_DIRS))

# Output directories
OBJDIR := build

# Find all source files with relative paths
SRC := $(wildcard $(addsuffix /*.cpp, $(SRC_DIRS)))

# Convert source paths to object paths in build folder, preserving directory structure
OBJ := $(patsubst %.cpp, $(OBJDIR)/%.o, $(SRC))

# Targets
TARGET = kernal.elf
BIN = kernal.bin
LST = kernal.lst

# Default target
all: $(OBJDIR) $(TARGET)

# Link target
$(TARGET): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $@
	$(OBJCOPY) -O binary $@ $(BIN)
	$(OBJDUMP) -D $@ > $(LST)

# Rule to compile each .cpp file into .o preserving directory structure
$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCDIRS) -c $< -o $@

# Create main build directory
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Flash the chip
burn: $(BIN)
	sleep 1
	st-flash --reset write $(BIN) 0x08000000 && st-flash reset
	#st-flash write $(BIN) 0x08000000 && st-flash reset

# OpenOCD connection
connect: $(TARGET)
	openocd -f /usr/share/openocd/scripts/interface/stlink-v2.cfg \
	        -f /usr/share/openocd/scripts/target/stm32f4x.cfg

# Debug with gdb
debug: $(TARGET)
	gdb-multiarch $(TARGET) \
	    -ex "target extended-remote localhost:3333" \
	    -ex "monitor reset halt" \
	    -ex "load" \
	    -ex "break main" \
	    -ex "continue"

# Clean build files
clean:
	rm -rf $(OBJDIR) $(TARGET) $(BIN) $(LST)

.PHONY: all clean burn connect debug
