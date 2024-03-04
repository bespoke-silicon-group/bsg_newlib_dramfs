#include <errno.h>
#include "dramfs_fdtable.h"
#include "dramfs_fs.h"
#include "systrace.h"

/* Close a file.  */
int
_close(int fd)
{
  if(dramfs_check_fd(fd) < 0) {
    return -1;
  }

  sys_tick(SYS_close);
  int ret = dramfs_free_fd(fd);
  sys_tock();

  return ret;
}
