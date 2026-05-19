

CC=cc
FLAGS=-c -Wall
LIBS=-lm
OBS=main.o cpu.o memoria.o gerir_prcss.o process.o #output.o

all: projeto_so

main.o: ligacao.h main.c
	$(CC) $(FLAGS) main.c

cpu.o: ligacao.h cpu.c
	$(CC) $(FLAGS) cpu.c

memoria.o: ligacao.h memoria.c
	$(CC) $(FLAGS) memoria.c

process.o: ligacao.h process.c
	$(CC) $(FLAGS) process.c

gerir_prcss.o: ligacao.h gerir_prcss.c
	$(CC) $(FLAGS) gerir_prcss.c

# output.o: ligacao.h output.c
# 	$(CC) $(FLAGS) output.c

clean limpar:
	rm -f projeto_so *.o
	rm -f *~
	echo "Remover: Ficheiros executáveis, objetos e temporários."

projeto_so: $(OBS)
	$(CC) -o projeto_so $(OBS) $(LIBS)