include ../config

ESMSRC = $(wildcard esm/*.cpp) $(wildcard esm/*/*.cpp)
ESMSRC = $(wildcard esm/*.cpp) $(wildcard esm/subrecords/*.cpp)
BSASRC = $(wildcard bsa/*.cpp)
NIFSRC = $(wildcard nif/*.cpp) $(wildcard nif/main/*.cpp)
GAMESRC = $(wildcard *.cpp)
STREAMSRC = $(wildcard streams/*stream.cpp) $(wildcard streams/*/*stream.cpp) 
UTILSRC = $(wildcard util/*.cpp) $(wildcard logc/*.cpp)
FILESRC = $(wildcard file/*.cpp) $(wildcard providers/fileprovider.cpp)

BSAOBJS = $(patsubst %, $(BUILDDIR)/%, $(BSASRC:.cpp=.o))
ESMOBJS = $(patsubst %, $(BUILDDIR)/%, $(ESMSRC:.cpp=.o))
NIFOBJS = $(patsubst %, $(BUILDDIR)/%, $(NIFSRC:.cpp=.o))
STREAMOBJS = $(patsubst %, $(BUILDDIR)/%, $(STREAMSRC:.cpp=.o))
FILEOBJS = $(patsubst %, $(BUILDDIR)/%, $(FILESRC:.cpp=.o))
UTILOBJS = $(patsubst %, $(BUILDDIR)/%, $(UTILSRC:.cpp=.o))
GAMEOBJS = $(patsubst %, $(BUILDDIR)/%, $(GAMESRC:.cpp=.o))

export

all: CFLAGS += -O2 -DNDEBUG
all: $(BUILDDIR)/$(PROJECT)

debug: CFLAGS += -DDEBUG -g #-pg
#debug: LDFLAGS += -pg
debug: $(BUILDDIR)/$(PROJECT)


bsatest: CFLAGS += -DDEBUG -g
bsatest: $(BUILDDIR)/bsatest


$(BUILDDIR)/bsatest: $(BSAOBJS) $(UTILOBJS) $(BUILDDIR)/tests/bsatest.o
	$(CC) $^ -o $@ $(LDFLAGS)


$(BUILDDIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@


$(BUILDDIR)/$(PROJECT): $(ESMOBJS) $(UTILOBJS) $(GAMEOBJS) $(STREAMOBJS) $(FILEOBJS) $(NIFOBJS) $(BSAOBJS) $(BUILDDIR)/$(PROJECT).o
	$(CC) $^ -o $@ $(LDFLAGS)
