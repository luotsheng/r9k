set(MODULE_NAME tools)
add_library(${MODULE_NAME} STATIC argparser.c ioutils.c)
target_include_directories(${MODULE_NAME} PUBLIC include)