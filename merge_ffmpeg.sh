NDK_ROOT=$HOME/Library/Android/sdk/ndk-bundle
SYSROOT=$NDK_ROOT/platforms/android-19/arch-arm/
TOOLCHAIN=$NDK_ROOT/toolchains/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64

$TOOLCHAIN/bin/arm-linux-androideabi-gcc -fPIC -shared -o libffmpeg.so \
  --sysroot="$SYSROOT" \
  cmdutils.o ffmpeg.o ffmpeg_filter.o ffmpeg_opt.o
