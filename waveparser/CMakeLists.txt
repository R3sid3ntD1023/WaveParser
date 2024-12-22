cmake_minimum_required( VERSION 3.8)

file(GLOB_RECURSE WAVE_SRC CMAKE_CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/**.cpp)
add_library(waveparser ${WAVE_SRC})
set_target_properties(
	waveparser PROPERTIES
	CXX_STANDARD 20
)
target_include_directories(waveparser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)


