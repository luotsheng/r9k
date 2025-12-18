set(MODULE_NAME argparser)
add_executable(${MODULE_NAME} argparser.c)
target_link_libraries(${MODULE_NAME} PRIVATE tools)