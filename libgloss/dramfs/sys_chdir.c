#include <errno.h>
#include <string.h>
#include "dramfs_fdtable.h"
#include "dramfs_fs.h"

int chdir(const char *path) {

  if(strlen(cwd) + strlen(path) > PATHLENGTH) {
    errno = ENAMETOOLONG;
    return -1;
  }

  char newcwd[PATHLENGTH];
  if(path[0] == '/') {
    strcpy(newcwd, path);
  }
  else {
    strcpy(newcwd, cwd);
    strcat(newcwd, "/");
    strcat(newcwd, path);
  }

  lfs_dir_t dir;
  lfs_entry_t entry;
  const char* pathname = newcwd;
  int err = lfs_dir_find(&dramfs_fs, &dir, &entry, &pathname);

  if(err) {
    errno = ENOENT;
    return -1;
  }

  cwd[0] = '\0';
  char* dirname = strtok(newcwd, "/");
  while(dirname != NULL) {
    if(strcmp(dirname, "..") == 0) {
      for(int i = strlen(cwd)-1; i >= 0; i--) {
        if(cwd[i] == '/') {
          cwd[i] = '\0';
          break;
        }
      }
    }
    else if(strcmp(dirname, ".") != 0) {
      strcat(cwd, "/");
      strcat(cwd, dirname);
    }
    dirname = strtok(NULL, "/");
  }

  if(strlen(cwd) == 0)
    strcpy(cwd, "/");

  return 0;
}
