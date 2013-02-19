BIN := flim
SRCS := $(wildcard src/*.c)
OBJS := ${SRCS:.c=.o}

CFLAGS  += -g -Wall
LDFLAGS +=  -l mozjs185 -l portmidi
.PHONY: all clean distclean print

all: $(BIN)

$(BIN): $(OBJS)
	$(LINK.cc) $(OBJS) -o $(BIN)

clean:
	@- $(RM) $(BIN)
	@- $(RM) $(OBJS)
