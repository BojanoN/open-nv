LIBDIR = .
DIRS= ./src/glew ./src/glew/auto ./src/zlib ./src/libav ./src/sdl2 ./src/openal-soft/build
DIRSCLEAN=$(addsuffix clean,$(DIRS))

CC=clang
CXX=clang++

all: glew zlib libav sdl2

zlib:
	cd src/zlib &&./configure
	$(MAKE) -C ./src/zlib static shared
	-	ln -s ./src/zlib/libz.a libz.a
	- ln -s ./src/zlib/libz.so libz.so

glew:
	$(MAKE) -C  ./src/glew/auto
	$(MAKE) -C ./src/glew
	- ln -s ./src/glew/lib/libGLEW.a ./libGLEW.a
	- ln -s ./src/glew/lib/libGLEW.so.2.2 ./libGLEW.so

libav:
	cd src/libav  && ./configure --enable-shared --disable-doc  --disable-programs   --enable-avutil --enable-avcodec --enable-avformat --enable-swresample --enable-swscale
	$(MAKE) -C ./src/libav
	- ln -s ./src/libav/libavcodec/libavcodec.so.58 libavcodec.so.58
	- ln -s ./src/libav/libavutil/libavutil.so.56 libavutil.so.56
	- ln -s ./src/libav/libavformat/libavformat.so.58 libavformat.so.58
	- ln -s ./src/libav/libswscale/libswscale.so.5 libswscale.so.5
	- ln -s ./src/libav/libswresample/libswresample.so.3 libswresample.so.3

sdl2:
	cd src/sdl2 && ./autogen.sh && ./configure --prefix=$(CURDIR)/src/sdl2  --disable-audio --disable-power --disable-sensor --disable-pulseaudio --disable-alsa --disable-sndio --disable-jack --disable-alsatest --enable-video-opengl --disable-video-opengles1 --disable-video-opengles2 --disable-video-vulkan --disable-render-d3d 
	$(MAKE) -C src/sdl2
	$(MAKE) -C src/sdl2 install
	- ln -s ./src/sdl2/lib/libSDL2.a libSDL2.a
	- ln -s ./src/sdl2/lib/libSDL2-2.0.so.0 libSDL2-2.0.so.0

clean: buildclean
	- rm -f *.a *.so
	- $(MAKE) -C ./src/sdl2 uninstall

buildclean: $(DIRSCLEAN)
%clean: %
	$(MAKE) -C $< clean
