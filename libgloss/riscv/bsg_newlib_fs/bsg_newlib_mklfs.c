// Program to initiate littlefs image of given set of files
//
// Bandhav Veluri
// 03-07-2019
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "lfs.h"
#include "lfs_bd.h"

// Main file system struct
lfs_t lfs;

// LFS configuration
struct lfs_config lfs_cfg = {
    // block device operations: lfs_bd.h
    .read  = lfs_read,
    .prog  = lfs_prog,
    .erase = lfs_erase,
    .sync  = lfs_sync,

    // block device default configuration
    .read_size   = 32,
    .prog_size   = 32,
    .block_size  = 0,
    .block_count = 0,
    .lookahead   = 32
};

// Pointer to filesystem memory
// lfs_bd uses this allocation as block device
uint8_t *lfs_ptr;

int main(int argc, char *argv[]) {
	int block_size;
	int block_count;
	FILE *img; // output fs image file handler

	if(argc < 3) {
		printf("Usage: bsg_newlib_mkfs <block_size> <block_count> [input file(s)]\n");
		return -1;
	}

	block_size = atoi(argv[1]);
	block_count = atoi(argv[2]);


  //+------------------------------------------------------
  //| Allocate memory for building Little FS image
  //+------------------------------------------------------

	// memory allocation for lfs
	lfs_ptr = (char *) malloc(block_size*block_count);

	// configure the size
	lfs_cfg.block_size = block_size;
	lfs_cfg.block_count = block_count;

	// format the fs
	if(lfs_format(&lfs, &lfs_cfg) < 0) {
		printf("LFS format error\n");
		return -1;
	}

	// mount the file system
	if(lfs_mount(&lfs, &lfs_cfg) < 0) {
		printf("LFS mount error\n");
		return -1;
	}
  

  //+-------------------------------------------------------
  //| Create stdin, stdout and stderr files in the littlefs
  //+-------------------------------------------------------
    
  lfs_file_t lfs_stdin, lfs_stdout, lfs_stderr;

  // Create stdio files
  lfs_file_open(&lfs, &lfs_stdin, "stdin", LFS_O_WRONLY | LFS_O_CREAT);
  lfs_file_open(&lfs, &lfs_stdout, "stdout", LFS_O_WRONLY | LFS_O_CREAT);
  lfs_file_open(&lfs, &lfs_stderr, "stderr", LFS_O_WRONLY | LFS_O_CREAT);

  // Close stdio files
  lfs_file_close(&lfs, &lfs_stdin);
  lfs_file_close(&lfs, &lfs_stdout);
  lfs_file_close(&lfs, &lfs_stderr);


  //+-------------------------------------------------------
  //| Copy input file to LFS image
  //+-------------------------------------------------------

	for(int i = 3; i < argc; i++) {
		// open input file for reading
		FILE *infile = fopen(argv[i], "r");
		if(infile == NULL) {
			printf("Error opening input file %s\n", argv[i]);
			return -1;
		}

		// create a lfs file for writing
		lfs_file_t lfs_file;
		if(lfs_file_open(&lfs, &lfs_file, argv[i], LFS_O_WRONLY | LFS_O_CREAT) < 0) {
			printf("LFS file open error\n");
			return -1;
		}

		char c = fgetc(infile);
		while (c != EOF) {
			lfs_file_write(&lfs, &lfs_file, &c, 1);
			c = fgetc(infile);
		}

		fclose(infile);
		lfs_file_close(&lfs, &lfs_file);
	}

	// Unmount lfs
	if(lfs_unmount(&lfs) < 0){
		printf("LFS unmounting error\n");
		return -1;
	}


  //+------------------------------------------------------
  //| Print the LFS image as a C array
  //+------------------------------------------------------

	printf("// BSG Newlib File System Initialization\n");
	printf("//\n");
	printf("// Autogenerated by bsg_newlib_mkfs utility\n");
	printf("\n");
  printf("#include <sys/types.h>\n");
	printf("\n");
	printf("#define BSG_NEWLIB_FS_BLOCK_SIZE %d\n", block_size);
	printf("#define BSG_NEWLIB_FS_BLOCK_COUNT %d\n", block_count);
	printf("\n");
	printf("uint8_t lfs_mem[] __attribute__ ((section (\".dram\"))) = {\n");
	
	// dump fs memory to stdout in C array format
	for(int i=0; i < block_size*block_count; i++) {
		printf("0x%0x", lfs_ptr[i]);

		if(i != (block_size*block_count) - 1)
			putchar(',');

		putchar('\n');
	}

	printf("};");

	return 0;
}