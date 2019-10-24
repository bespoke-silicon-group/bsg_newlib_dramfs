SECTIONS
{
  . = 0x80000000;

  .text : {
     *(.crtbegin)
     *(.text)
     *(.text.startup)
     *(.text.*)
  }

  .data : {
    *(.data*)
    *(.rodata*)
    *(.sdata .sdata.* .srodata*)
  }

  /* global pointer */
  _gp = .;

  .bss : {
    *(.bss*)
    *(.sbss*)
  }


  /* 
   * 'tohost' and 'fromhost' are special symbols required by spike 
   * for communication with host and they need 64 byte aligned
   */

  . = ALIGN(64);
  tohost = .;
  . = . + 4;

  . = ALIGN(64);
  fromhost = .;
  . = . + 4;


  /* Initial heap pointer */
  _end = . ;
 
  /* Initial stack pointer */
  _sp = 0x81000000;
}
