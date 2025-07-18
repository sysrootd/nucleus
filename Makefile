# Toolchain and flags
CC = arm-none-eabi-g++
AS = arm-none-eabi-as
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump

CFLAGS = -mcpu=cortex-m4 -mthumb -g -Wall -std=c++17 \
         -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti

ASFLAGS = -mcpu=cortex-m4 -mthumb

# Linker script and libraries
LDFLAGS = -T linker.ld -nostartfiles -nodefaultlibs -lc -lgcc

# Directories (added system/cmsis here)
SRC_DIRS := src system system/cmsis

INCLUDE_DIRS := include include/drivers system system/cmsis

INCDIRS := $(addprefix -I, $(INCLUDE_DIRS))

OBJDIR := build

# Source files (recursive wildcard for nested dirs)
# Use shell find command for recursive search of cpp and s files
CPP_SRC := $(shell find $(SRC_DIRS) -name '*.cpp')
ASM_SRC := $(shell find $(SRC_DIRS) -name '*.s')

# Object files with build/ prefix preserving directory structure
CPP_OBJ := $(patsubst %.cpp,$(OBJDIR)/%.o,$(CPP_SRC))
ASM_OBJ := $(patsubst %.s,$(OBJDIR)/%.o,$(ASM_SRC))
OBJ := $(CPP_OBJ) $(ASM_OBJ)

# Remove duplicate .o entries
OBJ := $(sort $(OBJ))

TARGET = $(OBJDIR)/kernal.elf
BIN = $(OBJDIR)/kernal.bin
LST = $(OBJDIR)/kernal.lst

all: $(OBJDIR) $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) $(LDFLAGS) -o $@
	$(OBJCOPY) -O binary $@ $(BIN)
	$(OBJDUMP) -D $@ > $(LST)

# Compile .cpp files preserving directory structure under build/
$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCDIRS) -c $< -o $@

# Assemble .s files preserving directory structure under build/
$(OBJDIR)/%.o: %.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

burn: $(BIN)
	sleep 1
	st-flash --connect-under-reset write $(BIN) 0x08000000

connect: $(TARGET)
	openocd -f /usr/share/openocd/scripts/interface/stlink-v2.cfg \
	        -f /usr/share/openocd/scripts/target/stm32f4x.cfg

debug: $(TARGET)
	gdb-multiarch $(TARGET) \
	    -ex "target extended-remote localhost:3333" \
	    -ex "monitor reset halt" \
	    -ex "load" \
	    -ex "break Reset_Handler" \
	    -ex "continue"

clean:
	rm -rf $(OBJDIR)

.PHONY: all clean burn connect debug