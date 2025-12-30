/*
-* SPDX-License-Identifier: MIT
 * Copyright (c) 2025 Varketh Nockrath
 */
#ifndef BASE64_H_
#define BASE64_H_

#include <stdlib.h>

int  base64_encode(const unsigned char *data, size_t len, char **p_plain);
int  base64_decode(const char *b64, size_t *out_len, unsigned char **p_cipher);

#endif /* BASE64_H_ */