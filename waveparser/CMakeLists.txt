cmake_minimum_required( VERSION 3.8)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

file(GLOB_RECURSE WAVE_SRC CMAKE_CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/**.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/include/**.h")
add_library(waveparser)
set_property(TARGET waveparser PROPERTY CXX_STANDARD 20)
target_sources(waveparser PRIVATE ${WAVE_SRC})
target_include_directories(waveparser PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include/")

if(MSVC)
	target_compile_options(waveparser PUBLIC /ZI PUBLIC /INCREMETAL PUBLIC /MP)
	target_compile_definitions(waveparser PUBLIC _CRT_SECURE_NO_WARNINGS)
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug$<$<CONFIG:Debug>:Debug>")
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
	
endif()



