.syntax unified
.cpu cortex-m4
.thumb

.global context_switch
.type context_switch, %function

context_switch:
    // Save callee-saved registers r4–r11 of old thread
    mrs r2, psp
    stmdb r2!, {r4-r11}
    str r2, [r0]          // *old_sp = new value of stack

    // Load new SP and restore r4–r11
    ldr r2, [r1]          // r2 = *new_sp
    ldmia r2!, {r4-r11}
    msr psp, r2

    bx lr
