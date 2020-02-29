#
# PGE Runner CMAKE Script
# Author: Rodolfo Lopez Pintor 2020
#

cmake_minimum_required(VERSION 3.4.1)

# The olcPGE itself is a separate GitHub & CMAKE project. Is is cloned as a submodule
# in this subdir, and includes its own CMAKE ! This will create olcPGE_portable.so.

add_subdirectory(pixFu)
add_subdirectory(pixFu_Extensions)

include_directories(pixFu/src)
include_directories(pixFu/src/core/headers)
include_directories(pixFu/src/items/headers)
include_directories(pixFu/src/input/headers)
include_directories(pixFu/src/arch/android/libpng/jni)
include_directories(pixFu/src/thirdparty/glm)

#  Our second SO library is the launcher native part, that we link with the PGE

add_library(pixFu_launcher SHARED
        ${GL3STUB_SRC}
        PixFu_launcher/Launcher.cpp
        PixFu_launcher/Renderer.cpp
        PixFu_launcher/RendererPix.cpp
        )

target_link_libraries(pixFu_launcher
        PUBLIC
        pixFu
        m
        )

#
# From now on, the Gradle Scripts takes back control and will cook an AAR module wifh:
#
# - The SO libraries
# - The header files zipped
# - The Launcher Java Activities, Manifests, etc
#
# Just by including this AAR as a module (see the examples) you are good to go. Your own CMAKE
# script and IDE will be able to use the libaries and headers created here.
#
