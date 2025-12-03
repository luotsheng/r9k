/*
* SPDX-License-Identifier: MIT
 * Copyright (c) 2025 viakko
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <argparser.h>

#define NS_VERSION "1.0"

static int on_dns(struct argparser *ap, struct option *opt)
{
        char buf[4096];
        FILE *fp;

        fp = fopen("/etc/resolv.conf", "r");
        if (!fp) {
                perror("cannot open /etc/resolv.conf");
                exit(1);
        }

        while (fgets(buf, sizeof(buf), fp))
                fputs(buf, stdout);

        fclose(fp);
        exit(0);
}

int main(int argc, char **argv)
{
        struct argparser *ap;
        struct option *help, *version;
        struct option *dns;

        ap = argparser_create("ns", NS_VERSION);
        if (!ap)
                return -1;

        argparser_add0(ap, &help, "h", "help", "show this help message and exit", __acb_help, opt_none);
        argparser_add0(ap, &version, "version", NULL, "show version", __acb_version, opt_none);
        argparser_add1(ap, &dns, "dns", NULL, "show resolv DNS address and exit", on_dns, opt_none);

        if (argparser_run(ap, argc, argv) != 0) {
                fprintf(stderr, "%s\n", argparser_error(ap));
                argparser_free(ap);
                return -1;
        }

        return 0;
}