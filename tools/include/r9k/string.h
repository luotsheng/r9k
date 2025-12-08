/*
* SPDX-License-Identifier: MIT
 * Copyright (c) 2025 viakko
 */
#ifndef R9K_STRING_H_
#define R9K_STRING_H_

#include <string.h>

int __is_str_blank(const char *str);

#define streq(a, b) (strcmp(a, b) == 0)
#define strne(a, b) (!streq(a, b))
#define isblank(str) (__is_str_blank(str))

#endif /* R9K_STRING_H_ */