CC := g++
ROOT := $(shell pwd)
INCs := $(ROOT)/include/
CFLAGS_BASE := -I$(INCs) --std=c++2a -Wall

ifeq ($(NDEBUG),1)
	CFLAGS := $(CFLAGS_BASE) -DNDEBUG
else
	CFLAGS := $(CFLAGS_BASE) -g
endif

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ -c $<

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

clean:
	rm -rf .test

TESTDIR := $(ROOT)/.test
TESTSRCDIR := $(ROOT)/tests
TESTS := $(shell find $(TESTSRCDIR) -type f -name '*.cpp')
TESTEXEs := $(patsubst %.cpp,%,$(TESTS))
TESTEXEs := $(patsubst $(TESTSRCDIR)%,$(TESTDIR)%,$(TESTEXEs))

test: pretest $(TESTEXEs)

pretest:
	rm -rf $(TESTDIR)
	mkdir -p $(TESTDIR)


$(TESTDIR)/%: $(TESTSRCDIR)/%.cpp
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -o $@ $<
	bash -c $@

$(TESTDIR):
	mkdir -p $(ROOT)/.test
