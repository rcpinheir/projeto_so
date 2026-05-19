#include <stdio.h>
#include <string.h>
#include "ligacao.h"

extern void init_memoria();
extern void libertar_memoria(int base, int tamanho);

int main(int argc, char *argv[]) {
    init_memoria();

    if (argc > 1) {
        if (strcmp(argv[1], "fcfs") == 0) algoritmo_escalonamento = 0;
        else if (strcmp(argv[1], "sjf") == 0) algoritmo_escalonamento = 1;
        else if (strcmp(argv[1], "priority") == 0) algoritmo_escalonamento = 2;
        else if (strcmp(argv[1], "rm") == 0) algoritmo_escalonamento = 3;
        else if (strcmp(argv[1], "edf") == 0) algoritmo_escalonamento = 4;
        else {
            printf("Algoritmo desconhecido '%s', usando FCFS.\n", argv[1]);
            algoritmo_escalonamento = 0;
        }
    } else {
        // valor por omissão (pode ser FCFS ou outro)
        algoritmo_escalonamento = 0;
    }


    // Gestor (PID 0) lê control.txt e cria processos ----
    FILE *ctrl = fopen("control.txt", "r");
    if (!ctrl) {
        printf("control.txt não encontrado\n");
        return 1;
    }

    char linha[256];
    while (fgets(linha, sizeof(linha), ctrl) && num_processos < MAX_PROCESSOS) {
        linha[strcspn(linha, "\n")] = '\0';
        if (linha[0] == '\0') continue;

        char nome[256];
        int prio = 5, prazo = 100;
        sscanf(linha, "%s %d %d", nome, &prio, &prazo);

        int base, num_inst;
        if (carregar_programa(nome, &base, &num_inst) != 0) {
            printf("Erro ao carregar %s\n", nome);
            continue;
        }
        int pid = criar_processo(nome, 0, base, num_inst, prio, prazo, 0, base);
        if (pid == -1) {
            printf("Limite de processos\n");
            libertar_memoria(base, num_inst);
        } else {
            printf("Processo %d: %s (base=%d, %d instr)\n", pid, nome, base, num_inst);
        }
    }
    fclose(ctrl);

    if (num_processos == 0) {
        printf("Nenhum processo carregado.\n");
        return 0;
    }

    // round‑robin
    executa_por_tempo(100);

    return 0;
}