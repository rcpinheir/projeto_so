#include <stdio.h>
#include <string.h>
#include "ligacao.h"


PCB tabela[MAX_PROCESSOS];
int num_processos = 0;
extern int quantum;

int executar_processo(int idx, int quantum_exec, int tempo_inicio) {

    PCB *p = &tabela[idx];

    p->state = EXEC;

    int executadas = 0;

    for (int i = 0; i < quantum_exec; i++) {

        if (executar_instrucao(p, tempo_inicio + i) == 0)
            break;

        executadas++;

        if (p->state == BLOQUEADO)
            break;
    }

    if (p->state == EXEC)
        p->state = PRONTO;

    return executadas;
}

void executa_por_tempo(int unidades) {
    int tempo_global = 0;  
    
    int restantes = unidades;

    while (restantes > 0) {
        int idx = escalonador_curto_prazo();
        if (idx == -1) {
            // nenhum processo pronto: idle
            printf("t=%d: CPU idle\n", tempo_global);
            tempo_global++;
            restantes--;
            continue;
        }

        int exec_units = (quantum < restantes) ? quantum : restantes;
        int executado = executar_processo(idx, exec_units, tempo_global);
        tempo_global += executado;
        restantes -= executado;
    }
}


int executar_instrucao(PCB *p, int tempo) {
    if (p->pc >= p->base + p->num_instrucoes) return 0;
    instruction *inst = &memory[p->pc];
    p->pc++;

    switch (inst->ins) {
        case 'M':
            p->variavel = inst->n;
            printf("t=%d: M %d -> var=%d\n", tempo, inst->n, p->variavel);
            break;
        case 'A':
            p->variavel += inst->n;
            printf("t=%d: A %d -> var=%d\n", tempo, inst->n, p->variavel);
            break;
        case 'S':
            p->variavel -= inst->n;
            printf("t=%d: S %d -> var=%d\n", tempo, inst->n, p->variavel);
            break;
        case 'B':
            p->state = BLOQUEADO;
            printf("t=%d: PID=%d bloqueado\n", tempo, p->pid);
            break;
        case 'T':
            printf("t=%d: PID=%d terminado\n", tempo, p->pid);
            p->state = TERMINADO;
            p->tempo_fim = tempo;
            libertar_memoria(p->base, p->num_instrucoes);
            return 0;
        case 'C': {
            int pid_filho = clonar_processo(p);

            if (pid_filho > 0) {
                printf("t=%d: PID=%d criou Processo Filho PID=%d (Fork)\n", tempo, p->pid, pid_filho);
                p->pc += inst->n; 
            } else if (pid_filho == -1) {
                printf("t=%d: Erro no Fork (Limite de processos atingido)\n", tempo);
            } else {
                printf("t=%d: Erro no Fork (Memória insuficiente)\n", tempo);
            }
            break;
        }

        case 'L': {
            printf("t=%d: PID=%d vai fazer Exec para '%s'\n", tempo, p->pid, inst->nome);
            
            if (substituir_programa(p, inst->nome) != 0) {
                printf("t=%d: Erro crítico ao fazer Exec do programa '%s' no PID=%d\n", tempo, inst->nome, p->pid);
                return 0; 
            }
            break;
        }
        default:
            printf("t=%d: Instrução %c ignorada\n", tempo, inst->ins);
    }

    p->tempo_cpu++;
    if (p->tempo_restante > 0)
    p->tempo_restante--;

    return 1;
}

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

        if (sscanf(linha, "%c %d", &op, &n) == 2) {

            inst->ins = op;
            inst->n = n;
            inst->nome[0] = '\0';
        }
        else if (sscanf(linha, "%c %s", &op, nome) == 2) {
        
            inst->ins = op;
            inst->n = 0;
            strcpy(inst->nome, nome);
        }
        else if (sscanf(linha, "%c", &op) == 1) {
        
            inst->ins = op;
            inst->n = 0;
            inst->nome[0] = '\0';
        }
        else {
        
            inst->ins = '?';
            inst->n = 0;
            inst->nome[0] = '\0';
        
            printf("Instrucao invalida: %s\n", linha);
        }
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

    p->tempo_restante = num_inst;   // se quiser guardar
    p->deadline = prazo;            // prazo absoluto (ajustar depois quando tivermos tempo de chegada)
    p->periodo = prazo;             // ou um valor específico lido do plan.txt
    p->tempo_chegada = 0;           // a ser preenchido pelo plan.txt
    p->tempo_fim = -1;

    num_processos++;
    return p->pid;
}