#include <sys/types.h>
#include <bsg_newlib_fs.h>
#include <bsg_newlib_fdtable.h>

/* Set position in a file.  */
off_t
_lseek(int file, off_t ptr, int dir)
{
  lfs_file_t *fptr;

  if(bsg_newlib_check_fd(file) < 0)
    return -1;
  else
    fptr = bsg_newlib_get_file(file);

  return lfs_file_seek(&bsg_newlib_fs, fptr, ptr, dir);
}
