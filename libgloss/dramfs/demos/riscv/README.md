# Newlib/DRAMFS RISC-V demo

## Installation

This project can be used with RISC-V GNU toolcahin by replacing the newlib 
submodule with this repository. This readme demos the usage of this BSP takes 
bare-metal spike as an example system. The same method can be follow to run 
this on real hardware bare-metal system. We used this project to succesfully 
run programs with file i/o and dynamic memory allocation on a bare-metal 
RISC-V processor that imeplements just user level RV32IM ISA excluding support 
for any kind of exceptions! 

RISC-V toolchainwith Newlib/Dramfs, for rv32im architecture, can be installed as below:

```
$ git clone --recursive https://github.com/riscv/riscv-gnu-toolchain.git
$ cd riscv-gnu-toolchain
$ git config --file=.gitmodules submodule.riscv-newlib.url https://github.com/bespoke-silicon-group/bsg_newlib_dramfs.git
$ git config --file=.gitmodules submodule.riscv-newlib.branch dramfs
$ git submodule sync
$ git submodule update --init --recursive --remote riscv-newlib
$ # Update the target name to append dramfs. Remove crt0.o installation.
$ echo '
diff --git a/Makefile.in b/Makefile.in
index a006b0a..098d2fd 100644
--- a/Makefile.in
+++ b/Makefile.in
@@ -53,7 +53,7 @@ endif
 
 make_tuple = riscv$(1)-unknown-$(2)
 LINUX_TUPLE  ?= $(call make_tuple,$(XLEN),linux-gnu)
-NEWLIB_TUPLE ?= $(call make_tuple,$(XLEN),elf)
+NEWLIB_TUPLE ?= $(call make_tuple,$(XLEN),elf-dramfs)
 MUSL_TUPLE ?= $(call make_tuple,$(XLEN),linux-musl)
 
 CFLAGS_FOR_TARGET := $(CFLAGS_FOR_TARGET_EXTRA) @cmodel@
@@ -520,8 +520,6 @@ stamps/merge-newlib-nano: stamps/build-newlib-nano stamps/build-newlib
 		$(INSTALL_DIR)/$(NEWLIB_TUPLE)/lib/$${mld}/libg_nano.a; \
 	    cp $(builddir)/install-newlib-nano/$(NEWLIB_TUPLE)/lib/$${mld}/libgloss.a\
 		$(INSTALL_DIR)/$(NEWLIB_TUPLE)/lib/$${mld}/libgloss_nano.a; \
-	    cp $(builddir)/install-newlib-nano/$(NEWLIB_TUPLE)/lib/$${mld}/crt0.o\
-		$(INSTALL_DIR)/$(NEWLIB_TUPLE)/lib/$${mld}/crt0.o; \
 	done
 # Copy nano header files into newlib install dir.
 	mkdir -p $(INSTALL_DIR)/$(NEWLIB_TUPLE)/include/newlib-nano; \ ' | git apply
$ ./configure --prefix=<riscv_install_dir> --disable-linux --with-arch=rv32im
$ make newlib
$ make install
```

This readme demos usage of this project taking bare-metal [Spike](https://github.com/riscv/riscv-isa-sim) 
as an example runtime-system. To reproduce the same, you have to install spike with a patch. The patch 
creates a memory-mapped IO in Spike that helps us print without needing the proxy kernel.

```
$ git clone --recursive https://github.com/riscv/riscv-isa-sim.git;
$ cd riscv-isa-sim
$ git checkout tags/v1.0.0
$ echo '
---
 fesvr/syscall.cc | 3 +++
 1 file changed, 3 insertions(+)

diff --git a/fesvr/syscall.cc b/fesvr/syscall.cc
index 6e8baf6..a2423eb 100644
--- a/fesvr/syscall.cc
+++ b/fesvr/syscall.cc
@@ -110,6 +110,9 @@ void syscall_t::handle_syscall(command_t cmd)
       std::cerr << "*** FAILED *** (tohost = " << htif->exit_code() << ")" << std::endl;
     return;
   }
+  else if (cmd.payload() & 0x2){
+      std::cerr << char(cmd.payload() >> 8);
+  }
   else // proxied system call
     dispatch(cmd.payload());
 
-- ' | git apply
$ ./configure --prefix=<riscv_install_dir>
$ make
$ make install
```

## Porting

Porting Newlib/Dramfs to a RISC-V system requires following four steps:
1. Implement `dramfs_exit(int)` (routine to exit the execution environment). 
2. Implement `dramfs_sendchar(char)` (routine to output non-file I/O (for debugging etc))
3. Call `dramfs_fs_init()` in the crt.
4. Define `_end` symbol (heap pointer) in the linker command file.

#### 1, 2. Interfacing functions:
```
/*
 * dramfs_intf.c
 * 
 * Sample Implementation of Newlib/DRAMFS interface
 * for Spike
 */

#include <stdlib.h>
#include <machine/dramfs_fs.h> // This header is installed with this project!

// Spike specific global variables.
// Defined these symbols in the linker script
extern volatile int tohost;
extern volatile int fromhost;

void dramfs_exit(int exit_status) {
  if(exit_status == EXIT_SUCCESS) {
    // Exit the environment successfully
    //
    // Replace below code with code to exit
    // successfully on your system.
    
    // Spike specifc code
    volatile int* ptr = &tohost;
    *ptr = 0x1;
  } else {
    // Exit the environment with failure
    // 
    // Replace below code with code to exit
    // with failure on your system.
    
    // Spike specifc code
    volatile int* ptr = &tohost;
    *ptr = 0x3;
  }

  // do nothing until we exit
  while(1);
}

void dramfs_sendchar(char ch) {
  // code to display ‘ch’
  // in many cases you may just want to have a memory
  // mapped I/O location that you write to
  // whether that is simulator magic, a NOC, a UART, or PCIe.
  
  // Spike specifc code
  volatile int* ptr = &tohost;
  *ptr = (ch << 8) | 0x2;
  while(fromhost == 0);
  fromhost = 0;
}
```

#### 3. Adding `dramfs_fs_init` call to C-runtime initialization code:

The function `dramfs_fs_init` has to called before calling the main. This steps mounts the LittleFS image using a tiny block device driver implemeted by us. A sample C-runtime initialization is provided below:

```
# crt.S

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
/* Sample linker command file for spike */

SECTIONS
{
  . = 0x80000000; /* Spike needs all user data after 0x80000000 */

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
```

## Usage

Running a program with Newlib/Dramfs requires user to link an additional file with 
initial LittleFS image consisting of input files. LittleFS image can be automatically 
generated by a tool called `dramfs_mklfs` that has already been installed with this 
package! The tool needs two inputs `lfs_block_size` and `lfs_block_count`. Total 
size of the file-system would be `lfs_block_size*lfs_block_count`.

After installation and porting, the procedure for running programs with Newlib/Dramfs 
is summarized by a sample program below. Let's say we want to run a file i/o program 
on a bare metal system which takes `hello.txt` as input.

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
$ <riscv_install_dir>/../riscv32-unknown-elf/bin/dramfs_mklfs 128 256 hello.txt > lfs_mem.c
$ <riscv_install_dir>/riscv32-unknown-elf-gcc -c crt.S lfs_mem.c dramfs_intf.c fhello.c
$ <riscv_install_dir>/riscv32-unknown-elf-gcc -nostartfiles -T link.ld lfs_mem.o crt.o dramfs_intf.o fhello.o  -o fhello
$ <riscv_install_dir>/spike -l --isa=rv32im fhello 2> spike.log

Hello! This is Little FS!
```
