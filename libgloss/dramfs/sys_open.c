#include <errno.h>
#include <fcntl.h>
#include "dramfs_fdtable.h"
#include "dramfs_fs.h"

/* Open a file.  */
int
_open(const char *name, int flags, int mode)
{
  int fd = dramfs_reserve_fd();
  int lfs_flags = 0;

  // File open flags mapping
  lfs_flags |= (flags & O_RDONLY) ? LFS_O_RDONLY : 0;
  lfs_flags |= (flags & O_WRONLY) ? LFS_O_WRONLY : 0;
  lfs_flags |= (flags & O_RDWR  ) ? LFS_O_RDWR   : 0;
  lfs_flags |= (flags & O_APPEND) ? LFS_O_APPEND : 0;
  lfs_flags |= (flags & O_CREAT ) ? LFS_O_CREAT  : 0;
  lfs_flags |= (flags & O_TRUNC ) ? LFS_O_TRUNC  : 0;
  lfs_flags |= (flags & O_EXCL  ) ? LFS_O_EXCL   : 0;

  if(fd < 0) {
    return -1;
  } else {
    lfs_file_t *fptr = dramfs_get_file(fd);

    int ret = lfs_file_open(&dramfs_fs, fptr, name, lfs_flags);

    if(ret < 0) {
      errno = ret;
      return -1;
    } else {
      return fd;
    }
  }
}
