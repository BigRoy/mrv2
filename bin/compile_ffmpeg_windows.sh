#!/usr/bin/env bash
# SPDX-License-Identifier: BSD-3-Clause
# mrv2
# Copyright Contributors to the mrv2 Project. All rights reserved.

#
# This script compiles a GPL or BSD version of ffmpeg. The GPL version has
# libx264 encoding and libvpx support.  The BSD version does not have libx264.
#
# You must run it from the root of the mrv2 project.
#

. etc/build_dir.sh

if [[ $KERNEL != *Msys* ]]; then
    echo
    echo "This script is for Windows MSys2-64 only."
    echo
    exit 1
fi

export LIBVPX_TAG=v1.12.0
export X264_TAG=master
export FFMPEG_TAG=n6.0

export GPL=""
for i in $@; do
    case $i in
	-g|--gpl)
	    export GPL=GPL
	    shift
	    ;;
	-b|--bsd|-l|--lgpl)
	    export GPL=LGPL
	    shift
	    ;;
	-j)
	    shift
	    shift
	    ;;
	*)
	    echo
	    echo "Unknown parameter.  Usage is:"
	    echo
	    echo "$0 --gpl"
	    echo
	    echo "or:"
	    echo
	    echo "$0 --bsd"
	    exit 1
	    ;;
    esac
done

ROOT_DIR=$PWD/$BUILD_DIR/tlRender/etc/SuperBuild/FFmpeg
export INSTALL_DIR=$PWD/$BUILD_DIR/install

if [[ $GPL == GPL ]]; then
    echo
    echo "GPL ffmpeg will be built in $ROOT_DIR"
    echo
elif [[ $GPL == LGPL ]]; then
    echo
    echo "LGPL ffmpeg will be built in $ROOT_DIR"
    echo
else
    echo
    echo "You need to provide either a --gpl or --bsd flag."
    exit 1
fi
#
# This configures the environment for compilation.  It also cleans at the
# end to leave it ready for mrv2 build.
#
export MSYS_LIBS=1

#
# Build with libvpx (webm) movies.
#
export BUILD_LIBVPX=1

#
# Build wiht h264 encoding.
#
export BUILD_LIBX264=1
if [[ $GPL == LGPL ]]; then
    export BUILD_LIBX264=0
fi

#
# Build FFMPEG with the GPL libraries specified above.
#
export BUILD_FFMPEG=1


#
# Fix CMake/Ninja include list when compiling on non-English locale
#
export VSLANG=1033  # @bug: does not work


if [[ $MSYS_LIBS == 1 ]]; then

    #echo ""
    #echo "If you are asked to close the terminal, you should do so and"
    #echo "run this script again."
    #echo ""
    pacman -Sy --noconfirm

    #
    # This is for libx264 and ffmpeg
    #
    echo "Installing packages needed to build libx264 and ffmpeg..."
    pacman -S make diffutils yasm nasm pkg-config --noconfirm

fi


mkdir -p $ROOT_DIR

cd    $ROOT_DIR

mkdir -p sources
mkdir -p build



#############
## BUILDING #
#############


## Build libvpx
export ENABLE_LIBVPX=""
if [[ $BUILD_LIBVPX == 1 ]]; then
    cd $ROOT_DIR/sources
    if [[ ! -d libvpx ]]; then
	git clone --depth 1 https://chromium.googlesource.com/webm/libvpx
    fi
    
    if [[ ! -e $INSTALL_DIR/lib/vpx.lib ]]; then
	cd $ROOT_DIR/build
	mkdir -p libvpx
	cd libvpx
    
	./../../sources/libvpx/configure --prefix=$INSTALL_DIR --target=x86_64-win64-vs16 --disable-examples --disable-docs 
	make -j ${CPU_CORES}
	make install
	mv $INSTALL_DIR/lib/x64/vpxmd.lib $INSTALL_DIR/lib/vpx.lib
	rm -rf $INSTALL_DIR/lib/x64/
    fi
    
    export ENABLE_LIBVPX='--enable-libvpx --extra-libs=vpx.lib --extra-libs=kernel32.lib --extra-libs=user32.lib --extra-libs=gdi32.lib --extra-libs=winspool.lib --extra-libs=shell32.lib --extra-libs=ole32.lib --extra-libs=oleaut32.lib --extra-libs=uuid.lib --extra-libs=comdlg32.lib --extra-libs=advapi32.lib --extra-libs=msvcrt.lib'
fi


#
# Build x264
#
export ENABLE_LIBX264=""
if [[ $BUILD_LIBX264 == 1 ]]; then
    cd $ROOT_DIR/sources

    if [[ ! -d x264 ]]; then
	git clone --depth 1 https://code.videolan.org/videolan/x264.git --branch ${X264_TAG}
	#
	# Fix x264 build scripts
	#
	cd x264
	curl "http://git.savannah.gnu.org/gitweb/?p=config.git;a=blob_plain;f=config.guess;hb=HEAD" > config.guess
	sed -i 's/host_os = mingw/host_os = msys/' configure
    fi
    

    if [[ ! -e $INSTALL_DIR/lib/libx264.lib ]]; then
	cd $ROOT_DIR/build
	mkdir -p x264
	cd x264
	CC=cl ./../../sources/x264/configure --prefix=$INSTALL_DIR --enable-shared
	make -j ${CPU_CORES}
	make install
	mv $INSTALL_DIR/lib/libx264.dll.lib $INSTALL_DIR/lib/libx264.lib
    fi
    
    export ENABLE_LIBX264="--enable-libx264"
fi

#
# Build ffmpeg
#

if [[ $BUILD_FFMPEG == 1 ]]; then
    cd $ROOT_DIR/sources

    if [[ ! -d ffmpeg ]]; then
	git clone --depth 1 git://source.ffmpeg.org/ffmpeg.git --branch ${FFMPEG_TAG}
    fi

    if [[ ! -d $INSTALL_DIR/lib/avformat.lib ]]; then
	cd $ROOT_DIR/build
	mkdir -p ffmpeg
	cd ffmpeg

	export CC=cl
	export PKG_CONFIG_PATH=$INSTALL_DIR/lib/pkgconfig
	
	./../../sources/ffmpeg/configure \
            --prefix=$INSTALL_DIR \
            --toolchain=msvc \
	    --target-os=win64 \
            --arch=x86_64 \
            --enable-x86asm \
            --enable-asm \
            --enable-shared \
            --disable-static \
            --disable-programs \
            --enable-swresample \
            $ENABLE_LIBX264 \
	    $ENABLE_LIBVPX \
            --enable-gpl \
            --extra-ldflags="-LIBPATH:$INSTALL_DIR/lib/" \
            --extra-cflags="-I$INSTALL_DIR/include/"

	# @bug: MSVC creates bad characters on MSVC with different languages but
	# we can't fix it
	make -j ${CPU_CORES}
	make install

	# @bug: FFmpeg places .lib in bin/
	mv $INSTALL_DIR/bin/*.lib $INSTALL_DIR/lib/
    fi
fi


if [[ $MSYS_LIBS == 1 ]]; then
    #
    # Remove the libx264 and ffmpeg libs we downloaded
    #
    echo "Removing packages used to build libx264 and ffmpeg..."
    pacman -R make diffutils yasm nasm pkg-config --noconfirm
fi
