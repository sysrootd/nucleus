# Toolchain
CXX = arm-none-eabi-g++
LD  = arm-none-eabi-ld
OBJCOPY = arm-none-eabi-objcopy
GDB = arm-none-eabi-gdb

# Target
TARGET = build/kernal.elf
BIN = build/kernal.bin

# Flags
MCU = -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=soft
CXXFLAGS = $(MCU) -Wall -Wextra -Os -ffreestanding -fno-exceptions -fno-rtti -Iinclude
LDFLAGS = -Tlinker.ld -nostartfiles $(MCU)

# Files
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:src/%.cpp=build/%.o)

# Default
all: $(BIN)

# Build steps
build/%.o: src/%.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $^

$(BIN): $(TARGET)
	$(OBJCOPY) -O binary $< $@

# Flash (optional, ST-Link)
flash: $(BIN)
	st-flash write $(BIN) 0x8000000

# Clean
clean:
	rm -rf build

.PHONY: all clean flash

