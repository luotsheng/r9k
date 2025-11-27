/*
* SPDX-License-Identifier: MIT
 * Copyright (c) 2025 viakko
 *
 * Network debugger
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <argparser.h>
#include <fx/typedefs.h>

/* --flushdns */
static int flush_dns(void)
{
        int r;

#ifdef _WIN32
        r = system("ipconfig /flushdns");
#elif __linux__
        r = system("sudo systemd-resolve --flush-caches");
#elif __APPLE__
        r = system("sudo dscacheutil -flushcache; sudo killall -HUP mDNSResponder 2>/dev/null");
        if (r != 0)
                r = system("sudo dscacheutil -flushcache; sudo killall -HUP mDNSResponder 2>/dev/null");
        if (r != 0)
                r = system("sudo killall -HUP mDNSResponder 2>/dev/null");
#endif
        if (r == 0)
                printf("DNS cache flushed successfully.\n");
        else
                printf("Failed to flush DNS cache.\n");

        return r;
}

static int show_dns()
{
        struct __res_state res;

        if (res_ninit(&res) != 0) {
                perror("res_ninit failed");
                return 1;
        }

        for (int i = 0; i < res.nscount; i++)
                printf("%s\n", inet_ntoa(res.nsaddr_list[i].sin_addr));

        res_nclose(&res);

        return 0;
}

static int show_resolv_conf()
{
        char buf[4096];
        FILE *fp;

        fp = fopen("/etc/resolv.conf", "r");
        if (!fp) {
                perror("cannot open /etc/resolv.conf");
                return 1;
        }

        while (fgets(buf, sizeof(buf), fp))
                fputs(buf, stdout);

        fclose(fp);

        return 0;
}

int main(int argc, char **argv)
{
        struct argparser *ap;
        struct option *opt_dns;
        struct option *opt_resolv;
        struct option *opt_flush_dns;

        ap = argparser_create();
        if (!ap)
                return -1;

        argparser_add1(ap, &opt_dns, "dns", NULL, "show resolv DNS address and exit", OP_NULL);
        argparser_add0(ap, &opt_resolv, "resolv", NULL, "show resolv.conf and exit", OP_NULL);
        argparser_add0(ap, &opt_flush_dns, NULL, "flushdns", "flush system DNS cache and exit", OP_NULL);

        if (argparser_run(ap, argc, argv) != 0) {
                fprintf(stderr, "%s\n", argparser_error(ap));
                argparser_free(ap);
                return -1;
        }

        if (opt_dns)
                return show_dns();

        if (opt_resolv)
                return show_resolv_conf();

        if (opt_flush_dns)
                return flush_dns();

        return 0;
}