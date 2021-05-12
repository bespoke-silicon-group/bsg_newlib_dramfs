#include <errno.h>
#include <string.h>
#include "dramfs_fdtable.h"
#include "dramfs_fs.h"

char* getcwd(char* buf, size_t size) {
  if(strlen(cwd) > size) {
    errno = ERANGE;
    return NULL;
  }

  strcpy(buf, path);
  return buf;
}
