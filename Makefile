CC := g++
ROOT := $(shell pwd)
INCs := $(ROOT)/include/
CFLAGS := -I$(INCs) --std=c++17
SRCDIR := $(ROOT)/src
BUILDDIR := $(ROOT)/.build
SRCs := $(shell find $(SRCDIR) -type f -name '*.cpp')
OBJs := $(patsubst %.cpp,%.o,$(SRCs))
OBJs := $(patsubst $(SRCDIR)%,$(BUILDDIR)%,$(OBJs))

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
	echo "Start testing"
	echo "tests: " $(TESTS)
	echo "testexes: " $(TESTEXEs)

pretest:
	rm -rf $(TESTDIR)
	mkdir -p $(TESTDIR)


$(TESTDIR)/%: $(TESTSRCDIR)/%.cpp $(OBJs)
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ $< $(OBJs)
	bash -c $@

$(TESTDIR):
	mkdir -p $(ROOT)/.test