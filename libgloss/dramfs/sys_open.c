/*
 * Copyright (c) 2023, University of Washington
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the University of Washington nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL UNIVERSITY OF WASHINGTON BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <errno.h>
#include <fcntl.h>
#include "dramfs_fdtable.h"
#include "dramfs_fs.h"

/* Open a file.  */
int
_open(const char *name, int flags, int mode)
{
  int fd = dramfs_reserve_fd();
  int lfs_flags = 0;

  // File open flags mapping
  lfs_flags |= (flags & O_RDONLY  ) ? LFS_O_RDONLY   : 0;
  lfs_flags |= (flags & O_WRONLY  ) ? LFS_O_WRONLY   : 0;
  lfs_flags |= (flags & O_RDWR    ) ? LFS_O_RDWR     : 0;
  lfs_flags |= (flags & O_APPEND  ) ? LFS_O_APPEND   : 0;
  lfs_flags |= (flags & O_CREAT   ) ? LFS_O_CREAT    : 0;
  lfs_flags |= (flags & O_TRUNC   ) ? LFS_O_TRUNC    : 0;
  lfs_flags |= (flags & O_EXCL    ) ? LFS_O_EXCL     : 0;
  lfs_flags |= (flags & O_NONBLOCK) ? LFS_O_NONBLOCK : 0;

  if(fd < 0) {
    return -1;
  } else {
    lfs_file_t *fptr = dramfs_get_file(fd);

    int ret = lfs_file_open(&dramfs_fs, fptr, name, lfs_flags);

    if(ret < 0) {
      errno = ret;
      return -1;
    } else {
      return fd;
    }
  }
}
