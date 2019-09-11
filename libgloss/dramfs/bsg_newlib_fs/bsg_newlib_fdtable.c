#include "bsg_newlib_fdtable.h"
#include "bsg_newlib_fs.h"

typedef struct bsg_newlib_fd_entry {
  int used;
  lfs_file_t file;
} bsg_newlib_fd_entry_t;

static bsg_newlib_fd_entry_t bsg_newlib_fdtable[BSG_NEWLIB_MAX_FDS];

void bsg_newlib_init_fdtable(void) {
  for(int i=0; i<BSG_NEWLIB_MAX_FDS; i++) {
    bsg_newlib_fdtable[i].used = 0;
  }
}

int bsg_newlib_check_fd(int fd) {
  if(fd < 0 || fd >= BSG_NEWLIB_MAX_FDS || bsg_newlib_fdtable[fd].used == 0) {
    errno = EBADF;
    return -1;
  }

  return 0;
}

int bsg_newlib_reserve_fd(void) {
  int fd = 0;

  for(;fd < BSG_NEWLIB_MAX_FDS; fd++) {
    if(bsg_newlib_fdtable[fd].used == 0) {
      bsg_newlib_fdtable[fd].used = 1;
      return fd;
    }
  }

  errno = ENFILE;
  return -1;
}

lfs_file_t *bsg_newlib_get_file(int fd) {
  return &bsg_newlib_fdtable[fd].file;
}

int bsg_newlib_free_fd(int fd) {
  if (bsg_newlib_fdtable[fd].used == 1) {
    lfs_file_t *fptr = bsg_newlib_get_file(fd);
    bsg_newlib_fdtable[fd].used = 0;
    return lfs_file_close(&bsg_newlib_fs, fptr);
  } else {
    return 0;
  }
}
