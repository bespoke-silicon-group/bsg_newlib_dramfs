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
#include <sys/types.h>
#include "dramfs_fdtable.h"
#include "dramfs_fs.h"

/* Read from a file.  */
ssize_t _read(int fd, void *ptr, size_t len)
{
  if(dramfs_check_fd(fd) < 0) {
    return -1;
  }

  if(fd == 0) {
    uint8_t *data = (uint8_t *)ptr;

    // Return early on len == 0
    if (len == 0) return (ssize_t) 0;

    int ch;
    if (dramfs_nonblock_fd(fd) == 0) {
      // Block to read just 1 character to start
      while ((ch = dramfs_getchar()) == -1);
    } else {
      // Read the first character, and return immediately if it's EOF
      if ((ch = dramfs_getchar()) == -1) return (ssize_t) 0;
    }

    // Keep reading until new
    int i = 0;
    do {
      data[i++] = ch;
      if (i == len) break;
    } while ((ch = dramfs_getchar()) != -1);

    return (ssize_t) i;
  }

  lfs_file_t *fptr = dramfs_get_file(fd);
  return (ssize_t) lfs_file_read(&dramfs_fs, fptr, ptr, (lfs_size_t) len);
}
