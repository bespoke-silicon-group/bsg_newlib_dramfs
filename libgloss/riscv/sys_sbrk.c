// Dynamic allocation in the dram
// TODO: add mutex
extern char _bsg_dram_end_addr[]; /* _bsg_data_end_addr is set in the linker command file */

char *_end = _bsg_dram_end_addr;
char *heap_ptr;

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

  if (!heap_ptr)
    heap_ptr = (char *) _end;
  base = heap_ptr;
  heap_ptr += nbytes;

  return base;
}
