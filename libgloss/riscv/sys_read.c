#include <errno.h>
#include <sys/types.h>
#include "bsg_newlib_fdtable.h"
#include "bsg_newlib_fs.h"

/* Read from a file.  */
ssize_t _read(int fd, void *ptr, size_t len)
{
  if(bsg_newlib_check_fd(fd) < 0) {
    return -1;
  }

  lfs_file_t *fptr = bsg_newlib_get_file(fd);
  return (ssize_t) lfs_file_read(&bsg_newlib_fs, fptr, ptr, (lfs_size_t) len);
}
