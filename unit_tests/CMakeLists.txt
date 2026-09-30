cmake_minimum_required(VERSION 3.5...4.0)

add_subdirectory("example")

target_compile_definitions(example PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/tests")
