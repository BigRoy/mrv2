#!/usr/bin/env bash


echo "Copying FFmpeg from $FFMPEG_ROOT to $BUILD_DIR"

mabs_build=${MABS_ROOT}/build
ffmpeg_options=${mabs_build}/ffmpeg_options.txt
if [[ ! -d $FFMPEG_ROOT/include/ && -e $ffmpeg_options ]];then
   echo "FFMPEG_ROOT $FFMPEG_ROOT/include is missing"
   exit 1
fi



if [[ -d ${MABS_ROOT} ]]; then
    echo "******************************************"
    echo "We located a media-autobuild_suite install"
    echo "******************************************"

    if [[ ! -e $ffmpeg_options ]]; then
	echo "cp -r $PWD/windows/media-autobuild_suite/build/* $mabs_build"
	cp -r $PWD/windows/media-autobuild_suite/build/* $mabs_build
	echo "You have not run the media-autobuild_suite yet."
	echo ""
	echo "In Windows' Explorer, go to:"
	echo "  ${MABS_ROOT}"
	echo "and click on the media-autobuild_suite.bar script."
	exit 1
    fi

    echo "Copying $FFMPEG_ROOT/bin-video/*.lib to $PWD/$BUILD_DIR/install/lib"
    cp -f ${FFMPEG_ROOT}/bin-video/*.lib $PWD/$BUILD_DIR/install/lib
    ls $PWD/$BUILD_DIR/install/lib

    echo "Copying $FFMPEG_ROOT/bin-video/*.dll to $PWD/$BUILD_DIR/install/bin"
    cp -f ${FFMPEG_ROOT}/bin-video/*.dll $PWD/$BUILD_DIR/install/bin
    ls $PWD/$BUILD_DIR/install/bin

else
    echo "******************************************"
    echo "Not a media-autobuild_suite install"
    echo "******************************************"
    echo "Copying $FFMPEG_ROOT/bin/*.lib to $PWD/$BUILD_DIR/install/lib"
    cp -f ${FFMPEG_ROOT}/bin/*.lib $PWD/$BUILD_DIR/install/lib
    ls $PWD/$BUILD_DIR/install/lib

    echo "Copying $FFMPEG_ROOT/bin/*.dll to $PWD/$BUILD_DIR/install/bin"
    cp -f ${FFMPEG_ROOT}/bin/*.dll $PWD/$BUILD_DIR/install/bin
    ls $PWD/$BUILD_DIR/install/bin
fi

echo "Copying header durectories from ${FFMPEG_ROOT}/include to $$PWD/$BUILD_DIR/install/include"
cp -r ${FFMPEG_ROOT}/include/lib* $PWD/$BUILD_DIR/install/include
ls -l $PWD/$BUILD_DIR/install/include

echo "Copying ${LIBINTL_ROOT}/lib/libintl.lib $PWD/$BUILD_DIR/install/lib/"
cp -f ${LIBINTL_ROOT}/lib/libintl.lib   $PWD/$BUILD_DIR/install/lib/
echo "Copying ${LIBINTL_ROOT}/bin/libiconv-2.dll $PWD/$BUILD_DIR/install/bin/"
cp -f ${LIBINTL_ROOT}/bin/libintl-8.dll $PWD/$BUILD_DIR/install/bin/
echo "Copying ${LIBINTL_ROOT}/bin/libiconv-2.dll $PWD/$BUILD_DIR/install/bin/"
cp -f ${LIBINTL_ROOT}/bin/libiconv-2.dll $PWD/$BUILD_DIR/install/bin/

#
# Let's sleep for 5 seconds so the developer can verify the settings.
#
sleep 5
