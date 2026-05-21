CC=cc
CFLAGS=-Wall -Iinclude
LIBS=-lm
SRC=src/main.c src/cpu.c src/memoria.c src/gerir_prcss.c src/process.c src/output.c
OBS=$(SRC:src/%.c=%.o)

all: projeto_so

%.o: src/%.c include/ligacao.h
	$(CC) $(CFLAGS) -c $< -o $@

projeto_so: $(OBS)
	$(CC) -o projeto_so $(OBS) $(LIBS)

clean limpar:
	rm -f projeto_so $(OBS)
	rm -f *~
	@echo "Removidos: executavel, objetos e temporarios."

.PHONY: all clean limpar
