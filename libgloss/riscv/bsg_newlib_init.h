#ifndef BSG_NEWLIB_INIT
#define BSG_NEWLIB_INIT

// Defined in bsg_manycore_lib
extern void bsg_set_tile_x_y(void);
extern int __bsg_x;
extern int __bsg_y;
extern int __bsg_tiles_Y;

void bsg_newlib_init(void);

#endif // BSG_NEWLIB_INIT
