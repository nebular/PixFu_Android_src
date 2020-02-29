cmake_minimum_required(VERSION 3.4.1)

#
# OLC/Android user application CMAKE
#
# - Just add subdirectory "lib" and link your code with "libolcPGE_Android"
# - create the "run.cpp" file to instantiate your class
# - your class will then be started by the launcher
#

set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")

# Main C & C++ flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -DGLM_FORCE_SILENT_WARNINGS -DGLM_FORCE_CXX17 -DGLM_FORCE_LEFT_HANDED -Wno-unknown-pragmas")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -frtti -fexceptions -Wall -DGLM_FORCE_SILENT_WARNINGS -DGLM_FORCE_CXX17 -DGLM_FORCE_LEFT_HANDED  -Wno-unknown-pragmas")

# OpenGL for Android, select ES2 or ES3 if available
if (${ANDROID_PLATFORM_LEVEL} LESS 12)
    message(FATAL_ERROR "OpenGL 2 is not supported before API level 11 \
                      (currently using ${ANDROID_PLATFORM_LEVEL}).")
    return()

elseif (${ANDROID_PLATFORM_LEVEL} LESS 18)
    add_definitions("-DDYNAMIC_ES3")
    set(GL3STUB_SRC opengl/gl3stub.c)
    set(OPENGL_LIB GLESv2)

else ()
    set(OPENGL_LIB GLESv3)

endif (${ANDROID_PLATFORM_LEVEL} LESS 12)

# Add OLC library and Android Launcher
add_subdirectory(lib)

# Make olcpixelengine headers visible to your program
include_directories(lib/pixFu/src)
include_directories(lib/pixFu/src/core/headers)
include_directories(lib/pixFu_Extensions/)

# Add your application source code
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/app)

# Enjoy !