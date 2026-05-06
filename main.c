#include <stdio.h>
#include "ligacao.h"

int main() {

    memory[0].ins = 'M'; memory[0].n = 10;
    memory[1].ins = 'A'; memory[1].n = 5;
    memory[2].ins = 'S'; memory[2].n = 2;
    memory[3].ins = 'M'; memory[3].n = 100;

    int base = 0;
    int num_inst = 4;

    Processo p;
    p.pid = 1;
    p.ppid = 0;
    p.base = base;
    p.pc = base;
    p.num_instrucoes = num_inst;
    p.variavel = 0;
    p.prioridade = 1;
    p.prazo = 100;

    int tempo=0;
    while (p.pc < p.base + p.num_instrucoes) { //base + instruçoes é o tamanho do processo
        instruction *inst = &memory[p.pc];   // aponta directamente para a instrução
    
        switch (inst->ins) {
            case 'M':
                p.variavel = inst->n;
                printf("t=%d: M %d -> variavel = %d\n", tempo, inst->n, p.variavel);
                break;
            case 'A':
                p.variavel += inst->n;
                printf("t=%d: A %d -> variavel = %d\n", tempo, inst->n, p.variavel);
                break;
            case 'S':
                p.variavel -= inst->n;
                printf("t=%d: S %d -> variavel = %d\n", tempo, inst->n, p.variavel);
                break;
            default:
                printf("t=%d: Instrução %c ignorada (não implementada)\n", tempo, inst->ins);
        }

    p.pc++;
    tempo++;
}return 0;
}