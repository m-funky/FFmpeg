NDK_ROOT=$HOME/Library/Android/sdk/ndk-bundle
SYSROOT=$NDK_ROOT/platforms/android-19/arch-arm/
TOOLCHAIN=$NDK_ROOT/toolchains/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64
CPU=arm
PREFIX=$(pwd)/output/android/debug/$CPU
ADDI_CFLAGS="-marm"

./configure \
  --prefix=$PREFIX \
  --target-os=android \
  --cross-prefix=$TOOLCHAIN/bin/arm-linux-androideabi- \
  --arch=armeabi-v7a \
  --cpu=armv7-a \
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
  --sysroot="$SYSROOT" \
  --enable-cross-compile \
  --enable-jni \
  --enable-mediacodec \
  --enable-libopenh264 \
  --enable-libfreetype \
  --pkg-config=$(which pkg-config) \
  --extra-cflags="-Os -fpic -mfloat-abi=softfp $ADDI_CFLAGS" \
  --extra-ldflags="$ADDI_LDFLAGS" \
  $ADDITIONAL_CONFIGURE_FLAG
