set(MODULE_NAME rsh)
add_executable(${MODULE_NAME} rsh.c)
target_link_libraries(${MODULE_NAME} PRIVATE tools)