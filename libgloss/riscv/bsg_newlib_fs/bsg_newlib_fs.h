#ifndef BSG_NEWLIB_FS_H
#define BSG_NEWLIB_FS_H

#include "littlefs/lfs/lfs.h"

// Following macros must be defined to specify the filesystem parameters. Can be
// defined as compile time options
#ifndef BSG_NEWLIB_FS_PTR
#error "File system address must be provided: define the macro BSG_NEWLIB_FS_PTR"
#endif
#ifndef BSG_NEWLIB_FS_BLOCK_SIZE
#error "File system block size must be provided: define the macro BSG_NEWLIB_FS_BLOCK_SIZE"
#endif
#ifndef BSG_NEWLIB_FS_BLOCK_COUNT
#error "File system block count must be provided: define the macro BSG_NEWLIB_FS_BLOCK_COUNT"
#endif

// These macros define cache sizes for file system operations.
// Can be tweaked to improve file system performace
#ifndef BSG_NEWLIB_FS_READ_SIZE
#define BSG_NEWLIB_FS_READ_SIZE 32
#endif
#ifndef BSG_NEWLIB_FS_PROG_SIZE
#define BSG_NEWLIB_FS_PROG_SIZE 32
#endif
#ifndef BSG_NEWLIB_FS_LOOKAHEAD
#define BSG_NEWLIB_FS_LOOKAHEAD 32
#endif

// Toplevel filesystem struct
// File system must be mounted to this struct before calling main
extern lfs_t bsg_newlib_lfs;

// File system configuration structure
// Defined by the library based on above macros
extern const struct lfs_config bsg_newlib_lfs_cfg;

#endif // BSG_NEWLIB_FS_H
