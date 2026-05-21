#include <stdio.h>
#include <string.h>
#include "ligacao.h"



int tempo_global = 0;

typedef struct { char nome[256]; int tempo_chegada; int prioridade; int prazo; int lancado; } Pendente;
static Pendente pendentes[100];
static int num_pendentes = 0;

 void lancar_chegados() {
    for (int i = 0; i < num_pendentes; i++) {
        if (!pendentes[i].lancado && pendentes[i].tempo_chegada <= tempo_global) {
            int base, num_inst;
            if (carregar_programa(pendentes[i].nome, &base, &num_inst) == 0) {
                int pid = criar_processo(pendentes[i].nome, 0, base, num_inst,
                                        pendentes[i].prioridade, pendentes[i].prazo, 0, base);
                if (pid > 0) {
                    tabela[pid-1].tempo_chegada = pendentes[i].tempo_chegada;
                    printf("[t=%d] Processo %d: %s criado (prioridade=%d, prazo=%d)\n", 
                           tempo_global, pid, pendentes[i].nome, pendentes[i].prioridade, pendentes[i].prazo);
                }
                else libertar_memoria(base, num_inst);
            }
            pendentes[i].lancado = 1;
        }
    }
}

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
    // Ler programas a executar do plan.txt
    FILE *plan = fopen("plan.txt", "r");
    if (plan) {
        char pl[256];
        while (fgets(pl, sizeof(pl), plan) && num_pendentes < 100) {
            pl[strcspn(pl, "\n")] = '\0';
            if (pl[0] == '\0') continue;
            Pendente *p = &pendentes[num_pendentes];
            p->prioridade = 5; p->prazo = 100; p->lancado = 0;
            if (sscanf(pl, "%255s %d %d %d", p->nome, &p->tempo_chegada,
                       &p->prioridade, &p->prazo) >= 2)
                num_pendentes++;
        }
        fclose(plan);
    }

    if (num_pendentes == 0) {
        printf("Nenhum processo carregado do plan.txt.\n");
        return 0;
    }

    printf("=> %d processos carregados de 'plan.txt' para a lista de chegadas pendentes.\n", num_pendentes);
    printf("=> A aguardar comandos (E, I, D, R, T)...\n");

    // Lançar processos que chegam no tempo 0
    lancar_chegados();

    // Ler comandos do control.txt (se existir), senao do stdin
    FILE *cmd_file = fopen("control.txt", "r");
    if (!cmd_file) cmd_file = stdin;

    char cmd[256];
    while (fgets(cmd, sizeof(cmd), cmd_file)) {
        cmd[strcspn(cmd, "\n")] = '\0';
        if (cmd[0] == '\0' || cmd[0] == '#') continue;

        switch (cmd[0]) {
            case 'E': {
                int n = quantum;
                if (cmd[1] != '\0') sscanf(cmd + 1, "%d", &n);
                lancar_chegados();
                executa_por_tempo(n);
                lancar_chegados();
                break;
            }
            case 'I': {
                int idx = escalonador_curto_prazo();
                if (idx >= 0) tabela[idx].state = BLOQUEADO;
                break;
            }
            case 'D':
                desbloquear_processos();
                break;
            case 'R':
                relatorio();
                break;
            case 'T':
                estatisticas_globais();
                return 0;
            default:
                break;
        }
    }

    if (cmd_file != stdin) fclose(cmd_file);
    estatisticas_globais();

    return 0;
}