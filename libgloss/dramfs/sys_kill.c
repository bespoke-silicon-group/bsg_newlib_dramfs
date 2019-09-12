#include <errno.h>

#undef errno
extern int errno;

/* Send a signal. Minimal implementation for a system without processes
   just causes an error.  */
int
_kill(int pid, int sig)
{
  errno = EINVAL;
  return -1;
}
