SRC=$(wildcard src/*.c)

INCLUDES?=
INCLUDES+=-I src

override CFLAGS?=-Wall
override CFLAGS+=$(INCLUDES)

OBJ=$(SRC:.c=.o)

BIN=\
	test

default: $(BIN)

$(BIN): $(OBJ) $(BIN:=.o)
	$(CC) $(CFLAGS) $(OBJ) $@.o -o $@

.PHONY: clean
clean:
	rm -f $(OBJ)
	rm -f $(BIN:=.o)
	rm -f $(BIN)
