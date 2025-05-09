

This project is a hobbyist, educational bare-metal kernel written in modern C++ targeting the STM32F401RBT6 ARM Cortex-M4 microcontroller. It is built entirely from scratch with no external OS dependencies and minimal runtime support.

## 📁 Project Structure

kernal/
├── src/ # C++ source files
├── include/ # Header files
├── system/ # Startup + CMSIS + system init
├── build/ # Compiled output (ELF, BIN)
├── linker.ld # Custom memory layout
├── Makefile # Build configuration
└── README.md


## ⚙️ Features (WIP)

- ✔️ Custom linker script and startup code
- ✔️ UART output (bare-metal)
- ⏳ Thread abstraction with context switching
- ⏳ Basic round-robin scheduler
- ⏳ CMSIS integration
- 🚀 Future: system calls, multitasking, synchronization primitives

## 🧱 Requirements

- ARM GCC Toolchain: `arm-none-eabi-g++`, `objcopy`, etc.
- STM32F401RBT6 board (e.g., STM32 Nucleo-F401RE)
- Flashing tools: `st-flash` or `OpenOCD`
- Make and a Unix-like environment (Linux, WSL, macOS)

## 🔨 Build

```bash
make

This generates:

    build/kernal.elf — ELF binary for debugging

    build/kernal.bin — Raw binary for flashing

🔥 Flash to Device

Using stlink:

make flash

Or manually:

st-flash write build/kernal.bin 0x8000000

🚧 Development Roadmap

Setup linker and startup

Bare-metal UART driver

Cooperative threading

Scheduler with SysTick

Minimal libc layer (optional)

    Drivers: GPIO, Timer, NVIC, etc.

🤝 Contributing

This is a personal educational project, but feel free to fork, clone, or suggest improvements.
📜 License

MIT License


---

Let me know if you want to include:
- Your GitHub username
- Screenshots or UART output samples
- A badge or toolchain install guide

Would you like me to drop this file directly into your project folder's content?
