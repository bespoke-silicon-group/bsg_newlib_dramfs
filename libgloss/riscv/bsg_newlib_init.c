#include <stdio.h>
#include "bsg_newlib_init.h"
#include "bsg_newlib_fs.h"

int bsg_newlib_init(void) {
  bsg_set_tile_x_y();

  // Only bottom left tile handles the filesystem
  if((__bsg_x == 0) && (__bsg_y == __bsg_tiles_Y-1)) {
    if(bsg_newlib_fs_init() < 0) {
      return -1;
    }

    // STDIO Files
    stdin = fopen("stdin", "r");
    stdout = fopen("stdout", "w");
    stderr = fopen("stderr", "w");
    
    if(stdin == NULL || stdout == NULL || stderr == NULL) {
      return -1;
    }
  }

  return 0;
}
