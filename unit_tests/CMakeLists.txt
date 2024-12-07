cmake_minimum_required(VERSION 3.8)

add_subdirectory("example")

target_compile_definitions(example PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/tests")
