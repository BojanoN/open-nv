PROJECT=openNV

DIRS := ./src/util ./src/esm ./src/util ./src/sds ./src/logc ./src
SOURCEDIR = ./src
BUILDDIR = ./build
LIBDIR = ./lib
TESTS := ./src/tests


FILES := $(foreach dir,$(DIRS),$(wildcard $(dir)/*.c))
BUILDFILES := $(subst $(SOURCEDIR)/,$(BUILDDIR)/,$(FILES)) 
OBJS := $(BUILDFILES:.c=.o)

CC=clang
CFLAGS := -std=c99 -DLOG_USE_COLOR -I $(SOURCEDIR) -Wall

LDFLAGS= -L $(LIBDIR)

all: CFLAGS += -O2 -DNDEBUG
all: $(BUILDDIR)/$(PROJECT)

debug: CFLAGS += -DDEBUG -g
debug: $(BUILDDIR)/$(PROJECT)

leak-check: debug
	valgrind --leak-check=full --show-leak-kinds=all $(BUILDDIR)/$(PROJECT)

$(BUILDDIR)/$(PROJECT): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/%.o: $(TESTS)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $< -o $@

tests:
	make -C $(TESTS)

clean:
	- rm -rf $(BUILDDIR)
	make -C $(TESTS) clean
