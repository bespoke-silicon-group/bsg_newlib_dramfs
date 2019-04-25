#ifndef BSG_NEWLIB_INIT
#define BSG_NEWLIB_INIT

// Defined in bsg_manycore_lib
extern void bsg_set_tile_x_y(void);
extern int __bsg_x;
extern int __bsg_y;
extern int __bsg_tiles_Y;

int bsg_newlib_init();

#endif // BSG_NEWLIB_INIT
