ENTRY(Reset_Handler)

MEMORY
{
  FLASH(rx) : ORIGIN = 0x08000000, LENGTH = 128K
  SRAM(rwx) : ORIGIN = 0x20000000, LENGTH = 64K
}

SECTIONS
{
  /* Code and read-only data go to FLASH */
  .text : 
  {
    KEEP(*(.isr_vector))        /* Interrupt vectors */
    *(.text)
    *(.text.*)
    *(.init)
    *(.fini)
    *(.rodata)
    *(.rodata.*)
    . = ALIGN(4);
    _etext = .;
  } > FLASH

  /* Initialized data copied from FLASH to SRAM */
  _sidata = LOADADDR(.data);    /* Source address in FLASH */

  .data : 
  {
    _sdata = .;                 /* Start of data section in RAM */
    *(.data)
    *(.data.*)
    . = ALIGN(4);
    _edata = .;                 /* End of data section */
  } > SRAM AT > FLASH

  /* Uninitialized data (zero-initialized) */
  .bss (NOLOAD) :
  {
    _sbss = .;
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss.*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
    __bss_end__ = _ebss;
  } > SRAM

  /* Heap region */
  .heap (NOLOAD) :
  {
    _sheap = .;
    . = . + 0x1000; /* 4 KB heap */
    _eheap = .;
  } > SRAM

  /* Exception table */
  .ARM.exidx :
  {
    *(.ARM.exidx* .gnu.linkonce.armexidx.*)
  } > FLASH

  /* Symbols for memory boundaries */
  PROVIDE(end = _ebss);
  PROVIDE(__end__ = _ebss);
}
