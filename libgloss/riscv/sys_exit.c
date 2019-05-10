#include <stdnoreturn.h>
#include "bsg_newlib_fs.h"

/* Exit a program without cleaning up files.  */
void
_exit(int exit_status) {
  bsg_newlib_exit(exit_status);
  while(1);
}
