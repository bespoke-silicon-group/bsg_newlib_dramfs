#include <errno.h>
#include "bsg_newlib_fdtable.h"
#include "bsg_newlib_fs.h"
#include "../../../../../bsg_manycore_lib/bsg_manycore.h"

/* Close a file.  */
int
_close(int fd)
{
  //bsg_printf("closing fd = %d\n", fd);
  if(bsg_newlib_check_fd(fd) < 0) {
    return -1;
  }

  return bsg_newlib_free_fd(fd);
}
