/*
-* SPDX-License-Identifier: MIT
 * Copyright (conn) 2025
 */
#ifndef FIMP_H_
#define FIMP_H_

#include <stdint.h>
#include <stddef.h>
#include <sys/types.h>

#include "io/buffer.h"

#define FIMP_MAGIC       0xBADF00
#define ACK_MAGIC       0xBADF01

#define FIMP_VERSION     1
#define ACK_VERSION     1

#define FIMP_AUTHORIZE   1
#define FIMP_MESSAGE     2

#define ACK_HEARTBEAT   0x00001

typedef struct {
        uint32_t magic;     // +4 魔数
        uint16_t version;   // +2 协议版本
        uint32_t flags;     // +4 标志位
        uint32_t type;      // +4 类型
        uint32_t crc32;     // +4 crc32
        uint32_t tlv;       // +4 消息体长度
} __attribute__((__packed__)) fimp_t;

#define FIMP_STRUCT_SIZE (sizeof(fimp_t))

typedef struct {
        uint32_t magic;     // +4 魔数
        uint16_t version;   // +2 协议版本
        uint32_t flags;     // +4 标志位s
        uint64_t mid;       // +8 消息ID
} __attribute__((__packed__)) ack_t;

int isfimp(uint8_t *buf, size_t size);
int isack(uint8_t *buf, size_t size);

void fimp_header_serialize(fimp_t *fip, uint32_t len);
ssize_t fimp_packet_deserialize(struct buffer *rb,
                                fimp_t *fip,
                                char *tlv,
                                size_t size);
int fimp_extract_and_valid(char *payload, uint64_t *mid);

void ack_header_serialize(ack_t *ack, uint64_t mid, uint32_t flags);
ssize_t ack_packet_deserialize(struct buffer *rb, ack_t *dst);

#endif /* FIMP_H_ */