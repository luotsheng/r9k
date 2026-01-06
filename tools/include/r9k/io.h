/*
-* SPDX-License-Identifier: MIT
 * Copyright (c) 2025 Varketh Nockrath
 */
#ifndef IO_UTILS_H_
#define IO_UTILS_H_

#include <stdio.h>

void *slurp(int fd, size_t *p_size);
ssize_t dump(int fd, const void *data, size_t size);
void *fslurp(FILE *stream, size_t *size);
ssize_t fdump(FILE *stream, const void *data, size_t size);
char *readfile(FILE *stream);
ssize_t writefile(FILE *stream, const void *data, size_t size);
char *readpath(const char *filename);
ssize_t writepath(const char *filename, const void *data, size_t size);

#endif /* IO_UTILS_H_ */
