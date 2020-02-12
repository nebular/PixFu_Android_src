#
# PGE Runner CMAKE Script
# Author: Rodolfo Lopez Pintor 2020
#

cmake_minimum_required(VERSION 3.4.1)

# The olcPGE itself is a separate GitHub & CMAKE project. Is is cloned as a submodule
# in this subdir, and includes its own CMAKE ! This will create olcPGE_portable.so.

add_subdirectory(PixEngine_core)

include_directories(PixEngine_core/src)
include_directories(PixEngine_core/src/core/headers)
include_directories(PixEngine_core/src/items/headers)
include_directories(PixEngine_core/src/input/headers)
include_directories(PixEngine_core/src/arch/android/libpng/jni)

#  Our second SO library is the launcher native part, that we link with the PGE

add_library(pixEngine_launcher SHARED
        ${GL3STUB_SRC}
        PixEngine_launcher/PgeRunner.cpp
        PixEngine_launcher/Renderer.cpp
        PixEngine_launcher/RendererPge.cpp
        )

target_link_libraries(pixEngine_launcher
        PUBLIC
        pixEngine_core
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
