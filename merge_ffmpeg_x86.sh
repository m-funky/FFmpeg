NDK_ROOT=$HOME/Library/Android/sdk/ndk-bundle
SYSROOT=$NDK_ROOT/platforms/android-19/arch-x86/
TOOLCHAIN=$NDK_ROOT/toolchains/x86-4.9/prebuilt/darwin-x86_64

$TOOLCHAIN/bin/i686-linux-android-gcc -Llibavcodec -Llibavdevice -Llibavfilter -Llibavformat -Llibavresample -Llibavutil -Llibpostproc -Llibswscale -Llibswresample --sysroot=$SYSROOT -isysroot $SYSROOT -Wl,--as-needed -Wl,-z,noexecstack -Wl,--warn-common -Wl,-rpath-link=libpostproc:libswresample:libswscale:libavfilter:libavdevice:libavformat:libavcodec:libavutil:libavresample -fPIC -shared -o libffmpeg.so cmdutils.o ffmpeg_opt.o ffmpeg_filter.o ffmpeg.o -lavfilter -lavformat -lavcodec -lswresample -lswscale -lavutil -L/usr/local/lib -lopenh264 -L/usr/local/lib -lfreetype -lm -lz -pthread
