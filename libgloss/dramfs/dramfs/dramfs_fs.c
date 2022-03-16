#include <unistd.h>
#include <fcntl.h>
#include "lfs.h"
#include "lfs_bd.h"
#include "dramfs_fs.h"
#include "dramfs_fdtable.h"

// File system memory pointer
uint8_t* lfs_ptr = lfs_mem;

// Init routine for Newlib FS
int dramfs_fs_init() {
  // initiate fdtable
  dramfs_init_fdtable();

	// mount the file system
	if(lfs_mount(&dramfs_fs, &dramfs_fs_cfg) < 0) {
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
