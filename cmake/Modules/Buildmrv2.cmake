# SPDX-License-Identifier: BSD-3-Clause
# mrv2 (mrViewer2)
# Copyright Contributors to the mrv2 Project. All rights reserved.

include(ExternalProject)


set(mrv2_ARGS
    ${TLRENDER_EXTERNAL_ARGS}
    -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHIECTURES}
    -DCMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET}
    -DCMAKE_VERBOSE_MAKEFILE=${CMAKE_VERBOSE_MAKEFILE}
    -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
    -DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}
    -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
    -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
    -DTLRENDER_MMAP=${TLRENDER_MMAP}
    -DTLRENDER_PYTHON=${TLRENDER_PYTHON}
    -DTLRENDER_OCIO=${TLRENDER_OCIO}
    -DTLRENDER_AUDIO=${TLRENDER_AUDIO}
    -DTLRENDER_FREETYPE=${TLRENDER_FREETYPE}
    -DTLRENDER_JPEG=${TLRENDER_JPEG}
    -DTLRENDER_TIFF=${TLRENDER_TIFF}
    -DTLRENDER_PNG=${TLRENDER_PNG}
    -DTLRENDER_EXR=${TLRENDER_EXR}
    -DTLRENDER_FFMPEG=${TLRENDER_FFMPEG}
    -DTLRENDER_GL=ON
    -DTLRENDER_BMD=${TLRENDER_BUILD_BMD}
    -DTLRENDER_BMD_SDK=${TLRENDER_BMD_SDK}
)

ExternalProject_Add(
    mrv2
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/mrv2
    DEPENDS tlRender FLTK BOOST ${Gettext} ${ZLIB_SHARED}
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/mrv2
    LIST_SEPARATOR |
    CMAKE_ARGS ${mrv2_ARGS})