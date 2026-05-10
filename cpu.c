#include <stdio.h>
#include <string.h>
#include "ligacao.h"


PCB tabela[MAX_PROCESSOS];
int num_processos = 0;



int carregar_programa(const char *nome_prg, int *base, int *num_inst) {
    FILE *f = fopen(nome_prg, "r");
    if (!f) return -1;

    char linha[256];
    int cont = 0;
    while (fgets(linha, sizeof(linha), f))
        if (linha[0] != '\n') cont++;
    fseek(f, 0, SEEK_SET);//conta quantas instruçoes tem e volta ao incicio do ficheiro

    if (alocar_memoria(cont, base) != 0) { fclose(f); return -2; }

    int idx = 0;
    while (fgets(linha, sizeof(linha), f) && idx < cont) {
        if (linha[0] == '\n') continue;
        linha[strcspn(linha, "\n")] = '\0';

        instruction *inst = &memory[*base + idx++];
        char op, nome[15];
        int n;

        if      (sscanf(linha, "%c %d", &op, &n) == 2)   { 
            inst->ins = op; inst->n = n; inst->nome[0] = '\0'; 
        }
        else if (sscanf(linha, "%c %s", &op, nome) == 2)  { 
            inst->ins = op; inst->n = 0; strcpy(inst->nome, nome); 
        }
        else { 
            inst->ins = op; inst->n = 0; inst->nome[0] = '\0'; }
    }
    *num_inst = cont;
    fclose(f);
    return 0;
}

int criar_processo(const char *nome, int ppid, int base, int num_inst,
                   int prioridade, int prazo, int variavel_inicial, int pc_inicial) {
    if (num_processos >= MAX_PROCESSOS) return -1;
    PCB *p = &tabela[num_processos];
    strcpy(p->nome, nome);
    p->pid = num_processos + 1;
    p->ppid = ppid;
    p->state = 0;
    p->base = base;
    p->pc = pc_inicial;
    p->num_instrucoes = num_inst;
    p->variavel = variavel_inicial;
    p->prioridade = prioridade;
    p->prazo = prazo;
    p->tempo_cpu = 0;
    num_processos++;
    return p->pid;
}