CC = gcc
CFLAGS = -Wall -Wextra -g -Iheaders

# Sources 
SRCS = src/main.c histo/histo_handler.c histo/avl_plant.c histo/csv_parser.c leaks/leaks_handler.c

# Transform .c -> .o
OBJS = $(SRCS:.c=.o)

# Executable name
EXEC = wildwater

# Default rule
all: $(EXEC)

# Linking
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compile object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Cleaning
clean:
	rm -f $(OBJS) $(EXEC) *.dat *.png

.PHONY: all clean
