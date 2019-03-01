#include <errno.h>
#include "bsg_newlib_fs/bsg_newlib_fdtable.h"
#include "bsg_newlib_fs/bsg_newlib_fs.h"

/* Read from a file.  */
ssize_t _read(int file, void *ptr, size_t len)
{
  lfs_file_t lfs_file;

  if(check_fd(fd) < 0) {
    return -1;
  }

  lfs_file = bsg_newlib_fdtable[fd];
  return (ssize_t) lfs_file_read(&bsg_newlib_fs, &lfs_file, ptr, (lfs_size_t) len);
}
