cmake_minimum_required(VERSION 3.5...4.0)

file(GLOB_RECURSE TEST_SRC CMAKE_CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
set(TARGET_NAME example)

add_executable(${TARGET_NAME})
set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 20)
target_sources(${TARGET_NAME} PRIVATE ${TEST_SRC})
target_link_libraries(${TARGET_NAME} PUBLIC waveparser)

if(MSVC)
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug$<$<CONFIG:Debug>:Debug>")
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
	target_compile_options(${TARGET_NAME} PRIVATE /ZI PRIVATE /INCREMENTAL PRIVATE /MP)
endif()
