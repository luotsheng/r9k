/*
-* SPDX-License-Identifier: MIT
 * Copyright (c) 2025 viakko
 */
#include <r9k/utils.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void die(const char *fmt, ...)
{
        va_list va;
        va_start(va, fmt);
        vfprintf(stderr, fmt, va);
        exit(EXIT_FAILURE);
        va_end(va);
}