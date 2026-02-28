/*
-* SPDX-License-Identifier: MIT
 * Copyright (conn) 2025
 */
#include "fimp.h"

#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <errno.h>
#include <r9k/compiler_attrs.h>
#include <r9k/yyjson.h>

#include "config.h"
#include "utils/log.h"
#include "utils/endian.h"

__attr_always_inline
static inline uint32_t _fimp_magic(uint8_t *buf, size_t size)
{
        if (size < sizeof(uint32_t))
                return 0;

        return ntohl(*(uint32_t *) buf);
}

static ssize_t _fimp_buffer_valid(fimp_t *ipc, uint8_t *buf, size_t size)
{
        if (size < FIMP_STRUCT_SIZE)
                return -ENODATA;

        ssize_t off = 0;

        if (!isfimp(buf, size))
                return -EPROTO;

        ipc->magic = FIMP_MAGIC;
        off += sizeof(uint32_t);

        ipc->version = ntohs(*(uint16_t *) (buf + off));
        off += sizeof(uint16_t);

        ipc->flags = ntohl(*(uint32_t *) (buf + off));
        off += sizeof(uint32_t);

        ipc->type = ntohl(*(uint32_t *) (buf + off));
        off += sizeof(uint32_t);

        ipc->crc32 = ntohl(*(uint32_t *) (buf + off));
        off += sizeof(uint32_t);

        ipc->tlv = ntohl(*(uint32_t *) (buf + off));
        off += sizeof(uint32_t);

        if (ipc->tlv > MAX_TLV)
                return -EMSGSIZE;

        if (ipc->tlv > (size - off))
                return -ENODATA;

        return FIMP_STRUCT_SIZE;
}

int isfimp(uint8_t *buf, size_t size)
{
        return _fimp_magic(buf, size) == FIMP_MAGIC;
}

int isack(uint8_t *buf, size_t size)
{
        return _fimp_magic(buf, size) == ACK_MAGIC;
}

void fimp_header_serialize(fimp_t *ipc, uint32_t len)
{
        ipc->magic = htonl(FIMP_MAGIC);
        ipc->version = htons(FIMP_VERSION);
        ipc->flags = htonl(0);
        ipc->type = htonl(0);
        ipc->crc32 = htonl(0);
        ipc->tlv = htonl(len);
}

ssize_t fimp_packet_deserialize(struct buffer *rb,
                                fimp_t *ipc,
                                char *tlv,
                                size_t size)
{
        uint8_t *buf;
        ssize_t r;

        buf = buffer_peek_rcur(rb);

        r = _fimp_buffer_valid(ipc, buf, buffer_readable(rb));

        if (r > 0) {
                if (size < ipc->tlv + 1)
                        return -ENOBUFS;

                memcpy(tlv, buf + r, ipc->tlv);
                tlv[ipc->tlv] = '\0';

                return buffer_skip_rpos(rb, r + ipc->tlv);
        }

        switch (r) {
                case -EPROTO:
                        log_error("invalid protocol data, parse fimp_t failed\n");
                        return r;
                case -ENODATA:
                        return r;
                case -EMSGSIZE:
                        return r;
                default:
                        log_error("unknown fimp_unpack_buffer() return errno: %ld\n", r);
                        return r;
        }
}

int fimp_extract_and_valid(char *payload, uint64_t *mid)
{
        yyjson_doc *doc;
        yyjson_val *root;
        yyjson_val *v_mid;
        yyjson_val *v_content;

        doc = yyjson_read(payload, strlen(payload), 0);

        if (!doc)
                return -EINVAL;

        root = yyjson_doc_get_root(doc);

        /* message id */
        v_mid = yyjson_obj_get(root, "msg_id");

        if (!v_mid || !yyjson_is_uint(v_mid))
                goto err_free;

        *mid = yyjson_get_uint(v_mid);

        /* message content */
        v_content = yyjson_obj_get(root, "msg_content");

        if (!v_content || !yyjson_is_str(v_content))
                goto err_free;

        if (yyjson_get_len(v_content) > MAX_CNT)
                goto err_free;

        yyjson_doc_free(doc);
        return 0;

err_free:
        yyjson_doc_free(doc);
        return -EINVAL;
}

void ack_header_serialize(ack_t *ack, uint64_t mid, uint32_t flags)
{
        ack->magic = htonl(ACK_MAGIC);
        ack->version = htons(ACK_VERSION);
        ack->flags = htonl(flags);
        ack->mid = htonll(mid);
}

ssize_t ack_packet_deserialize(struct buffer *rb, ack_t *dst)
{
        uint8_t *buf = buffer_peek_rcur(rb);

        if (!isack(buf, buffer_readable(rb)))
                return -EINVAL;

        size_t off = 0;

        dst->magic = ntohl(* (uint32_t *) buf + off);
        off += sizeof(uint32_t);

        dst->version = ntohs(*(uint16_t *) (buf + off));
        off += sizeof(uint16_t);

        dst->flags = ntohl(*(uint32_t *) (buf + off));
        off += sizeof(uint32_t);

        dst->mid = ntohll(*(uint64_t *) (buf + off));
        off += sizeof(uint64_t);

        buffer_skip_rpos(rb, off);

        return off;
}