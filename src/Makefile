program_NAME := flimd
program_SRCS := $(wildcard *.c)
program_OBJS := ${program_SRCS:.c=.o}
program_INCLUDE_DIRS := lua/src/
program_LIBRARY_DIRS := lua/src/
program_LIBRARIES := portmidi lua

test_SRCS := $(wildcard ../tests/*.c)
test_OBJS := $(test_SRCS:.c=.o)

CFLAGS  += -g -Wall
LDFLAGS += $(foreach librarydir,$(program_LIBRARY_DIRS),-L$(librarydir))
LDFLAGS += $(foreach library,$(program_LIBRARIES),-l$(library))

FLIM_INSTALL=$(QUIET_INSTALL)($INSTALL)

PREFIX?=/usr/local
INSTALL_BIN=$(PREFIX)/bin

.PHONY: all clean distclean print

all: $(program_NAME)

test: $(program_OBJS) $(test_OBJS)
	$(LINK.cc) $(test_OBJS) $(filter-out main.o, $(program_OBJS)) -o ../bin/test
	../bin/test

lua/src/liblua.a:
	$(MAKE) macosx -C lua

$(program_NAME): $(program_OBJS) lua/src/liblua.a
	$(LINK.cc) $(program_OBJS) -o $(program_NAME)

clean:
	@- $(RM) $(program_NAME)
	@- $(RM) $(program_OBJS)
	@- $(RM) $(test_OBJS)
	@- $(RM) ../bin/test

cleanall:
	@- $(MAKE) clean
	@- cd lua && $(MAKE) clean

install: $(program_NAME)
	cp -pf $(program_NAME) $(INSTALL_BIN)
	cp -pf ../bin/flim $(INSTALL_BIN)
