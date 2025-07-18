# Nucleus RTOS (STM32F401RBT6)

A minimal real-time operating system (RTOS) for the STM32F401RBT6 microcontroller, written in C++. This project demonstrates basic multitasking, GPIO control, and cooperative scheduling on ARM Cortex-M4 MCUs.

## Features

- Cooperative thread scheduler
- Thread stack management
- SysTick-based timing
- GPIO abstraction for STM32
- Example: Blinking two LEDs with different periods

## Directory Structure

```
nucleus/
├── src/           # Source code
├── include/       # Header files
├── build/         # Build output (created after compilation)
├── system/        # CMSIS and device files
├── .vscode/       # VS Code configuration (tasks, launch, settings)
├── Makefile       # Build script
└── README.md
```

## Requirements

- STM32F401RBT6 board (or compatible STM32F4)
- ARM GCC toolchain (`arm-none-eabi-gcc`)
- Make
- OpenOCD
- ST-Link programmer/debugger
- [Cortex-Debug VS Code extension](https://marketplace.visualstudio.com/items?itemName=marus25.cortex-debug)

## Building

From the project root, run:
```sh
make
```
This will generate `build/kernal.elf` and `build/kernal.bin`.

To clean:
```sh
make clean
```

## Flashing

Use OpenOCD and ST-Link to flash the binary:
```sh
openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program build/kernal.elf verify reset exit"
```

## Debugging

1. Install the **Cortex-Debug** extension in VS Code.
2. Connect your ST-Link to the board.
3. Press `F5` or select **Run > Start Debugging** in VS Code.

## VS Code Integration

- `.vscode/tasks.json`: Build and clean tasks
- `.vscode/launch.json`: Debug configuration for STM32F401RBT6 with OpenOCD
- `.vscode/settings.json`: Toolchain and file association settings

## License

MIT License

---

**Note:** Adjust pin numbers and device files as needed for your specific