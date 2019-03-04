#ifndef BSG_NEWLIB_FS_H
#define BSG_NEWLIB_FS_H

#include "lfs.h"

// Toplevel filesystem struct
// File system must be mounted to this struct by crt before calling main
extern lfs_t bsg_newlib_fs;

// File system configuration structure
// Defined by the library based on above macros
extern struct lfs_config bsg_newlib_fs_cfg;

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

// File system size parameters
#ifndef BSG_NEWLIB_FS_BLOCK_SIZE
#define BSG_NEWLIB_FS_BLOCK_SIZE 128
#endif
#ifndef BSG_NEWLIB_FS_BLOCK_COUNT
#define BSG_NEWLIB_FS_BLOCK_COUNT 16
#endif

#endif // BSG_NEWLIB_FS_H
