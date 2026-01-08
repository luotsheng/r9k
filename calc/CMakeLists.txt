set(MODULE_NAME clac)
add_executable(${MODULE_NAME} calc.c)
target_link_libraries(${MODULE_NAME} PRIVATE tools)