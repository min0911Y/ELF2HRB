SECTIONS
{
  . = 0x0;
  .text : { *(.text) }
  . = 4096*1024;
  .data : { *(.data) }
  .rodata : { *(.rodata) }
  .bss : { *(.bss) }
}