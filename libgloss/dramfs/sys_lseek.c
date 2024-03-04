#include <sys/types.h>
#include <dramfs_fs.h>
#include <dramfs_fdtable.h>
#include "systrace.h"

/* Set position in a file.  */
off_t
_lseek(int file, off_t ptr, int dir)
{
  sys_tick(SYS_lseek);
  lfs_file_t *fptr;

  if(dramfs_check_fd(file) < 0)
    return -1;
  else
    fptr = dramfs_get_file(file);

  off_t ret = lfs_file_seek(&dramfs_fs, fptr, ptr, dir);
  sys_tock();

  return ret;
}
