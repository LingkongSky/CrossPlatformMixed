cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("CrossPlatformMixed")


set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)

add_executable (CrossPlatformMixed "main.cpp"  "md5.h"  "md5.cpp" "utils.cpp" "crossPlatform.h")

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
	target_link_libraries(CrossPlatformMixed pthread)
endif ()

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CrossPlatformMixed PROPERTY CXX_STANDARD 17)
endif()

target_compile_options(CrossPlatformMixed 
	PRIVATE 
	$<$<CXX_COMPILER_ID:MSVC>:/bigobj> 
	#$<$<CXX_COMPILER_ID:GNU>:-Wa,-mbig-obj>
	) 

set_target_properties(CrossPlatformMixed PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build"
)
