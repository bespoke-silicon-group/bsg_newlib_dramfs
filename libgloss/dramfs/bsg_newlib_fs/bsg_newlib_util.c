// BSG Newlib utility functions
//
// Bandhav Veluri
// 08/15/2019

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include "bsg_newlib_util.h"

int is_file(const char* path) {
	struct stat s;
	stat(path, &s);
	return S_ISREG(s.st_mode);
}

int is_dir(const char* path) {
	struct stat s;
	stat(path, &s);
	return S_ISDIR(s.st_mode);
}

int cp_file_to_lfs(const char* path, lfs_t* lfs) {
	if(!is_file(path)) {
		return -1;
	}

  // open input file for reading
  FILE *infile = fopen(path, "r");
  if(infile == NULL) {
  	fprintf(stderr, "Error opening input file %s\n", path);
  	return -1;
  }

  // create a lfs file for writing
  lfs_file_t lfs_file;
  if(lfs_file_open(lfs, &lfs_file, path, LFS_O_WRONLY | LFS_O_CREAT) < 0) {
  	fprintf(stderr, "LFS file open error %s\n", path);
  	return -1;
  }

  int c = fgetc(infile);
  while (c != EOF) {
  	lfs_file_write(lfs, &lfs_file, &c, 1);
  	c = fgetc(infile);
  }

  fclose(infile);
  lfs_file_close(lfs, &lfs_file);

	return 0;
}

int lfs_cp(const char* path, lfs_t* lfs) {
	if(is_file(path)) {
		return cp_file_to_lfs(path, lfs);
	} else if(is_dir(path)) {
		DIR* dir;
		struct dirent* ent;

		if(dir = opendir(path)) {
			if(lfs_mkdir(lfs, path) < 0) {
				fprintf(stderr, "LFS COPY: Error creating %s in lfs\n", path);
				return -1;
			}

			while(ent = readdir(dir)) {
				// Build the full path of file or dir
				char sep[] = "/";
				char* ent_full_path = (char*) malloc(strlen(path)+strlen(sep)+strlen(ent->d_name)+1);
				strcpy(ent_full_path, path);
				strcat(ent_full_path, sep);
				strcat(ent_full_path, ent->d_name);

				if((strcmp(ent->d_name, ".") == 0) | (strcmp(ent->d_name, "..")==0)) {
					continue;
				} else {
					if(ent->d_type == DT_REG) {
						if(cp_file_to_lfs(ent_full_path, lfs) < 0) 
							return -1;
					} else if(ent->d_type == DT_DIR) {
						// Recursively copy the dir
						if(lfs_cp(ent_full_path, lfs) < 0)
							return -1;
					}
				}
			}

			return 0;
		} else {
			fprintf(stderr, "LFS COPY: Error opening directory %s\n", path);
			return -1;
		}
	}

	fprintf(stderr, "LFS COPY: argument should be a file or directory\n");
	return -1;
}
