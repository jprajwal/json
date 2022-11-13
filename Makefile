CC := g++
ROOT := $(shell pwd)
INCs := $(ROOT)/include/
SRCDIR := $(ROOT)/src
BUILDDIR := $(ROOT)/.build
SRCs := $(shell find $(SRCDIR) -type f -name '*.cpp')
OBJs := $(patsubst %.cpp,%.o,$(SRCs))
OBJs := $(patsubst $(SRCDIR)%,$(BUILDDIR)%,$(OBJs))
CFLAGS_BASE := -I$(INCs) --std=c++17 -Wall

ifeq ($(NDEBUG),1)
	CFLAGS := $(CFLAGS_BASE) -DNDEBUG
else
	CFLAGS := $(CFLAGS_BASE) -g
endif

build: $(OBJs) | $(BUILDDIR)
	$(CC) $(CFLAGS) -o test $?

# $(OBJs): $(SRCs) | $(BUILDDIR)
# 	$(CC) $(CFLAGS) -o $@ -c $<

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ -c $<

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

clean:
	rm -rf .build .test

TESTDIR := $(ROOT)/.test
TESTSRCDIR := $(ROOT)/tests
TESTS := $(shell find $(TESTSRCDIR) -type f -name '*.cpp')
TESTEXEs := $(patsubst %.cpp,%,$(TESTS))
TESTEXEs := $(patsubst $(TESTSRCDIR)%,$(TESTDIR)%,$(TESTEXEs))

test: pretest $(TESTEXEs)

pretest:
	rm -rf $(TESTDIR)
	mkdir -p $(TESTDIR)


$(TESTDIR)/%: $(TESTSRCDIR)/%.cpp $(OBJs)
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ $< $(OBJs)
	bash -c $@

$(TESTDIR):
	mkdir -p $(ROOT)/.test
