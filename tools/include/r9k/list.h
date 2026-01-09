/*
* SPDX-License-Identifier: MIT
 * Copyright (c) 2025
 */
#ifndef LIST_H_
#define LIST_H_

#include <stdlib.h>

struct list;

typedef void (*list_free_func_t) (void *);

struct list *list_new(void);
void list_free(struct list *list, list_free_func_t fn_free);
void list_push_back(struct list *list, void *item);
void *list_at(struct list *list, size_t index);
size_t list_size(struct list *list);

#endif /* LIST_H_ */