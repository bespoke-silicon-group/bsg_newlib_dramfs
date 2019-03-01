#include <errno.h>
#include "bsg_newlib_fs/bsg_newlib_fdtable.h"
#include "bsg_newlib_fs/bsg_newlib_fs.h"

/* Open a file.  */
int
_open(const char *name, int flags, int mode)
{
  int res;
  int fd;
  lfs_file_t file;

  res = lfs_file_open(&bsg_newlib_lfs, &file, name, flags);

  if(res < 0) {
    errno = res;
    return -1;
  } else {
    for(fd = 0; fd < BSG_NEWLIB_MAX_FDS; fd++) {
      if(bsg_newlib_fdtable[fd] == NULL) {
        bsg_newlib_fdtable[fd] = file;
        return fd;
      }
    }

    errno = ENFILE;
    return -1;
  }
}
