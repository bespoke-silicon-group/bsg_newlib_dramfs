#ifndef DRAMFS_FDTABLE_H
#define DRAMFS_FDTABLE_H

#include <errno.h>
#include "lfs.h"

#ifndef DRAMFS_MAX_FDS
#define DRAMFS_MAX_FDS 20
#endif

void dramfs_init_fdtable(void);

int dramfs_check_fd(int fd);

int dramfs_reserve_fd(void);

int dramfs_free_fd(int fd);

lfs_file_t *dramfs_get_file(int fd);

#endif // DRAMFS_FDTABLE_H
