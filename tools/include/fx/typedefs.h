/* typedefs.h: Created by Ekko on 2025/11/18 */
#ifndef TYPEDEFS_H_
#define TYPEDEFS_H_

#define die(fmt, ...)                           \
do {                                            \
        fprintf(stderr, fmt, ##__VA_ARGS__);    \
        exit(EXIT_FAILURE);                     \
} while (0)

#endif /* TYPEDEFS_H_ */