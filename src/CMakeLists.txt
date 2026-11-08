cmake_minimum_required(VERSION 3.15)
project(strutils VERSION 4.2 LANGUAGES C CXX)
 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
 
if(NOT MSVC)
    message(FATAL_ERROR "libstr requiere arquitectura Win y compilador MSVC.")
endif()
 
if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(PLATFORM_DIR "x32")
else()
    set(PLATFORM_DIR "x64")
endif()

set(ARQUITECTURE_DIR ${CMAKE_SYSTEM_PROCESSOR})
if(NOT ARQUITECTURE_DIR)
    set(ARQUITECTURE_DIR ${CMAKE_HOST_SYSTEM_NAME})
endif()

set(DIR_BUILD "${PLATFORM_DIR}-${ARQUITECTURE_DIR}")
 
set(SRC_strutils
    "${CMAKE_CURRENT_SOURCE_DIR}/../core.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/../core.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/core_string.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/core_string.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/strings.rc"
)
 
add_library(strutils SHARED ${SRC_strutils})
 
set_target_properties(strutils PROPERTIES LINK_FLAGS "/DEF:${CMAKE_CURRENT_SOURCE_DIR}/core/strings.def")
 
set_target_properties(strutils PROPERTIES
    PREFIX ""
    OUTPUT_NAME "libstr"
    SUFFIX ".dll"
    WINDOWS_EXPORT_ALL_SYMBOLS FALSE 
) 

target_compile_definitions(strutils PRIVATE
    WIN32 _WINDOWS _USRDLL UNICODE _UNICODE _CRT_SECURE_NO_WARNINGS TARGET_OS_WIN32 WIN32_LEAN_AND_MEAN
) 
 
set(DIR_BIN "${CMAKE_CURRENT_SOURCE_DIR}/../../bin/${DIR_BUILD}")

set_target_properties(strutils PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${DIR_BIN}/$<CONFIG>"
    LIBRARY_OUTPUT_DIRECTORY "${DIR_BIN}/$<CONFIG>"
    ARCHIVE_OUTPUT_DIRECTORY "${DIR_BIN}/$<CONFIG>"
)
 
add_custom_command(TARGET strutils POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/examples"
        "$<TARGET_FILE_DIR:strutils>/"
)
