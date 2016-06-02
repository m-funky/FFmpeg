NDK_ROOT=$HOME/Library/Android/sdk/ndk-bundle
SYSROOT=$NDK_ROOT/platforms/android-19/arch-arm/
TOOLCHAIN=$NDK_ROOT/toolchains/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64

# this command made from make log
# make -j8 SHELL="sh -x" 2>&1 | tee make.log

rm $(pwd)/output/android/debug/arm/include/*
rm $(pwd)/output/android/debug/arm/libs/*

make clean
make -j8

$TOOLCHAIN/bin/arm-linux-androideabi-gcc \
  -Llibavcodec \
  -Llibavfilter \
  -Llibavformat \
  -Llibavresample \
  -Llibavutil \
  -Llibpostproc \
  -Llibswscale \
  -Llibswresample \
  --sysroot=$SYSROOT \
  -isysroot $SYSROOT \
  -Wl,--as-needed \
  -Wl,-z,noexecstack \
  -Wl,--warn-common \
  -Wl,-rpath-link=libpostproc:libswresample:libswscale:libavfilter:libavformat:libavcodec:libavutil:libavresample \
  -fPIC \
  -shared \
  -o libffmpeg.so cmdutils.o ffmpeg_opt.o ffmpeg_filter.o ffmpeg.o \
  -lavfilter \
  -lavformat \
  -lavcodec \
  -lswresample \
  -lswscale \
  -lavutil \
  -L/usr/local/lib \
  -lopenh264 \
  -L/freetype/lib \
  -lfreetype \
  -L/usr/local/lib \
  -lass \
  -lm \
  -lz \
  -pthread

mkdir -p $(pwd)/output/android/debug/arm/libs
mkdir -p $(pwd)/output/android/debug/arm/include

mv libffmpeg.so $(pwd)/output/android/debug/arm/libs/.

cp libavcodec/libavcodec-*.so $(pwd)/output/android/debug/arm/libs/.
cp libavfilter/libavfilter-*.so $(pwd)/output/android/debug/arm/libs/.
cp libavformat/libavformat-*.so $(pwd)/output/android/debug/arm/libs/.
cp libavutil/libavutil-*.so $(pwd)/output/android/debug/arm/libs/.
cp libswresample/libswresample-*.so $(pwd)/output/android/debug/arm/libs/.
cp libswscale/libswscale-*.so $(pwd)/output/android/debug/arm/libs/.

cp ffmpeg.h $(pwd)/output/android/debug/arm/include/.
cp cmdutils.h $(pwd)/output/android/debug/arm/include/.
cp config.h $(pwd)/output/android/debug/arm/include/.

