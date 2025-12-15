CC = gcc
CFLAGS = -Wall -Wextra -g -Iheaders


SRCS = src/main.c Histo/histo_handler.c Histo/avl_usine.c Histo/csv_parser.c Leaks/leaks_handler.c

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

# Clean
clean:
	rm -f $(OBJS) $(EXEC) *.dat *.png

.PHONY: all clean
