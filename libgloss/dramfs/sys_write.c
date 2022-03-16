#include <errno.h>
#include <sys/types.h>
#include "dramfs_fdtable.h"
#include "dramfs_fs.h"

/* Write to a file.  */
ssize_t
_write(int fd, const void *ptr, size_t len)
{
  if(dramfs_check_fd(fd) < 0) {
    return -1;
  }

  // write to console if it's stdout
  if(fd == 1 || fd==2) {
    for(int i=0; i<len; i++){
      char* cp = (char*) ptr;
      dramfs_sendchar(cp[i]);
    }
    return len;
  }

  lfs_file_t *fptr = dramfs_get_file(fd);
  return (ssize_t) lfs_file_write(&dramfs_fs, fptr, ptr, (lfs_size_t) len);
}
