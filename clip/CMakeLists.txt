set(MODULE_NAME clip)
add_executable(${MODULE_NAME} main.c clip.c)
target_link_libraries(${MODULE_NAME} PRIVATE tools)