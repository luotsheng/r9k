/*
-* SPDX-License-Identifier: MIT
 * Copyright (c) 2025 Varketh Nockrath
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <r9k/readline.h>

#include "io/socket.h"
#include "config.h"
#include "utils/log.h"
#include "ipc.h"

void client_start()
{
        int fd;
        ipc_t ipc;
        char body[4096];
        ssize_t r;

        fd = tcp_connect("127.0.0.1", PORT);

        if (fd < 0) {
                log_error("connect to 127.0.0.1 %d failed, cause: %s\n", PORT, syserr);
                exit(1);
        }

        char prompt[64];
        char *line = NULL;

        while (1) {
                snprintf(prompt, sizeof(prompt), "%d > ", fd);

                line = readline(prompt);

                if (!line)
                        continue;

                snprintf(body, sizeof(body), "{\"msg_id\":1023890128390189321,\"msg_at\":[10012173,20038192,30041002],\"from\":10051182,\"to\":\"G100239301932\",\"is_reply\":true,\"reply_to_msg_id\":1023890128390189320,\"timestamp\":1770345129543,\"msg_content\":\"%s\"}", line);
                uint32_t len = strlen(body);
                log_info("body len: %u\n", len);
                ipc_header_build(&ipc, len);

reconnect:
                if (fd < 0) {
                        fd = tcp_connect("127.0.0.1", PORT);
                        log_info("reconnect fd: %d\n", fd);
                }

                r = send(fd, &ipc, sizeof(ipc_t), MSG_NOSIGNAL);

                if (r < 0) {
                        close(fd);
                        fd = -1;
                        goto reconnect;
                }

                r = send(fd, body, len, MSG_NOSIGNAL);

                if (r < 0) {
                        close(fd);
                        fd = -1;
                        goto reconnect;
                }
        }

}