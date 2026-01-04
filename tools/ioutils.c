/*
-* SPDX-License-Identifier: MIT
 * Copyright (c) 2025 Varketh Nockrath
 */
#include <r9k/ioutils.h>
//std
#include <stdlib.h>
#include <string.h>
#include <errno.h>

char *readall(FILE *stream)
{
        char *buf = NULL;
        char *tmp;
        size_t len = 0;
        size_t cap = 0;
        char chunk[4096];
        size_t n;

        while ((n = fread(chunk, 1, sizeof(chunk), stream)) > 0) {
                if (len + n + 1 > cap) {
                        cap = cap ? cap + (cap >> 1) : sizeof(chunk) * 2;
                        if (cap < len + n + 1)
                                cap = len + n + 1;

                        tmp = realloc(buf, cap);
                        if (!tmp) {
                                if (buf)
                                        free(buf);
                                return NULL;
                        }
                        buf = tmp;
                }
                memcpy(buf + len, chunk, n);
                len += n;
        }

        if (!buf)
                return NULL;

        buf[len] = '\0';

        return buf;
}

char *readfile(const char *path)
{
        char *buf;
        FILE *stream;

        stream = fopen(path, "r");
        if (!stream)
                return NULL;

        buf = readall(stream);
        fclose(stream);

        return buf;
}

char *readin()
{
        return readall(stdin);
}

ssize_t writefile(const char *path, const char *mode, const void *data, size_t size)
{
        if (!path || !mode || !data) {
                errno = EINVAL;
                return -1;
        }

        FILE *stream = fopen(path, mode);
        if (!stream)
                return -1;

        size_t r = fwrite(data, 1, size, stream);

        if (ferror(stream)) {
                fclose(stream);
                return -1;
        }

        fclose(stream);
        return r;
}