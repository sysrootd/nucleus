# Nucleus RTOS (STM32F401x)

A minimal real-time operating system (RTOS) for the STM32F401x microcontrollers, written in C++. This project demonstrates basic multitasking, GPIO control, and cooperative scheduling on ARM Cortex-M4 MCUs.

## Features

- Cooperative thread scheduler
- Thread stack management
- SysTick-based timing
- GPIO abstraction for STM32

## Directory Structure

nucleus/
├── src/ # Source code
├── include/ # Header files
├── build/ # Build output (created after compilation)
├── system/ # CMSIS and device files
├── tools/ # Code quality tools
│ ├── run-clang-format.py
│ └── run-clang-tidy.py
├── .vscode/ # VS Code configuration
├── Makefile # Build script
├── CMakeLists.txt
└── README.md
text


## Requirements

- STM32F401x board (or compatible STM32F4)
- ARM GCC toolchain (`arm-none-eabi-gcc`)
- Make
- OpenOCD
- ST-Link programmer/debugger
- [Cortex-Debug VS Code extension](https://marketplace.visualstudio.com/items?itemName=marus25.cortex-debug)

## Building
```sh
From the project root, run:
make

This will generate build/kernal.elf and build/kernal.bin.

To clean:
sh

make clean

Flashing

Use OpenOCD and ST-Link to flash the binary:
sh

openocd -f interface/stlink.cfg -f target/stm32f4x.cfg -c "program build/kernal.elf verify reset exit"

Debugging

    Install the Cortex-Debug extension in VS Code

    Connect your ST-Link to the board

    Press F5 or select Run > Start Debugging in VS Code

Code Formatting & Static Analysis
Prerequisites
bash

## Formatting

# Ubuntu/Debian
sudo apt-get install clang-format clang-tidy

Formatting Commands

Format all source files (run from project root):
bash

python3 tools/run-clang-format.py -i -r --style=LLVM ./src ./include

Static Analysis

First generate compilation database:
bash

cd build && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON .. && cd ..

Then run clang-tidy:
bash

python3 tools/run-clang-tidy.py -p ./build ./src ./include

Recommended Checks

For embedded development, consider these checks:
bash

-checks='-*,modernize-*,bugprone-*,readability-*,misc-*,-modernize-use-trailing-return-type'

VS Code Integration

Add to .vscode/settings.json:
json

{
  "editor.formatOnSave": true,
  "C_Cpp.clang_format_path": "clang-format",
  "C_Cpp.clang_format_style": "LLVM",
  "C_Cpp.codeAnalysis.clangTidy.enabled": true,
  "C_Cpp.codeAnalysis.clangTidy.path": "clang-tidy"
}

VS Code Configuration

    .vscode/tasks.json: Build and clean tasks

    .vscode/launch.json: Debug configuration for STM32F401RBT6 with OpenOCD

    .vscode/settings.json: Toolchain and file association settings
License

Copyright (c) 2025
Open to all, anyone can change and can be distribute under free software licence.
