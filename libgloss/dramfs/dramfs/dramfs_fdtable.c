#include "dramfs_fdtable.h"
#include "dramfs_fs.h"

typedef struct dramfs_fd_entry {
  int used;
  lfs_file_t file;
} dramfs_fd_entry_t;

static dramfs_fd_entry_t dramfs_fdtable[DRAMFS_MAX_FDS];

void dramfs_init_fdtable(void) {
  for(int i=0; i<DRAMFS_MAX_FDS; i++) {
    dramfs_fdtable[i].used = 0;
  }
}

int dramfs_check_fd(int fd) {
  if(fd < 0 || fd >= DRAMFS_MAX_FDS || dramfs_fdtable[fd].used == 0) {
    errno = EBADF;
    return -1;
  }

  return 0;
}

int dramfs_reserve_fd(void) {
  int fd = 0;

  for(;fd < DRAMFS_MAX_FDS; fd++) {
    if(dramfs_fdtable[fd].used == 0) {
      dramfs_fdtable[fd].used = 1;
      return fd;
    }
  }

  errno = ENFILE;
  return -1;
}

lfs_file_t *dramfs_get_file(int fd) {
  return &dramfs_fdtable[fd].file;
}

int dramfs_free_fd(int fd) {
  if (dramfs_fdtable[fd].used == 1) {
    lfs_file_t *fptr = dramfs_get_file(fd);
    dramfs_fdtable[fd].used = 0;
    return lfs_file_close(&dramfs_fs, fptr);
  } else {
    return 0;
  }
}
