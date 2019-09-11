#include <errno.h>
#include "dramfs_fdtable.h"
#include "dramfs_fs.h"

/* Close a file.  */
int
_close(int fd)
{
  if(dramfs_check_fd(fd) < 0) {
    return -1;
  }

  return dramfs_free_fd(fd);
}
