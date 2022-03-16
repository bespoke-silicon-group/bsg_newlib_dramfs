// Dynamic allocation in the dram
// TODO: add mutex
extern char _end[]; /* _end is set in the linker command file */

char *heap_ptr = _end;

/*
 * sbrk -- changes heap size size. Get nbytes more
 *         RAM. We just increment a pointer in what's
 *         left of memory on the board.
 */
char *
_sbrk (nbytes)
     int nbytes;
{
  char        *base;
  
  base = heap_ptr;
  heap_ptr += nbytes;

  return base;
}
