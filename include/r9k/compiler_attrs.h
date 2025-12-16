/*
-* SPDX-License-Identifier: MIT
 * Copyright (c) 2025 viakko
 */
#ifndef COMPILER_ATTRUBUTES_H_
#define COMPILER_ATTRUBUTES_H_

#define __attr_nonnull(...) __attribute__((nonnull(__VA_ARGS__)))
#define __attr_unused       __attribute__((unused))
#define __attr_noreturn     __attribute__((noreturn))
#define __attr_printf(a, b) __attribute__((format(printf, a, b)))



#endif /* COMPILER_ATTRUBUTES_H_ */