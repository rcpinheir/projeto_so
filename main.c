#include <stdio.h>
#include "ligacao.h"

extern void init_memoria();
extern int alocar_memoria(int tamanho, int *base);
extern void libertar_memoria(int base, int tamanho);

int main() {

    init_memoria();   

    char nome_ficheiro[] = "p1.prg";
    int base, num_inst;
    if (carregar_programa(nome_ficheiro, &base, &num_inst) != 0) {
        printf("Erro ao carregar %s\n", nome_ficheiro);
        return 1;
    }

    // Criar processo para esse programa
    int pid = criar_processo(nome_ficheiro, 0, base, num_inst, 1, 100, 0, base);
    if (pid == -1) {
        printf("Erro: limite de processos\n");
        libertar_memoria(base, num_inst);
        return 1;
    }

    PCB *p = &tabela[0];

    int tempo=0;
    while (p->pc < p->base + p->num_instrucoes) { //base + instruçoes é o tamanho do processo
        instruction *inst = &memory[p->pc];   // aponta directamente para a instrução
    
        switch (inst->ins) {
            case 'M':
                p->variavel = inst->n;
                printf("t=%d: M %d -> variavel = %d\n", tempo, inst->n, p->variavel);
                break;
            case 'A':
                p->variavel += inst->n;
                printf("t=%d: A %d -> variavel = %d\n", tempo, inst->n, p->variavel);
                break;
            case 'S':
                p->variavel -= inst->n;
                printf("t=%d: S %d -> variavel = %d\n", tempo, inst->n, p->variavel);
                break;
            case 'B':
                p->state = BLOQUEADO;
                printf("t=%d: PID=%d B (bloqueado)\n", tempo, p->pid);
                break;
            case 'T':
                printf("t=%d: PID=%d terminado\n", tempo, p->pid);
                p->state = TERMINADO;
                break;
            default:
                printf("t=%d: Instrução %c ignorada (não implementada)\n", tempo, inst->ins);
        }

    p->pc++;
    tempo++;
}
    p->state = 2; 
    libertar_memoria(p->base, p->num_instrucoes);
return 0;
}