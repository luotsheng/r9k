/*
* SPDX-License-Identifier: MIT
 * Copyright (c) 2025 viakko
 */
#ifndef TYPEDEFS_H_
#define TYPEDEFS_H_

#include <stdbool.h>

#define die(fmt, ...)                           \
do {                                            \
        fprintf(stderr, fmt, ##__VA_ARGS__);    \
        exit(EXIT_FAILURE);                     \
} while (0)

#endif /* TYPEDEFS_H_ */