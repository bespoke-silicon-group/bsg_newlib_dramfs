#include <unistd.h>
#include <fcntl.h>
#include "lfs.h"
#include "lfs_bd.h"
#include "bsg_newlib_fs.h"
#include "bsg_newlib_fdtable.h"

lfs_t bsg_newlib_fs;

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

  // Open stdio files
  if(open("stdin", O_RDONLY) != 0) { 
    return -1;
  }

  if(open("stdout", O_WRONLY) != 1) {
    return -1;
  }

  if(open("stderr", O_WRONLY) != 2) {
    return -1;
  }

	return 0;
}
