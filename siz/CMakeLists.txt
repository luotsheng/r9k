set(MODULE_NAME siz)
add_executable(${MODULE_NAME} siz.c)
target_link_libraries(${MODULE_NAME} PRIVATE tools)