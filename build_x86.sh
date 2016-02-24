NDK_ROOT=$HOME/Library/Android/sdk/ndk-bundle
SYSROOT=$NDK_ROOT/platforms/android-19/arch-x86/
TOOLCHAIN=$NDK_ROOT/toolchains/x86-4.8/prebuilt/darwin-x86_64
CPU=x86
PREFIX=$(pwd)/android/debug/$CPU
ADDI_CFLAGS='-march=i686'

./configure \
  --prefix=$PREFIX \
  --target-os=linux \
  --cross-prefix=$TOOLCHAIN/bin/i686-linux-android- \
  --arch=x86 \
  --cpu=i686 \
  --disable-static \
  --enable-debug=3 \
  --disable-stripping \
  --enable-shared \
  --disable-doc \
  --disable-ffplay \
  --disable-ffprobe \
  --disable-ffserver \
  --disable-avdevice \
  --disable-symver \
  --disable-yasm \
  --sysroot="$SYSROOT" \
  --enable-cross-compile \
  --enable-libopenh264 \
  --pkg-config=$(which pkg-config) \
  --extra-cflags="-std=c99 -Os -Wall -fpic -pipe -DANDROID -DNDEBUG  -march=atom -msse3 -ffast-math -mfpmath=sse" \
  --extra-ldflags="-lm -lz -Wl,--no-undefined -Wl,-z,noexecstack" \
  $ADDITIONAL_CONFIGURE_FLAG
