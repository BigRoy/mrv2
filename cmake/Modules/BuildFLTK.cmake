# SPDX-License-Identifier: BSD-3-Clause
# mrv2 
# Copyright Contributors to the mrv2 Project. All rights reserved.

include( ExternalProject )

#set( FLTK_TAG master )
# Stable TAG
if (NOT APPLE)
    set( FLTK_TAG master )
else()
    set( FLTK_TAG c2cce9cba86dcc208c9396af994010cd49484988 )
endif()


set( FLTK_BUILD_SHARED_LIBS OFF )
if(MRV2_PYFLTK)
    set( FLTK_BUILD_SHARED_LIBS ON )
endif()

set( FLTK_PATCH )
set( FLTK_BUILD_TYPE ${CMAKE_BUILD_TYPE} )
set( FLTK_CXX_COMPILER ${CMAKE_CXX_COMPILER})
set( FLTK_CXX_FLAGS ${CMAKE_CXX_FLAGS} )
set( FLTK_C_COMPILER ${CMAKE_C_COMPILER})
set( FLTK_C_FLAGS ${CMAKE_C_FLAGS} )
if (APPLE)
    set(FLTK_PATCH cmake -E copy_if_different "${PROJECT_SOURCE_DIR}/cmake/patches/FLTK-patch/Fl_Cocoa_Gl_Window_Driver.mm" "${CMAKE_BINARY_DIR}/FLTK-prefix/src/FLTK/src/drivers/Cocoa")
elseif(WIN32)
    set(FLTK_C_COMPILER cl)
    set(FLTK_CXX_COMPILER cl)
else()
    set(FLTK_C_FLAGS -fPIC)
    set(FLTK_CXX_FLAGS -fPIC)
endif()


if (APPLE OR WIN32)
    set( FLTK_WAYLAND OFF )
    set( FLTK_PANGO   OFF )
else()
    set( FLTK_WAYLAND  ON ) # we'll leave it on, but it is way
		       # buggy with, at least, Nvidia cards.
    set( FLTK_PANGO    ON )
endif()

ExternalProject_Add(
    FLTK
    GIT_REPOSITORY "https://github.com/fltk/fltk.git"
    GIT_TAG ${FLTK_TAG}
    PATCH_COMMAND ${FLTK_PATCH}
    CMAKE_ARGS
    -DCMAKE_C_COMPILER=${FLTK_C_COMPILER}
    -DCMAKE_CXX_COMPILER=${FLTK_CXX_COMPILER}
    -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHIECTURES}
    -DCMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}
    -DCMAKE_VERBOSE_MAKEFILE=${CMAKE_VERBOSE_MAKEFILE}
    -DCMAKE_MODULE_PATH=${CMAKE_MODULE_PATH}
    -DCMAKE_BUILD_TYPE=${FLTK_BUILD_TYPE}
    -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
    -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}
    -DCMAKE_C_FLAGS=${FLTK_C_FLAGS}
    -DCMAKE_CXX_FLAGS=${FLTK_CXX_FLAGS}
    -DCMAKE_INSTALL_MESSAGE=${CMAKE_INSTALL_MESSAGE}
    -DFLTK_BUILD_EXAMPLES=OFF
    -DFLTK_BUILD_TEST=OFF
    -DOPTION_BUILD_SHARED_LIBS=${FLTK_BUILD_SHARED_LIBS}
    -DOPTION_USE_SYSTEM_ZLIB=0
    -DOPTION_USE_SYSTEM_LIBJPEG=0
    -DOPTION_USE_SYSTEM_LIBPNG=0
    -DOPTION_USE_FLTK_PANGO=${FLTK_PANGO}
    -DOPTION_USE_FLTK_WAYLAND=${FLTK_WAYLAND}
    )

set(FLTK_DEP FLTK)
