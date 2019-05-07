#include <stdio.h>
#include <stdlib.h>
#include "bsg_newlib_fs.h"
#include "../../../../../bsg_manycore_lib/bsg_manycore.h"

/* Exit a program without cleaning up files.  */
void
_exit(int exit_status)
{
  //bsg_printf("Exiting\n");
  if(exit_status == EXIT_SUCCESS) {
    bsg_finish();
  } else {
    bsg_fail();
  }

  bsg_wait_while(1);
}
