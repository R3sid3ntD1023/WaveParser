cmake_minimum_required( VERSION 3.8)

file(GLOB_RECURSE WAVE_SRC CMAKE_CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_library(waveparser STATIC LANGUAGES CXX)
set_property(TARGET waveparser PROPERTY CXX_STANDARD 20)
target_sources(TARGET waveparser PRIVATE ${WAVE_SRC})
target_include_directories(TARGET waveparser PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

if(MSVC)
	target_compile_options(waveparser PUBLIC /ZI PUBLIC /INCREMETAL PUBLIC /MP)
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug$<$<CONFIG:Debug>:Debug>")
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
	
endif()