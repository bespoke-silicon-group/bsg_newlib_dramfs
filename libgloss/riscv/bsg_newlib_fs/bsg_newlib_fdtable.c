#include "bsg_newlib_fdtable.h"

lfs_file_t bsg_newlib_fdtable[BSG_NEWLIB_MAX_FDS] = {NULL};

int check_fd(int fd) {
  if(fd < 0 || fd >= BSG_NEWLIB_MAX_FDS || bsg_newlib_fdtable[fd] == NULL) {
    errno = EBADF;
    return -1;
  }

  return 0;
}
