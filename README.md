# Newlib/DRAMFS (also known as Panic Room) 
###### Bandhav Veluri and Michael Taylor, Bespoke Silicon Group UW  

## Introduction

Newlib is a light-weight C standard library implementation for embedded systems that typical relies on a "board support package" to call out to real I/O devices. 
We provide a BSP that is based on a RAMDISK that you compile into your binary. 

It elegantly separates system specific functionality (system calls) into an easily portable portion called Libgloss. Libgloss contains system call implementations for different architectures/systems in it. Porting Newlib to an architecture/system essentially involves porting these system call implementations in Libgloss. Complete guide for porting Newlib can be found in [5].

Running POSIX programs on bare metal systems require some sort of implementation for file i/o and malloc. Malloc depends on just one system-call called sbrk, which essentially increments or decrements heap pointer as and when requested. Whereas, file i/o requires an actual file system, or an interface that can mimic a file system. This Newlib port, which, we informally refer to as *Panic Room*, uses an open-source lightweight file-system designed for embedded flash file systems by ARM called LittleFS (LFS). LittleFS also supports a DRAM-based file system, which is the one we use. 

The idea is to implement file i/o syscalls by simply translating them to LFS function calls, which in turn will operate on memory, rather than trying to use a more complex I/O emulation facility, like proxy kernels, that package up I/O calls and ship them over to a host. This allows for more reproducible I/O emulation, since the host is no longer an asynchronous process. It also eliminates the infrastructure pain caused by mapping these proxy-IO calls to different embodiments of the design -- simulator, VCS, verilator, FPGA emulation, ASIC testboard, and PCI-e hosted chip.

## Installation

Newlib/Dramfs is intended to be a separate Board Support Package (BSP) in Newlib. Newlib can be configured to install this BSP by setting the target to end with `dramfs`:

```
$ ./configure --target=<AnyArchiteture>-<AnyVendor>-<AnyString>dramfs [<other options>]
```

Since this BSP works with any ISA, architecture field could be anything!

## Porting

Porting Newlib/Dramfs requires following four steps:
1. Implement `dramfs_exit(int)` (routine to exit the execution environment). 
2. Implement `dramfs_sendchar(char)` (routine to output non-file I/O (for debugging etc)).
3. Implment C-runtime initiation including a call to `dramfs_fs_init()` in it.
4. Implement linker command script including definition of `_end` symbol (heap pointer) in it.

### Demo

See https://github.com/bespoke-silicon-group/bsg_newlib_dramfs/tree/dramfs/libgloss/dramfs/demos/riscv for a demo of Panic Room on RISCV.

#### 1, 2. Interfacing functions:
```
/*
 * dramfs_intf.c
 * 
 * Sample Implementation of Newlib/DRAMFS interface
 */

#include <stdlib.h>
#include <machine/dramfs_fs.h> // This header is installed with this BSP!

void dramfs_exit(int exit_status) {
  if(exit_status == EXIT_SUCCESS) {
    // Exit the environment successfully
    
    // Replace this with code to exit
    // successfully on your system.
  } else {
    // Exit the environment with failure
    
    // Replace this with code to exit
    // with failure on your system.
  }

  // do nothing until we exit
  while(1);
}

void dramfs_sendchar(char ch) {
  // code to display ‘ch’
  // in many cases you may just want to have a memory
  // mapped I/O location that you write to
  // whether that is simulator magic, a NOC, a UART, or PCIe.
}
```

#### 3. Adding `dramfs_fs_init` call to C-runtime initialization code:

The function `dramfs_fs_init` (implemented in this BSP) has to called before calling the main. This step mounts the LittleFS image using a tiny block device driver implemeted by us. A sample C-runtime initialization is provided below:

```
# Sample crt.S
# 
# This is architecture specific; hence user would implement this code
# Following example is for a RISC-V system.

.section .crtbegin,"a"

.globl _start
_start:

  # initialize global pointer
  la gp, _gp

  # initialize stack pointer
  la sp, _sp

  call dramfs_fs_init # initialize dramfs
  lw a0, 0(sp)        # argc
  lw a1, -4(sp)       # argv
  li a2, 0            # envp = NULL
  call main
  tail exit

2:
  # Should never this point
  j 2b
```

#### 4. Defining `_end` symbol in the linker command file:

This can be done as shown in the sample linker script below:

```
/* link.ld */
/* Sample linker command file */

SECTIONS
{
  . = 0x80000000; /* text and data start at 0x80000000 */

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

  /* Initial heap pointer */
  _end = . ;
 
  /* Initial stack pointer */
  _sp = 0x81000000;
}
```

## Usage

Running a program with Newlib/Dramfs requires user to link an additional file consisting of initial LittleFS image with input files to the program. LittleFS image can be automatically generated by a tool called `dramfs_mklfs` that has already been installed with this BSP! The tool needs two parameters `lfs_block_size` and `lfs_block_count`. Total size of the file-system would be `lfs_block_size*lfs_block_count`. See [2] for information about how to play with these parameters to improve performance.

Usage of `dramfs_mklfs`:
```
dramfs_mklfs <lfs_block_size> <lfs_block_count> <inputfile1> <inputdir1> <inputfile2> ...
```

After installation and porting, the procedure for running programs with Newlib/Dramfs is summarized by a sample program below. Let's say we want to run a file i/o program on a bare metal system which takes `hello.txt` as input.

```
$ cat fhello.c 
/*
 * fhello.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  int c;

  /* Open hello.txt for reading */
  FILE *hello = fopen("hello.txt", "r");
  if(hello == NULL)
    return -1;


  /* Iterate through the entire file
   * and print the contents to stdout
   */

  putchar('\n');

  while((c = fgetc(hello)) != EOF) {
    putchar(c);
  }

  fclose(hello);
  return 0;
}
$ cat hello.txt 
Hello! This is Little FS!
$ dramfs_mklfs 128 256 hello.txt > lfs_mem.c
$ <xxx>-<xxx>-<xxx>dramfs-gcc -c crt.S lfs_mem.c dramfs_intf.c fhello.c
$ <xxx>-<xxx>-<xxx>dramfs-gcc -nostartfiles -T link.ld lfs_mem.o crt.o dramfs_intf.o fhello.o  -o fhello
$ <Your system's runtime executable> fhello

Hello! This is Little FS!
```

## Notes

- Newlib is implemented on top of a set of operating system syscalls listed in [4]. Newlib/dramfs currently implements `sbrk`, `open`, `close`, `read`, `write`, `exit`, `lseek`, `stat` and `fstat`. These are sufficient to handle the vast majority of newlib POSIX calls. For those that are not supported, they return setting appropriate error number. We plan to increase this set over time, and also encourage your pull requests.
- Block size and count can be tweaked to improve performance depending on your system. More on this can be understood from [2].
- Current implementations of syscalls are *not* thread-safe. So, this can only be used for single threaded programs as of now.
- The modifications to newlib are solely contained to this file, and the directory https://github.com/bespoke-silicon-group/bsg_newlib_dramfs/tree/dramfs/libgloss/dramfs.

## References

[1] Bootstrapping Idea: https://drive.google.com/open?id=1_Ie94nZvyvMjEb1GQTPzfcBNaVs6xfVXZ_bHTH5_z3k  
[2] LittleFS: https://github.com/ARMmbed/littlefs  
[3] RISC-V BSP: https://github.com/riscv/riscv-newlib/tree/riscv-newlib-3.1.0/libgloss/riscv  
[4] Newlib OS interface: https://sourceware.org/newlib/libc.html#Syscalls  
[5] Newlib porting guide: https://www.embecosm.com/appnotes/ean9/ean9-howto-newlib-1.0.html 
