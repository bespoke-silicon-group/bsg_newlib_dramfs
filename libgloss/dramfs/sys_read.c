#include <errno.h>
#include <sys/types.h>
#include "dramfs_fdtable.h"
#include "dramfs_fs.h"

/* Read from a file.  */
ssize_t _read(int fd, void *ptr, size_t len)
{
  if(dramfs_check_fd(fd) < 0) {
    // Read from host if stdin not initialized
    if(fd == 0)
      goto host_getchar;
    else
      return -1;
  }

  // Read from LFS file
  lfs_file_t *fptr = dramfs_get_file(fd);
  ssize_t ret = lfs_file_read(&dramfs_fs, fptr, ptr, (lfs_size_t) len);

  // Fall through if stdin is empty otherwise return
  if(!((fd == 0) && (ret == 0)))
    return ret;

host_getchar:;
  uint8_t *data = (uint8_t *)ptr;

  // Return early on len == 0
  if (len == 0) return (ssize_t) 0;

  int ch;
  if (dramfs_nonblock_fd(fd) == 0) {
    // Block to read just 1 character to start
    while ((ch = dramfs_getchar()) == -1);
  } else {
    // Read the first character, and return immediately if it's EOF
    if ((ch = dramfs_getchar()) == -1) return (ssize_t) 0;
  }

  // Keep reading until new
  int i = 0;
  do {
    data[i++] = ch;
    if (i == len) break;
  } while ((ch = dramfs_getchar()) != -1);

  return (ssize_t) i;
}
