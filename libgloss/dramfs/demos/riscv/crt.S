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
