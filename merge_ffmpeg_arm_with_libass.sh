NDK_ROOT=$HOME/Library/Android/sdk/ndk-bundle
SYSROOT=$NDK_ROOT/platforms/android-19/arch-arm/
TOOLCHAIN=$NDK_ROOT/toolchains/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64

# this command made from make log
# make -j8 SHELL="sh -x" 2>&1 | tee make.log

$TOOLCHAIN/bin/arm-linux-androideabi-gcc -fPIC -shared -o libffmpeg.so \
  --sysroot="$SYSROOT" \
  cmdutils.o ffmpeg.o ffmpeg_filter.o ffmpeg_opt.o
$TOOLCHAIN/bin/arm-linux-androideabi-gcc -Llibavcodec -Llibavdevice -Llibavfilter -Llibavformat -Llibavresample -Llibavutil -Llibpostproc -Llibswscale -Llibswresample --sysroot=/Users/takuma_okamoto_pn089/Library/Android/sdk/ndk-bundle/platforms/android-19/arch-arm/ -isysroot /Users/takuma_okamoto_pn089/Library/Android/sdk/ndk-bundle/platforms/android-19/arch-arm/ -Wl,--as-needed -Wl,-z,noexecstack -Wl,--warn-common -Wl,-rpath-link=libpostproc:libswresample:libswscale:libavfilter:libavdevice:libavformat:libavcodec:libavutil:libavresample -fPIC -shared -o libffmpeg.so cmdutils.o ffmpeg_opt.o ffmpeg_filter.o ffmpeg.o -lavfilter -lavformat -lavcodec -lswresample -lswscale -lavutil -L/usr/local/lib -lopenh264 -L/freetype/lib -lfreetype -L/usr/local/lib -lass -lm -lz -pthread

#/Users/takuma_okamoto_pn089/Library/Android/sdk/ndk-bundle/toolchains/arm-linux-androideabi-4.9/prebuilt/darwin-x86_64/bin/arm-linux-androideabi-gcc -Llibavcodec -Llibavdevice -Llibavfilter -Llibavformat -Llibavresample -Llibavutil -Llibpostproc -Llibswscale -Llibswresample --sysroot=/Users/takuma_okamoto_pn089/Library/Android/sdk/ndk-bundle/platforms/android-19/arch-arm/ -isysroot /Users/takuma_okamoto_pn089/Library/Android/sdk/ndk-bundle/platforms/android-19/arch-arm/ -Wl,--as-needed -Wl,-z,noexecstack -Wl,--warn-common -Wl,-rpath-link=libpostproc:libswresample:libswscale:libavfilter:libavdevice:libavformat:libavcodec:libavutil:libavresample -o ffmpeg_g cmdutils.o ffmpeg_opt.o ffmpeg_filter.o ffmpeg.o -lavfilter -lavformat -lavcodec -lswresample -lswscale -lavutil -L/usr/local/lib -lopenh264 -L/freetype/lib -lfreetype -L/usr/local/lib -lass -lm -lz -pthread
