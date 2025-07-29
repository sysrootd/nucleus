.syntax unified
.thumb

.global thread_launch
.global context_switch

// Launch first thread
thread_launch:
    msr psp, r0           // PSP = stack pointer
    movs r0, #2
    msr control, r0       // Use PSP, unprivileged
    isb

    pop {r4-r11}          // Restore callee-saved regs
    bx lr                  // Jump to thread entry (xPSR auto-restored)
    
// Context switch: save old_sp, restore new_sp
context_switch:
    mrs r2, psp
    stmdb r2!, {r4-r11}
    str r2, [r0]             // Save old_sp

    ldr r2, [r1]
    ldmia r2!, {r4-r11}
    msr psp, r2
    bx lr
