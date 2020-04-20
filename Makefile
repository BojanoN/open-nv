PROJECT=openNV

DIRS := ./src/esm ./src ./src/logc ./src/util
SOURCEDIR = ./src
BUILDDIR = ./build
LIBDIR = ./lib
INCLUDEDIRS = $(SOURCEDIR) $(LIBDIR)/include

FILES := $(foreach dir,$(DIRS),$(wildcard $(dir)/*.cpp))
BUILDFILES := $(subst $(SOURCEDIR)/,$(BUILDDIR)/,$(FILES)) 
OBJS := $(BUILDFILES:.cpp=.o)


CC=clang++
INCLUDEFLAGS = $(foreach d, $(INCLUDEDIRS), -I$d)
CFLAGS := --std=c++17 -DLOG_USE_COLOR $(INCLUDEFLAGS) -Wall -MD

LDFLAGS= -L$(LIBDIR) -lz

all: CFLAGS += -O2 -DNDEBUG
all: $(BUILDDIR)/$(PROJECT)

debug: CFLAGS += -DDEBUG -g
debug: $(BUILDDIR)/$(PROJECT)

leak-check: debug
	valgrind --leak-check=full --show-leak-kinds=all $(BUILDDIR)/$(PROJECT)

$(BUILDDIR)/$(PROJECT): $(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/%.o: $(TESTS)/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $< -o $@

libs:
	make -C $(LIBDIR) all

libclean:
	make -C $(LIBDIR) clean

clean:
	- rm -rf $(BUILDDIR)
	make -C $(LIBDIR) buildclean

-include $(OBJS:.o=.d)
