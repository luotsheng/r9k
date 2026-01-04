/*
-* SPDX-License-Identifier: MIT
 * Copyright (c) 2025 Varketh Nockrath
 */
#ifndef IO_UTILS_H_
#define IO_UTILS_H_

#include <stdio.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-function"

char *readall(FILE *fp);
char *readfile(const char *path);
char *readin(void);
ssize_t writefile(const char *path, const char *mode, const void *data, size_t size);

#pragma GCC diagnostic pop
#endif /* IO_UTILS_H_ */