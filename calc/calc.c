/*
 * SPDX-License-Identifier: MIT
 * Copyright (c) 2025
 */
#include <stdio.h>

int main(int argc, char* argv[])
{
        FILE *fp;

        if (argc != 2)
                return 1;

        fp = popen("bc -l", "w");
        if (!fp)
                return 1;

        fprintf(fp, "%s\n", argv[1]);
        pclose(fp);

        return 0;
}