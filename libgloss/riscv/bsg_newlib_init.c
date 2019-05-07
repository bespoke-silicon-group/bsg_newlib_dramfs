#include <fcntl.h>
#include <stdlib.h>
#include "bsg_newlib_init.h"
#include "bsg_newlib_fs.h"
#include "../../../../../bsg_manycore_lib/bsg_manycore.h"

void bsg_newlib_init(void) {
  bsg_set_tile_x_y();

  // Only bottom left tile handles the filesystem
  if((__bsg_x == 0) && (__bsg_y == __bsg_tiles_Y-1)) {
    // Init file system
    if(bsg_newlib_fs_init() < 0) {
      exit(EXIT_FAILURE);
    }

    // Open stdio files
    if(open("stdin", O_RDONLY) != 0) { 
      exit(EXIT_FAILURE);
    }

    if(open("stdout", O_WRONLY) != 1) {
      exit(EXIT_FAILURE);
    }

    if(open("stderr", O_WRONLY) != 2) {
      exit(EXIT_FAILURE);
    }
  }
}
