#include "lfs.h"
#include "lfs_bd.h"
#include "bsg_newlib_fs.h"
#include "bsg_newlib_fdtable.h"
#include "../../../../../../bsg_manycore_lib/bsg_manycore.h"

lfs_t bsg_newlib_fs;
extern uint8_t lfs_mem[];

// File system memory pointer
uint8_t* lfs_ptr = lfs_mem;

// Init routine for BSG Newlib FS
int bsg_newlib_fs_init() {
  // initiate fdtable
  bsg_newlib_init_fdtable();

	// mount the file system
	if(lfs_mount(&bsg_newlib_fs, &bsg_newlib_fs_cfg) < 0) {
		return -1;
	}

	return 0;
}
