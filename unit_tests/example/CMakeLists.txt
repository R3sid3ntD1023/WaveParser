cmake_minimum_required(VERSION 3.8)

file(GLOB_RECURSE TEST_SRC CMAKE_CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_executable(example LANGUAGES CXX)
set_property(TARGET example PROPERTY CXX_STANDARD 20)
target_sources(example PRIVATE ${TEST_SRC})
target_link_directories(example PUBLIC waveparser)

if(MSVC)
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug$<$<CONFIG:Debug>:Debug>")
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
	target_compile_options(example PRIVATE /ZI PRIVATE /INCREMENTAL PRIVATE /MP)
	target_compile_definitions(example PUBLIC _CRT_SECURE_NO_WARNINGS)
endif()
