// Copyright (c) 2015-2016 Nuxi, https://nuxi.nl/
//
// SPDX-License-Identifier: BSD-2-Clause

#include <sys/stat.h>

#include <wasi/core.h>
#include <errno.h>

#include "stat_impl.h"

int fstat(int fildes, struct stat *buf) {
  __wasi_filestat_t internal_stat;
#ifdef __wasilibc_unmodified_upstream // fstat
  __wasi_errno_t error = __wasi_file_stat_fget(fildes, &internal_stat);
#else
  __wasi_errno_t error = __wasi_fd_filestat_get(fildes, &internal_stat);
#endif
  if (error != 0) {
    errno = error;
    return -1;
  }
  to_public_stat(&internal_stat, buf);
  return 0;
}
