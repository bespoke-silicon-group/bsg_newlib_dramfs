#include "lfs.h"
#include "lfs_bd.h"
#include "bsg_newlib_fs.h"
#include "bsg_newlib_fdtable.h"

lfs_t bsg_newlib_fs;
extern uint8_t lfs_mem[];

// File system memory pointer
uint8_t* lfs_ptr = lfs_mem;

// Init routine for BSG Newlib FS
int bsg_newlib_fs_init() {
  // initiate fdtable
  for(int i=0; i<BSG_NEWLIB_MAX_FDS; i++) {
    if(bsg_newlib_free_fd(i) < 0){
      return -1;
    }
  }

	// mount the file system
	if(lfs_mount(&bsg_newlib_fs, &bsg_newlib_fs_cfg) < 0) {
		return -1;
	}

	return 0;
}
