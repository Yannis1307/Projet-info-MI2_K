CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11
SRC = src/main.c
OBJ = $(SRC:.c=.o)
BIN = wildwater

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(BIN)

.PHONY: all clean