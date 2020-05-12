PROJECT=openNV

SOURCEDIR = $(CURDIR)/src
BUILDDIR = $(CURDIR)/build
LIBDIR = $(CURDIR)/lib
INCLUDEDIRS = $(SOURCEDIR) $(LIBDIR)/include

export

LDFLAGS= -L$(LIBDIR) -lz 

all:
	$(MAKE) -C $(SOURCEDIR) all

bsatest:
	$(MAKE) -C $(SOURCEDIR) bsatest

debug:
	$(MAKE) -C $(SOURCEDIR) debug

leak-check: debug
	valgrind --leak-check=full --show-leak-kinds=all $(BUILDDIR)/$(PROJECT)

libs:
	make -C $(LIBDIR) all

libclean:
	make -C $(LIBDIR) clean

clean:
	- rm -rf $(BUILDDIR)
	make -C $(LIBDIR) buildclean

-include $(OBJS:.o=.d)
