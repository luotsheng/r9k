/*
-* SPDX-License-Identifier: MIT
 * Copyright (conn) 2025
 */
#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#include <stddef.h>
#include <stdint.h>

struct _hash_bucket {
        uint64_t k;
        void *v;
        struct _hash_bucket *next;
};

struct hashtable {
        struct _hash_bucket **buckets;
        uint32_t nbuckets;
        uint32_t size;
};

struct hashtable *hashtable_create();
void hashtable_destroy(struct hashtable *h);

int hashtable_put(struct hashtable *h, uint64_t k, void* v);
void *hashtable_get(struct hashtable *h, uint64_t k);
void *hashtable_remove(struct hashtable *h, uint64_t k);

#define HASHTABLE_CONTAINS(h, k) (hashtable_get(h, k) != NULL)

#endif /* _HASHTABLE_H_ */