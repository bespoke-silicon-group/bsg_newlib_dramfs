#include <errno.h>
#include "bsg_newlib_fs/bsg_newlib_fdtable.h"
#include "bsg_newlib_fs/bsg_newlib_fs.h"

/* Close a file.  */
int
_close(int file)
{
  lfs_file_t lfs_file;

  if(check_fd(fd) < 0) {
    return -1;
  }

  lfs_file = bsg_newlib_fdtable[fd];
  return lfs_file_close(&bsg_newlib_fs, &lfs_file);
}
