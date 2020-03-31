PROJECT=openNV

rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2)$(filter $(subst *,%,$2),$d))

SOURCEDIR = ./src
BUILDDIR = ./build
LIBDIR = ./lib

FILES := $(call rwildcard,$(SOURCEDIR),*.c)
BUILDFILES := $(subst $(SOURCEDIR)/,$(BUILDDIR)/,$(FILES)) 

OBJS := $(BUILDFILES:.c=.o)

CC=clang
CFLAGS := -std=c99 -DLOG_USE_COLOR -I $(SOURCEDIR)

LDFLAGS= -L $(LIBDIR)

all: $(BUILDDIR)/$(PROJECT)

debug: CFLAGS += -DDEBUG
debug: $(BUILDDIR)/$(PROJECT)

$(BUILDDIR)/$(PROJECT): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	- rm -rf $(BUILDDIR)
