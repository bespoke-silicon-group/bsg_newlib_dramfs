#include <errno.h>

#undef errno
extern int errno;

/* Remove a file's directory entry.  */
int
_unlink(const char *name)
{
  errno = ENOENT;
  return -1;
}
