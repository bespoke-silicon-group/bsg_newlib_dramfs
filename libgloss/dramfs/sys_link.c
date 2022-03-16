#include <errno.h>

#undef errno
extern int errno;

/* Establish a new name for an existing file. Minimal implementation */
int _link(const char *old_name, const char *new_name)
{
  errno = EMLINK;
  return -1;
}
