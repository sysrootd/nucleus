.syntax unified
.cpu cortex-m4
.thumb

.global rtos_launch_first
.type rtos_launch_first, %function
rtos_launch_first:
    // Load rtos::g_current->sp
    ldr r0, =_ZN4rtos9g_currentE   // address of g_current (C++ mangled symbol)
    ldr r0, [r0]                   // g_current (TCB*)
    ldr r0, [r0]                   // t->sp

    msr psp, r0                    // set PSP = thread stack pointer
    movs r0, #2
    msr control, r0                // switch to PSP, unprivileged=0
    isb

    // Restore r4-r11 (software saved registers)
    pop {r4-r11}

    // Exception return sequence
    ldr lr, =0xFFFFFFFD            // return to Thread mode, PSP, no FP
    bx lr
