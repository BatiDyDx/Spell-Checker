# Makefile para corrector ortografico

# Compilador y banderas
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99
BIN = main

# Archivos fuente
SOURCE = $(wildcard src/*.c)
SOURCE_O = $(SOURCE:c=o)

STRUCTURES = $(wildcard structures/*.c)
STRUCTURES_O = $(STRUCTURES:c=o)

TESTS = tests/main.c

# Compilar el programa
all: $(SOURCE_O) $(STRUCTURES_O)
	$(CC) $(CFLAGS) $^ utils.c -o $(BIN)

# Crear .o desde archivos .c
%.o: %.c
	$(CC) -c $(CFLAGS) $^ -o $@

# Compilar para depuracion
.PHONY: debug
debug:
	$(CC) -g $(CFLAGS) $(SOURCE) $(STRUCTURES) utils.c -o $(BIN)

# Remover archivos .o, ejecutables, etc
.PHONY: clean
clean:
	@-rm -r *.dSYM 2>/dev/null || true
	@-rm src/*.o structures/*.o 2>/dev/null || true
	@-rm $(BIN) 2>/dev/null || true 