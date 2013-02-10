BIN := flim
SRCS := $(wildcard src/*.c)
OBJS := ${SRCS:.c=.o}

CFLAGS  += -g -Wall
LDFLAGS += -L js-1.8.5/js/src -l js_static -l portmidi
.PHONY: all clean distclean print

all: $(BIN)

$(BIN): $(OBJS)
	$(LINK.cc) $(OBJS) -o $(BIN)

clean:
	@- $(RM) $(BIN)
	@- $(RM) $(OBJS)
