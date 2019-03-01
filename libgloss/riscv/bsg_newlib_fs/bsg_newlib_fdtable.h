#ifndef BSG_NEWLIB_FDTABLE_H
#define BSG_NEWLIB_FDTABLE_H

#include <errno.h>

#ifndef BSG_NEWLIB_MAX_FDS
#define BSG_NEWLIB_MAX_FDS 128
#endif

extern lfs_file_t bsg_newlib_fdtable[BSG_NEWLIB_MAX_FDS];

int check_fd(int fd);

#endif // BSG_NEWLIB_FDTABLE_H
