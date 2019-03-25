#include "lfs.h"
#include "lfs_bd.h"
#include "bsg_newlib_fs.h"
#include "bsg_newlib_fdtable.h"

// Data mem allocation for FS
uint8_t lfs_mem[BSG_NEWLIB_FS_BLOCK_SIZE*BSG_NEWLIB_FS_BLOCK_COUNT] __attribute__ ((section (".dram")));

// File system memory pointer
uint8_t *lfs_ptr = lfs_mem;

// lfs_t bsg_newlib_fs;

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

// Init routine for BSG Newlib FS
int bsg_newlib_fs_init() {
  // initite fdtable
  for(int i=0; i<BSG_NEWLIB_MAX_FDS; i++) {
    if(bsg_newlib_free_fd(i) < 0){
      return -1;
    }
  }

	// format the file system
	if(lfs_format(&bsg_newlib_fs, &bsg_newlib_fs_cfg) < 0) {
		return -1;
	}

	// mount the file system
	if(lfs_mount(&bsg_newlib_fs, &bsg_newlib_fs_cfg) < 0) {
		return -1;
	}

	return 0;
}
