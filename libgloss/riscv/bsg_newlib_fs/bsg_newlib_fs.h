#ifndef BSG_NEWLIB_FS_H
#define BSG_NEWLIB_FS_H

#include "lfs.h"

// Toplevel filesystem struct
lfs_t bsg_newlib_fs;

// File system configuration structure
extern struct lfs_config bsg_newlib_fs_cfg;

// File system init routine
int bsg_newlib_fs_init(void);

extern int __bsg_x;
extern int __bsg_y;
extern int __bsg_tiles_Y;

#define bsg_x __bsg_x
#define bsg_y __bsg_y

#endif // BSG_NEWLIB_FS_H
