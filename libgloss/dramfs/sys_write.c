#include <errno.h>
#include <sys/types.h>
#include "bsg_newlib_fdtable.h"
#include "bsg_newlib_fs.h"

/* Write to a file.  */
ssize_t
_write(int fd, const void *ptr, size_t len)
{
  if(bsg_newlib_check_fd(fd) < 0) {
    return -1;
  }

  // write to console if it's stdout
  if(fd == 1 || fd==2) {
    for(int i=0; i<len; i++){
      char* cp = (char*) ptr;
      bsg_newlib_sendchar(cp[i]);
    }
  }

  lfs_file_t *fptr = bsg_newlib_get_file(fd);
  return (ssize_t) lfs_file_write(&bsg_newlib_fs, fptr, ptr, (lfs_size_t) len);
}
