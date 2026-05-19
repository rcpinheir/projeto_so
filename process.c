#include <stdio.h>
#include <string.h>
#include "ligacao.h"
#include <stdlib.h>


// Instrução C
int clonar_processo(PCB *pai) {
    int base_filho;
    
    if (alocar_memoria(pai->num_instrucoes, &base_filho) != 0) {
        return -2; 
    }

    //  Copia as instruções do pai para o espaço de memória do filho
    for (int i = 0; i < pai->num_instrucoes; i++) {
        memory[base_filho + i] = memory[pai->base + i];
    }

    // pc já aponta para a próxima instrução
    int pc_relativo = pai->pc - pai->base;
    int pc_inicial_filho = base_filho + pc_relativo;

    int pid_filho = criar_processo(
        pai->nome,          
        pai->pid,           
        base_filho,         
        pai->num_instrucoes,
        pai->prioridade,    
        pai->prazo,         
        pai->variavel,      
        pc_inicial_filho    
    );

    if (pid_filho == -1) {
        libertar_memoria(base_filho, pai->num_instrucoes);
        return -1; 
    }
    tabela[pid_filho - 1].state = PRONTO;
    return pid_filho;
}

// Instrução L
int substituir_programa(PCB *p, const char *nome_ficheiro) {

    char nome_completo[256];

    snprintf(nome_completo, sizeof(nome_completo),"%s.prg", nome_ficheiro);

    int nova_base, novas_instrucoes;
    
    if (carregar_programa(nome_ficheiro, &nova_base, &novas_instrucoes) != 0) {
        return -1; 
    }

    int base_antiga = p->base;
    int tamanho_antigo = p->num_instrucoes;

    strcpy(p->nome, nome_ficheiro);

    p->base = nova_base;
    p->pc = nova_base;
    p->num_instrucoes = novas_instrucoes;

    libertar_memoria(base_antiga, tamanho_antigo);
    p->num_instrucoes = novas_instrucoes;

    return 0; 
}