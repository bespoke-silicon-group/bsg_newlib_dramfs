#include "bsg_newlib_fs.h"
#include "lfs.h"
#include "lfs_bd.h"

// Data mem allocation for FS
uint8_t lfs_mem[BSG_NEWLIB_FS_BLOCK_SIZE*BSG_NEWLIB_FS_BLOCK_COUNT] __attribute__ ((section (".dram")));

// File system memory pointer
uint8_t *lfs_ptr = lfs_mem;

// LFS configuration
struct lfs_config bsg_newlib_fs_cfg = {
    // block device operations
    .read  = lfs_read,
    .prog  = lfs_prog,
    .erase = lfs_erase,
    .sync  = lfs_sync,

    // block device configuration
    .read_size   = BSG_NEWLIB_FS_READ_SIZE,
    .prog_size   = BSG_NEWLIB_FS_PROG_SIZE,
    .block_size  = BSG_NEWLIB_FS_BLOCK_SIZE,
    .block_count = BSG_NEWLIB_FS_BLOCK_COUNT,
    .lookahead   = BSG_NEWLIB_FS_LOOKAHEAD
};
