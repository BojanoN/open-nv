PROJECT=openNV

DIRS := ./src/esm ./src/ ./src/logc
SOURCEDIR = ./src/
BUILDDIR = ./build
LIBDIR = ./lib
TESTS := ./src/tests


FILES := $(foreach dir,$(DIRS),$(wildcard $(dir)/*.cpp))
BUILDFILES := $(subst $(SOURCEDIR)/,$(BUILDDIR)/,$(FILES)) 
OBJS := $(BUILDFILES:.cpp=.o)

CC=clang++
CFLAGS := --std=c++17 --stdlib=libc++ -DLOG_USE_COLOR -I $(SOURCEDIR) -Wall -MD

LDFLAGS= -L $(LIBDIR) -lc++ -lc++abi

all: CFLAGS += -O2 -DNDEBUG
all: $(BUILDDIR)/$(PROJECT)

debug: CFLAGS += -DDEBUG -g
debug: $(BUILDDIR)/$(PROJECT)

leak-check: debug
	valgrind --leak-check=full --show-leak-kinds=all $(BUILDDIR)/$(PROJECT)

$(BUILDDIR)/$(PROJECT): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/%.o: $(TESTS)/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $< -o $@

tests:
	make -C $(TESTS)

clean:
	- rm -rf $(BUILDDIR)
	make -C $(TESTS) clean

-include $(OBJS:.o=.d)
