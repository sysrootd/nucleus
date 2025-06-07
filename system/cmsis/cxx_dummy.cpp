// system/cmsis/cxx_dummy.cpp

extern "C" int __cxa_guard_acquire(volatile int* g) {
    return !*g;
}

extern "C" void __cxa_guard_release(volatile int* g) {
    *g = 1;
}

extern "C" void __cxa_pure_virtual() {
    while (1); // Trap for pure virtual calls
}
