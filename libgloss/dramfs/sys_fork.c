#include <errno.h>

#undef errno
extern int errno;

/* Create a new process. Minimal implementation for a system without
   processes from newlib documentation.  */
int _fork()
{
  errno = EAGAIN;
  return -1;
}
