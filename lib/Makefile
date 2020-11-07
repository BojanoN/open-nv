LIBDIR = .
DIRS= ./src/glew ./src/glew/auto ./src/zlib ./src/libav ./src/sdl2 ./src/openal-soft/build
DIRSCLEAN=$(addsuffix clean,$(DIRS))

CC=clang
CXX=clang++

all: glew zlib libav sdl2 openal

zlib:
	cd src/zlib &&./configure
	$(MAKE) -C ./src/zlib static
	ln -s ./src/zlib/libz.a libz.a
	ln -s ./src/zlib/libz.so libz.so

glew:
	$(MAKE) -C  ./src/glew/auto
	$(MAKE) -C ./src/glew
	ln -s ./src/glew/lib/libGLEW.a ./libGLEW.a
	ln -s ./src/glew/lib/libGLEW.so ./libGLEW.so

libav:
	cd src/libav  && ./configure --enable-shared --disable-doc  --disable-programs  --disable-everything --enable-avutil --enable-avcodec --enable-avformat --enable-swresample --enable-swscale	--enable-decoder=mp3 --enable-decoder=binkaudio_dct --enable-decoder=binkaudio_rdft --enable-decoder=bink --enable-decoder=wavpack
	$(MAKE) -C ./src/libav
	ln -s ./src/libav/libavcodec/libavcodec.so. libavcodec.so
	ln -s ./src/libav/libavcodec/libavcodec.a libavcodec.a
	ln -s ./src/libav/libavutil/libavutil.so libavutil.so
	ln -s ./src/libav/libavutil/libavutil.a libavutil.a
	ln -s ./src/libav/libavformat/libavformat.so libavformat.so
	ln -s ./src/libav/libavformat/libavformat.a libavformat.a
	ln -s ./src/libav/libswscale/libswscale.so libswscale.so
	ln -s ./src/libav/libswscale/libswscale.a libswscale.a
	ln -s ./src/libav/libswresample/libswresample.so libswresample.so
	ln -s ./src/libav/libswresample/libswresample.a libswresample.a

sdl2:
	cd src/sdl2 && ./autogen.sh && ./configure --prefix=$(CURDIR)/src/sdl2  --disable-audio --disable-power --disable-sensor --disable-pulseaudio --disable-alsa --disable-sndio --disable-jack --disable-alsatest --enable-video-opengl --disable-video-opengles1 --disable-video-opengles2 --disable-video-vulkan --disable-render-d3d 
	$(MAKE) -C src/sdl2
	$(MAKE) -C src/sdl2 install
	ln -s ./src/sdl2/lib/libSDL2.a libSDL2.a
	ln -s ./src/sdl2/lib/libSDL2.so libSDL2.so

openal:
	cd src/openal-soft/build && cmake ..
	$(MAKE) -C ./src/openal-soft/build
	ln -s ./src/openal-soft/build/libopenal.so	libopenal.so

clean: buildclean
	- rm -f *.a *.so
	- $(MAKE) -C ./src/sdl2 uninstall

buildclean: $(DIRSCLEAN)
%clean: %
	$(MAKE) -C $< clean
