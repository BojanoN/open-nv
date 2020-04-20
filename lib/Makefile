LIBDIR = .
DIRS=./src/zlib
DIRSCLEAN=$(addsuffix clean,$(DIRS))

all: zlib

zlib:
	make -C ./src/zlib static
	mv ./src/zlib/libz.a $(LIBDIR)

clean: buildclean
	- rm -f *.a

buildclean: $(DIRSCLEAN)
%clean: %
	make -C $< clean
