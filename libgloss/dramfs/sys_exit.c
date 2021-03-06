#include <unistd.h>
#include "dramfs_fs.h"

/* Exit a program without cleaning up files.  */
void
_exit(int exit_status) {
  // close stdio
  close(0);
  close(1);
  close(2);

  dramfs_exit(exit_status);
  while(1);
}
