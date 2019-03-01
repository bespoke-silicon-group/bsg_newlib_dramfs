#include "littlefs/lfs/fs.h"
#include "littlefs/bd/lfs_bd.h"

// Global pointer to the file system in DRAM
// uint8_t *lfs_ptr = BSG_NEWLIB_FS_PTR;

// LFS static buffers: these buffer reside in the local dmem
// and act as caches for lfs operations.
static uint8_t lfs_read_buffer[BSG_NEWLIB_FS_READ_SIZE];
static uint8_t lfs_prog_buffer[BSG_NEWLIB_FS_PROG_SIZE];
static uint8_t lfs_lookahead_buffer[BSG_NEWLIB_FS_LOOKAHEAD/8];
static uint8_t lfs_file_buffer[BSG_NEWLIB_FS_PROG_SIZE];

// LFS configuration
const struct lfs_config bsg_newlib_lfs_cfg = {
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
    .lookahead   = BSG_NEWLIB_FS_LOOKAHEAD,

    // buffers for fs operations
    .read_buffer      = lfs_read_buffer,
    .prog_buffer      = lfs_prog_buffer,
    .lookahead_buffer = lfs_lookahead_buffer,
    .file_buffer      = lfs_file_buffer
};
