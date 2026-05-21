#include <stdio.h>
#include "ligacao.h"

void relatorio() {
    printf("\nTEMPO ACTUAL: %d\n", tempo_global);

    printf("\nPROCESSO EM EXECUCAO:\n");
    for (int i = 0; i < num_processos; i++) {
        PCB *p = &tabela[i];
        if (p->state == EXEC)
            printf("  pid=%d ppid=%d prio=%d var=%d cpu=%d\n",
                   p->pid, p->ppid, p->prioridade, p->variavel, p->tempo_cpu);
    }

    printf("\nPROCESSOS BLOQUEADOS:\n");
    for (int i = 0; i < num_processos; i++) {
        PCB *p = &tabela[i];
        if (p->state == BLOQUEADO)
            printf("  pid=%d ppid=%d prio=%d var=%d cpu=%d\n",
                   p->pid, p->ppid, p->prioridade, p->variavel, p->tempo_cpu);
    }

    printf("\nPROCESSOS PRONTOS:\n");
    for (int i = 0; i < num_processos; i++) {
        PCB *p = &tabela[i];
        if (p->state == PRONTO)
            printf("  pid=%d ppid=%d prio=%d var=%d cpu=%d\n",
                   p->pid, p->ppid, p->prioridade, p->variavel, p->tempo_cpu);
    }

    printf("\nPROCESSOS TERMINADOS:\n");
    for (int i = 0; i < num_processos; i++) {
        PCB *p = &tabela[i];
        if (p->state == TERMINADO)
            printf("  pid=%d ppid=%d prio=%d var=%d cpu=%d fim=%d\n",
                   p->pid, p->ppid, p->prioridade, p->variavel, p->tempo_cpu, p->tempo_fim);
    }
    printf("\n");
}

void estatisticas_globais() {
    printf("\nESTATISTICAS GLOBAIS t=%d\n", tempo_global);
    printf("  %-5s %-20s %-8s %-8s %-6s %-10s\n",
           "PID", "Nome", "Chegada", "Fim", "CPU", "Turnaround");

    int total = 0, n = 0;
    for (int i = 0; i < num_processos; i++) {
        PCB *p = &tabela[i];
        if (p->state == TERMINADO && p->tempo_fim >= 0) {
            int ta = p->tempo_fim - p->tempo_chegada;
            printf("  %-5d %-20s %-8d %-8d %-6d %-10d\n",
                   p->pid, p->nome, p->tempo_chegada, p->tempo_fim, p->tempo_cpu, ta);
            total += ta;
            n++;
        } else {
            printf("  %-5d %-20s %-8d %-8s %-6d %-10s\n",
                   p->pid, p->nome, p->tempo_chegada, "-", p->tempo_cpu, "-");
        }
    }
    if (n > 0)
        printf("Turnaround medio: %.2f\n", (double)total / n);
    printf("\n");
}
