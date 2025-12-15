set(MODULE_NAME strc)
add_executable(${MODULE_NAME} strc.c)
target_link_libraries(${MODULE_NAME} PRIVATE tools)