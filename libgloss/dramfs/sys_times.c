#include <sys/times.h>

/* Timing info for current process. Minimal implementation */
clock_t
_times(struct tms *buf)
{
  return -1;
}
