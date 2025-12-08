/*
* SPDX-License-Identifier: MIT
 * Copyright (c) 2025 viakko
 */
#include <r9k/string.h>

int is_str_blank(const char *str)
{
        return !str || !*str || strspn(str, "\t\r\n") == strlen(str);
}