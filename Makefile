# Makefile for bare-metal STM32F401RBT6 project

# Toolchain and flags
CC = arm-none-eabi-g++
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump

#CFLAGS = -mcpu=cortex-m4 -mthumb -g -Wall -std=c++17 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -Iinclude


CFLAGS = -mcpu=cortex-m4 -mthumb -g -Wall -std=c++17 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti
LDFLAGS = -T linker.ld -nostartfiles -nostdlib -nodefaultlibs


INCDIRS = -Iinclude -Isystem
OBJDIR = build

# Source files
SRC = $(wildcard src/*.cpp system/*.cpp)
OBJ = $(patsubst %.cpp, $(OBJDIR)/%.o, $(notdir $(SRC)))

TARGET = kernal.elf
BIN = kernal.bin

# Default target
all: $(OBJDIR) $(TARGET)

# Link target
$(TARGET): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $@
	$(OBJCOPY) -O binary $@ $(BIN)
	$(OBJDUMP) -D $@ > kernal.lst

# Compile sources
$(OBJDIR)/%.o: src/%.cpp
	$(CC) $(CFLAGS) $(INCDIRS) -c $< -o $@

$(OBJDIR)/%.o: system/%.cpp
	$(CC) $(CFLAGS) $(INCDIRS) -c $< -o $@

# Create build dir
$(OBJDIR):
	mkdir -p $(OBJDIR)


burn: kernal.bin
	st-flash write kernal.bin 0x08000000 && st-flash reset

connect: kernal.elf
	openocd -f /usr/share/openocd/scripts/interface/stlink-v2.cfg \
	        -f /usr/share/openocd/scripts/target/stm32f4x.cfg

debug: kernal.elf
	gdb-multiarch kernal.elf \
	    -ex "target extended-remote localhost:3333" \
	    -ex "monitor reset halt" \
	    -ex "load" \
	    -ex "break main" \
	    -ex "continue"


# Clean
clean:
	rm -rf $(OBJDIR)/*.o $(TARGET) $(BIN) kernal.lst

.PHONY: all clean flash debug
