CC = gcc
CFLAGS = -Wall -Wextra -g -Iheaders


SRCS = src/main.c histo/histo_handler.c histo/avl_usine.c histo/csv_parser.c

# Transformation .c -> .o
OBJS = $(SRCS:.c=.o)

# Nom de l'exécutable 
EXEC = wildwater

# Règle par défaut
all: $(EXEC)

# Édition des liens
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compilation des fichiers objets
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Cleaning
clean:
	rm -f $(OBJS) $(EXEC) *.dat *.png

.PHONY: all clean
