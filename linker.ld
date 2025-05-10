ENTRY(Reset_Handler)

MEMORY
{
  FLASH(rx) : ORIGIN = 0x08000000, LENGTH = 128K
  SRAM(rwx) : ORIGIN = 0x20000000, LENGTH = 64K
}

SECTIONS
{
  .text ALIGN(4) :
  {
    *(.isr_vector)
    *(.text)
    *(.text.*)
    *(.init)
    *(.fini)
    *(.rodata)
    *(.rodata.*)
    . = ALIGN(4);
    _etext = .;
  }> FLASH

  _la_data = LOADADDR(.data);

  .data ALIGN(4) : AT(LOADADDR(.data))
  {
    _sdata = .;
    *(.data)
    *(.data.*)
    . = ALIGN(4);
    _edata = .;
  }> SRAM

  .bss ALIGN(4) :
  {
    _sbss = .;
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss.*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
    __bss_end__ = _ebss;
  }> SRAM

  .heap ALIGN(8) (NOLOAD) :
  {
    _sheap = .;
    . = . + 0x1000; /* 4KB heap */
    _eheap = .;
  }> SRAM

  /* Optional: unwind info for stack traces */
  .ARM.exidx :
  {
    *(.ARM.exidx* .gnu.linkonce.armexidx.*)
  } > FLASH

  PROVIDE(end = _ebss);
  PROVIDE(__end__ = _ebss);
}

