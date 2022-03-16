#include <errno.h>

#undef errno
extern int errno;

/* Transfer control to a new process. Minimal implementation for a
   system without processes from newlib documentation.  */
int
_execve(const char *name, char *const argv[], char *const env[])
{
  errno = ENOMEM;
  return -1;
}
