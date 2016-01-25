NDK_ROOT=$HOME/Library/Android/sdk/ndk-bundle
SYSROOT=$NDK_ROOT/platforms/android-19/arch-arm/
TOOLCHAIN=$NDK_ROOT/toolchains/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64
CPU=arm
PREFIX=$(pwd)/android/$CPU
ADDI_CFLAGS="-marm"

./configure \
  --prefix=$PREFIX \
  --target-os=linux \
  --cross-prefix=$TOOLCHAIN/bin/arm-linux-androideabi- \
  --arch=arm \
  --cpu=armv7-a \
  --disable-static \
  --enable-shared \
  --disable-doc \
  --disable-ffmpeg \
  --disable-ffplay \
  --disable-ffprobe \
  --disable-ffserver \
  --disable-avdevice \
  --disable-symver \
  --sysroot="$SYSROOT" \
  --enable-cross-compile \
  --enable-libopenh264 \
  --pkg-config=$(which pkg-config) \
  --extra-cflags="-Os -fpic $ADDI_CFLAGS" \
  --extra-ldflags="$ADDI_LDFLAGS" \
  $ADDITIONAL_CONFIGURE_FLAG

