.section ".text.init"
.globl _start
.type  _start, @function
_start:
# Setup the global pointer
.option push
.option norelax
  la gp, _gp
.option pop

# Setup stack pointers.
# We then subtract off 8K*coreID. The top 4K is for the core emulation stack
# the lower 4K is for the program.
  la   sp, _sp
  csrr t0, mhartid
  slli t0, t0, 13
  sub  sp, sp, t0

# Save the stack pointer to mscratch, so it can be used on first trap entry
  csrw mscratch, sp
# Get 4K and subtract it off to get the program stack pointer
  li   t0, 1
  slli t0, t0, 12
  sub  sp, sp, t0

# Setup mtvec
  la t0, __mtvec_handler
  csrw mtvec, t0

# Enable FPU
  li t0, 0x00006000 & (0x00006000 >> 1);
  csrs mstatus, t0;
  csrwi fcsr, 0

# 0 all registers except sp and gp
  addi x1,  x0, 0
  addi x4,  x0, 0
  addi x5,  x0, 0
  addi x6,  x0, 0
  addi x7,  x0, 0
  addi x8,  x0, 0
  addi x9,  x0, 0
  addi x10, x0, 0
  addi x11, x0, 0
  addi x12, x0, 0
  addi x13, x0, 0
  addi x14, x0, 0
  addi x15, x0, 0
  addi x16, x0, 0
  addi x17, x0, 0
  addi x18, x0, 0
  addi x19, x0, 0
  addi x20, x0, 0
  addi x21, x0, 0
  addi x22, x0, 0
  addi x23, x0, 0
  addi x24, x0, 0
  addi x25, x0, 0
  addi x26, x0, 0
  addi x27, x0, 0
  addi x28, x0, 0
  addi x29, x0, 0
  addi x30, x0, 0
  addi x31, x0, 0

  # Clear the bss segment
  la      a0, _edata
  la      a2, _end
  sub     a2, a2, a0
  li      a1, 0
  call    memset

  # Initialize LFS
  call dramfs_fs_init
  bnez a0, 1f

  la   a0, __libc_fini_array # Register global termination functions
  call atexit                #  to be called upon exit
  call __libc_init_array     # Run global initialization functions

  # Load command line arguments
  la a0, _argc
  lw a0, 0(a0) # a0 = argc
  la a1, _argv # a1 = argv
  li a2, 0     # a2 = envp = NULL

  call main
1:
  tail exit
1:
  nop
  j 1b
  .size  _start, .-_start

# Stub trap handler
.globl __mtvec_handler
.weak  __mtvec_handler

__mtvec_handler:
  li a0, -1
  tail exit
  .size __mtvec_handler, .-__mtvec_handler

# Default command line args
.globl _argc
.weak  _argc
.globl _argv
.weak  _argv

_argc:
_argv:
  .word 0x00000000
  .size _argc, .-_argc

.section .sdata
.globl _gp
.weak  _gp
.align 1
.align 12
_gp:
  .size _gp, .-_gp

.globl _sp
.weak  _sp
.align 20
_sp:
  .size _sp, .-_sp
