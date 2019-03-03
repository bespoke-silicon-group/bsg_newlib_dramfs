#include "bsg_newlib_fs.h"
#include "lfs.h"
#include "lfs_bd.h"

// Data mem allocation for FS
uint8_t lfs_mem[BSG_NEWLIB_FS_BLOCK_SIZE*BSG_NEWLIB_FS_BLOCK_COUNT] __attribute__ ((section (".dram")));

// File system memory pointer
uint8_t *lfs_ptr = lfs_mem;

// LFS static buffers: these buffer reside in the local dmem
// and act as caches for lfs operations.
static uint8_t lfs_read_buffer[BSG_NEWLIB_FS_READ_SIZE];
static uint8_t lfs_prog_buffer[BSG_NEWLIB_FS_PROG_SIZE];
static uint8_t lfs_lookahead_buffer[BSG_NEWLIB_FS_LOOKAHEAD/8];
static uint8_t lfs_file_buffer[BSG_NEWLIB_FS_PROG_SIZE];

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
    .lookahead   = BSG_NEWLIB_FS_LOOKAHEAD,

    // buffers for fs operations
    .read_buffer      = lfs_read_buffer,
    .prog_buffer      = lfs_prog_buffer,
    .lookahead_buffer = lfs_lookahead_buffer,
    .file_buffer      = lfs_file_buffer
};

// Static buffer for lfs file open configuration
static uint8_t lfs_file_cfg_buf[BSG_NEWLIB_FS_PROG_SIZE];

struct lfs_file_config bsg_newlib_file_cfg = {
  .buffer = lfs_file_cfg_buf
};
