#include <stdlib.h>
#include <machine/dramfs_fs.h>

// Spike specific global variables.
// Declared in crt.S
extern volatile int tohost;
extern volatile int fromhost;

void dramfs_exit(int exit_status) {
  if(exit_status == EXIT_SUCCESS) {
    volatile int* ptr = &tohost;
    *ptr = 0x1;
  } else {
    volatile int* ptr = &tohost;
    *ptr = 0x3;
  }

  while(1);
}

void dramfs_sendchar(char ch) {
  volatile int* ptr = &tohost;
  *ptr = (ch << 8) | 0x2;
  while(fromhost == 0);
  fromhost = 0;
}
