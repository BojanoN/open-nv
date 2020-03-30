PROJECT=openNV

rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2)$(filter $(subst *,%,$2),$d))

SOURCEDIR = ./src
BUILDDIR = ./build
LIBDIR = ./lib

FILES := $(call rwildcard,$(SOURCEDIR),*.cpp)
BUILDFILES := $(subst $(SOURCEDIR)/,$(BUILDDIR)/,$(FILES)) 

OBJS := $(BUILDFILES:.cpp=.o)

CXX=g++
CXXFLAGS := --std=c++17 -I $(SOURCEDIR)

LDFLAGS= -L $(LIBDIR) -lspdlog

all: $(BUILDDIR)/$(PROJECT)

debug: CXXFLAGS += -DDEBUG
debug: $(BUILDDIR)/$(PROJECT)

$(BUILDDIR)/$(PROJECT): $(OBJS)
	$(CXX) $(LDFLAGS) $^ -o $@

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@


clean:
	- rm -rf $(BUILDDIR)
