#include <stdio.h>
#include <string.h>
#include "ligacao.h"

// Definição da memória e blocos livres
instruction memory[MEM_SIZE];
MemBlock free_blocks[100];
int num_free_blocks;

// Inicializa a memória: um único bloco livre de 0 a MEM_SIZE-1
void init_memoria() {
    free_blocks[0].start = 0;
    free_blocks[0].size = MEM_SIZE;
    num_free_blocks = 1;
}

// Alocação first-fit
int alocar_memoria(int tamanho, int *base) {
    for (int i = 0; i < num_free_blocks; i++) {
        if (free_blocks[i].size >= tamanho) {
            *base = free_blocks[i].start; //vai apontando para onde está na memoria.
            if (free_blocks[i].size == tamanho) {
                // remove bloco
                for (int j = i; j < num_free_blocks-1; j++)
                    free_blocks[j] = free_blocks[j+1];
                num_free_blocks--;
            } else {
                free_blocks[i].start += tamanho;
                free_blocks[i].size -= tamanho;
            }
            return 0;
        }
    }
    return -1;
}

// Liberta um bloco e coalesce
void libertar_memoria(int base, int tamanho) {
    // Adiciona o bloco
    free_blocks[num_free_blocks].start = base;
    free_blocks[num_free_blocks].size = tamanho;
    num_free_blocks++;
    
    // Ordena por start (bubble sort simples)
    for (int i = 0; i < num_free_blocks-1; i++) {
        for (int j = i+1; j < num_free_blocks; j++) {
            if (free_blocks[i].start > free_blocks[j].start) {
                MemBlock tmp = free_blocks[i];
                free_blocks[i] = free_blocks[j];
                free_blocks[j] = tmp;
            }
        }
    }
    
    // Coalesce (junta blocos adjacentes)
    for (int i = 0; i < num_free_blocks-1; i++) {
        if (free_blocks[i].start + free_blocks[i].size == free_blocks[i+1].start) {
            free_blocks[i].size += free_blocks[i+1].size;
            for (int j = i+1; j < num_free_blocks-1; j++)
                free_blocks[j] = free_blocks[j+1];
            num_free_blocks--;
            i--; // reavalia este i
        }
    }
}

void defragmentar() {
    printf("Defragmentação não implementada ainda\n");
}