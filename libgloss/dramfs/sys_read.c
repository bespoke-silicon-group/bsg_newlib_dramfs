#include <errno.h>
#include <sys/types.h>
#include "dramfs_fdtable.h"
#include "dramfs_fs.h"

/* Read from a file.  */
ssize_t _read(int fd, void *ptr, size_t len)
{
  if(dramfs_check_fd(fd) < 0) {
    return -1;
  }

  lfs_file_t *fptr = dramfs_get_file(fd);
  return (ssize_t) lfs_file_read(&dramfs_fs, fptr, ptr, (lfs_size_t) len);
}
