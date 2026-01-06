/*
-* SPDX-License-Identifier: MIT
 * Copyright (c) 2025 Varketh Nockrath
 */
#include <r9k/io.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#define N_TRACE
#include <r9k/trace.h>

#define BUFFSIZE 8192

void *slurp(int fd, size_t *p_size)
{
        if (fd < 0)
                return NULL;

        uint8_t *buf = NULL;
        uint8_t *tmp = NULL;
        size_t cap = 0;
        size_t len = 0;
        ssize_t n = 0;

        cap = BUFFSIZE;
        buf = (uint8_t *) malloc(cap);
        if (!buf) {
                errno = ENOMEM;
                return NULL;
        }

        while (1) {
                if (len >= cap) {
                        cap = cap * 2;

                        if (cap <= len || cap > SIZE_MAX / 2) {
                                free(buf);
                                errno = ENOMEM;
                                return NULL;
                        }

                        tmp = realloc(buf, cap);
                        if (!tmp) {
                                free(buf);
                                errno = ENOMEM;
                                return NULL;
                        }

                        buf = tmp;
                }

                n = read(fd, buf + len, cap - len);

                if (n > 0) {
                        len += n;
                        continue;
                }

                if (n == 0)
                        break;

                if (n == -1 && errno == EINTR)
                        continue;

                free(buf);
                return NULL;
        }

        tmp = realloc(buf, len);
        if (tmp)
                buf = tmp;

        if (p_size)
                *p_size = len;

        return buf;
}

ssize_t dump(int fd, const void *data, size_t size)
{
        if (fd < 0 || !data) {
                errno = EINVAL;
                return -1;
        }

        if (size == 0)
                return 0;

        ssize_t n;
        size_t written = 0;

        while (written < size) {
                n = write(fd, (const uint8_t *) data + written, size - written);

                if (n > 0) {
                        written += n;
                        continue;
                }

                if (n == -1 && errno == EINTR)
                        continue;

                return -1;
        }

        return written;
}

void *fslurp(FILE *stream, size_t *size)
{
        if (!stream) {
                errno = EINVAL;
                return NULL;
        }

        return slurp(fileno(stream), size);
}

ssize_t fdump(FILE *stream, const void *data, size_t size)
{
        if (!stream || !data) {
                errno = EINVAL;
                return -1;
        }

        return dump(fileno(stream), data, size);
}

char *readfile(FILE *stream)
{
        if (!stream) {
                errno = EINVAL;
                return NULL;
        }

        char *buf;
        size_t size;

        buf = fslurp(stream, &size);
        if (!buf)
                return NULL;

        char *tmp = realloc(buf, size + 1);
        if (!tmp) {
                free(buf);
                return NULL;
        }

        buf = tmp;
        buf[size] = '\0';

        return buf;
}

ssize_t writefile(FILE *stream, const void *data, size_t size)
{
        if (!stream || !data) {
                errno = EINVAL;
                return -1;
        }

        if (size == 0)
                return 0;

        return fdump(stream, data, size);
}

char *readpath(const char *filename)
{
        FILE *stream = fopen(filename, "rb");
        if (!stream)
                return NULL;

        char *buf = readfile(stream);
        fclose(stream);

        return buf;
}

ssize_t writepath(const char *filename, const void *data, size_t size)
{
        if (!data) {
                errno = EINVAL;
                return -1;
        }

        if (size == 0)
                return 0;

        FILE *stream = fopen(filename, "wb");
        if (!stream)
                return -1;

        ssize_t r = writefile(stream, data, size);
        fclose(stream);

        return r;
}

ssize_t readall(int fd, void *buf, size_t size)
{
        if (fd < 0 || !buf) {
                errno = EINVAL;
                return -1;
        }

        size_t n = 0;

        while (n < size) {
                ssize_t r = read(fd, (uint8_t *) buf + n, size - n);

                if (r > 0) {
                        n += r;
                        continue;
                }

                if (r == 0)
                        break;

                if (r == -1 && errno == EINTR)
                        continue;

                return r;
        }

        return n;
}

ssize_t writeall(int fd, const void *buf, size_t size)
{
        if (fd < 0 || !buf) {
                errno = EINVAL;
                return -1;
        }

        size_t n = 0;

        while (n < size) {
                ssize_t r = write(fd, (uint8_t *) buf + n, size - n);

                if (r > 0) {
                        n += r;
                        continue;
                }

                if (r == -1 && errno == EINTR)
                        continue;

                return r;
        }

        return n;
}