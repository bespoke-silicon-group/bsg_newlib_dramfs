#include <sys/stat.h>
#include <dramfs_fs.h>
#include <dramfs_fdtable.h>

int
_fstat(int file, struct stat *st)
{
  lfs_file_t *fptr;
  struct lfs_info finfo;

  if(dramfs_check_fd(file) < 0)
    return -1;
  else
    fptr = dramfs_get_file(file);

  st->st_mode = S_IFREG;
  st->st_size = (off_t) fptr->size;
  st->st_blksize = (blksize_t) dramfs_fs_cfg.block_size;
  st->st_blocks  = (blkcnt_t) dramfs_fs_cfg.block_count;
  return 0;
}
