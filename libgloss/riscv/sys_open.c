#include <errno.h>
#include "bsg_newlib_fdtable.h"
#include "bsg_newlib_fs.h"

/* Open a file.  */
int
_open(const char *name, int flags, int mode)
{
  int fd = bsg_newlib_reserve_fd();

  if(fd < 0) {
    return -1;
  } else {
    lfs_file_t *fptr = bsg_newlib_get_file(fd);

    // Open file using static buffer & lfs_file_opencfg. lfs_file_open uses malloc
    int res = lfs_file_opencfg(&bsg_newlib_fs, fptr, name, flags, &bsg_newlib_file_cfg);

    if(res < 0) {
      errno = res;
      return -1;
    } else {
      return fd;
    }
  }
}
