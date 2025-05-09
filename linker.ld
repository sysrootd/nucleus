/* linker.ld — for STM32F401RBT6 */

ENTRY(Reset_Handler)

MEMORY
{
  FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 128K
  RAM   (xrw) : ORIGIN = 0x20000000, LENGTH = 64K
}

SECTIONS
{
  .text :
  {
    KEEP(*(.isr_vector))       /* Vector table */
    *(.text*)                  /* Code */
    *(.rodata*)                /* Read-only data */
    _etext = .;
  } > FLASH

  .data : AT(_etext)
  {
    _sdata = .;
    *(.data*)
    _edata = .;
  } > RAM

  .bss :
  {
    _sbss = .;
    *(.bss*)
    *(COMMON)
    _ebss = .;
  } > RAM

  /* Stack at the end of RAM */
  _stack_top = ORIGIN(RAM) + LENGTH(RAM);
}

