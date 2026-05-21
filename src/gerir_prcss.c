#include "ligacao.h"
#include <stdio.h>
#include <limits.h>

int algoritmo_escalonamento = 0;
int quantum = 5;

int escolher_fcfs() {
    int escolhido = -1;
    for (int i = 0; i < num_processos; i++) {
        if (tabela[i].state == PRONTO) {
            if (escolhido == -1 || tabela[i].tempo_chegada < tabela[escolhido].tempo_chegada)
                escolhido = i;
        }
    }
    return escolhido;
}

int escolher_sjf() {
    int escolhido = -1;
    int menor_restante = INT_MAX;
    for (int i = 0; i < num_processos; i++) {
        if (tabela[i].state == PRONTO) {
            int restante = tabela[i].tempo_restante;
            if (restante < menor_restante) {
                menor_restante = restante;
                escolhido = i;
            }
        }
    }
    return escolhido;
}

int escolher_priority() {
    int escolhido = -1;
    int maior_prioridade = INT_MAX;  // porque menor valor = mais prioridade
    for (int i = 0; i < num_processos; i++) {
        if (tabela[i].state == PRONTO) {
            if (tabela[i].prioridade < maior_prioridade) {
                maior_prioridade = tabela[i].prioridade;
                escolhido = i;
            }
        }
    }
    return escolhido;
}

int escolher_rm() {
    int escolhido = -1;
    int menor_periodo = INT_MAX;
    for (int i = 0; i < num_processos; i++) {
        if (tabela[i].state == PRONTO) {
            if (tabela[i].periodo < menor_periodo) {
                menor_periodo = tabela[i].periodo;
                escolhido = i;
            }
        }
    }
    return escolhido;
}

int escolher_edf() {
    int escolhido = -1;
    int menor_deadline = INT_MAX;
    for (int i = 0; i < num_processos; i++) {
        if (tabela[i].state == PRONTO) {
            if (tabela[i].deadline < menor_deadline) {
                menor_deadline = tabela[i].deadline;
                escolhido = i;
            }
        }
    }
    return escolhido;
}

int escalonador_curto_prazo() {
    switch (algoritmo_escalonamento) {
        case 0: return escolher_fcfs();
        case 1: return escolher_sjf();
        case 2: return escolher_priority();
        case 3: return escolher_rm();
        case 4: return escolher_edf();
        default: return escolher_fcfs();
    }
}

