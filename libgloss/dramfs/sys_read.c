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

  if(dramfs_check_fd(fd) == 0) {
    uint8_t *data = (uint8_t *)ptr;

    // Return early on len == 0
    if (len == 0) return (ssize_t) 0;

    // We buffer until len or newline, which is standard behavior for a console
    int ch;
    // Read just 1 character to start
    while ((ch = dramfs_getchar()) == -1);
    int i = 0;
    do {
      data[i++] = ch;
      if (i == len) break;
    } while ((ch = dramfs_getchar()) != -1);

    return (ssize_t) i;
  }

  lfs_file_t *fptr = dramfs_get_file(fd);
  return (ssize_t) lfs_file_read(&dramfs_fs, fptr, ptr, (lfs_size_t) len);
}
