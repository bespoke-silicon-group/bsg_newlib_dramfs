#include "lfs.h"

// Check if a path is a regular file
int is_file(const char*);

// Check if a path is a directory
int is_dir(const char*);

// Copy a file to lfs struct
int cp_file_to_lfs(const char*, lfs_t*);

// Copy a file/directory recursively to lfs struct
int lfs_cp(const char*, lfs_t*);
