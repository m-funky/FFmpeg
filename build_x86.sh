NDK_ROOT=$HOME/Library/Android/sdk/ndk-bundle
SYSROOT=$NDK_ROOT/platforms/android-19/arch-x86/
TOOLCHAIN=$NDK_ROOT/toolchains/x86-4.9/prebuilt/darwin-x86_64
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
  --enable-libfreetype \
  --pkg-config=$(which pkg-config) \
  --extra-cflags="-Os -fpic" \
  --extra-ldflags="" \
  $ADDITIONAL_CONFIGURE_FLAG
