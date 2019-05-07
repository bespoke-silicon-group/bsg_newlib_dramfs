// Dynamic allocation in the dram
// TODO: add mutex
#include "../../../../../bsg_manycore_lib/bsg_manycore.h"
extern char _bsg_dram_end_addr[]; /* _bsg_data_end_addr is set in the linker command file */

char *heap_ptr = _bsg_dram_end_addr;

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

  //bsg_printf("sbrk base = %x\n", base);
  return base;
}
