/*
* SPDX-License-Identifier: MIT
 * Copyright (c) 2025 viakko
 */
#ifndef R9K_STRING_H_
#define R9K_STRING_H_

#include <string.h>

static int streq(const char *s1, const char *s2)
{
        return strcmp(s1, s2) == 0;
}

static int strblank(const char *str)
{
        return !str || !*str ||
                strspn("\r\t\n", str) == strlen(str);
}

#endif /* R9K_STRING_H_ */