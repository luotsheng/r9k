/*
* SPDX-License-Identifier: MIT
 * Copyright (c) 2025 viakko
 *
 * Network debugger
 */
#include <stdio.h>
#include <argparser.h>

int main(int argc, char **argv)
{
        struct argparser *ap;

        ap = argparser_create();
        if (!ap)
                return -1;

        if (argparser_run(ap, argc, argv) != 0) {
                fprintf(stderr, "%s\n", argparser_error(ap));
                argparser_free(ap);
                return -1;
        }

        return 0;
}
