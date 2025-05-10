# STM32F401RBT6 Bare-Metal Kernel

This is a bare-metal kernel project for the STM32F401RBT6 microcontroller. The kernel is written in C++ and designed to run without an operating system, utilizing low-level hardware initialization, interrupt handling, and basic system setup.

## Project Structure

- **src/**: Contains C++ source files for the kernel.
- **include/**: Header files for kernel and drivers.
- **system/**: System initialization files (CMSIS, startup code, etc.).
- **build/**: Contains compiled object files, ELF, and BIN files.
- **linker.ld**: Custom memory layout for the STM32F401RBT6.
- **Makefile**: Build system for compiling and linking the project.

## Setup

Ensure you have the necessary tools installed:
- **arm-none-eabi-gcc** (for compiling and linking)
- **OpenOCD** (for flashing the STM32F401RBT6)
- **GDB** (for debugging)

## Building the Project

1. Clone the repository and navigate to the project directory.
2. Run `make` to build the project.
3. The output will be in `build/kernal.elf` and `build/kernal.bin`.

## Flashing the Microcontroller

Use OpenOCD or STM32CubeProgrammer to flash the `.bin` file to the STM32F401RBT6.

Example:
```bash
openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program build/kernal.bin verify reset exit"

openocd -f interface/stlink.cfg -f target/stm32f4x.cfg arm-none-eabi-gdb build/kernal.elf

### Step 4: Flashing and Debugging

Once your code is built, you'll want to flash it to your STM32F401RBT6 board using **OpenOCD** or **STM32CubeProgrammer**.

Example command for **OpenOCD**:

```bash
openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program build/kernal.bin verify reset exit"

openocd -f interface/stlink.cfg -f target/stm32f4x.cfg arm-none-eabi-gdb build/kernal.elf
