#include <sys/stat.h>
#include <bsg_newlib_fs.h>
#include <bsg_newlib_fdtable.h>

int
_fstat(int file, struct stat *st)
{
  lfs_file_t *fptr;
  struct lfs_info finfo;

  if(bsg_newlib_check_fd(file) < 0)
    return -1;
  else
    fptr = bsg_newlib_get_file(file);

  st->st_mode = S_IFREG;
  st->st_size = (off_t) fptr->size;
  st->st_blksize = (blksize_t) bsg_newlib_fs_cfg.block_size;
  st->st_blocks  = (blkcnt_t) bsg_newlib_fs_cfg.block_count;
  return 0;
}
