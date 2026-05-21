#define LIGACAO_H
#define MEM_SIZE 1000
#define MAX_PROCESSOS 50

#define PRONTO     0
#define EXEC       1
#define BLOQUEADO  2
#define TERMINADO  3

#define FCFS       0
#define SJF        1
#define PRIORIDADE 2
#define RM         3   
#define EDF        4 

extern int tempo_global;

typedef struct {
    char nome[256];       // nome do ficheiro .prg
    int pid;
    int ppid;
    int state;            // 0=pronto, 1=exec, 2=bloqueado, 3=terminado
    int base;             // endereço de início na memória (start)
    int pc;               // program counter (endereço actual)
    int num_instrucoes;
    int variavel;
    int prioridade;
    int prazo;            
    int tempo_cpu;
    int tempo_restante;       //  SJF 
    int deadline;             
    int periodo;              // RM/EDF 
    int tempo_chegada;        // para estatísticas
    int tempo_fim;         
} PCB;

extern PCB tabela[MAX_PROCESSOS];
extern int num_processos;
extern int algoritmo_escalonamento;   
extern int quantum;

typedef struct {
    int start;
    int size;
} MemBlock;

extern MemBlock free_blocks[100];
extern int num_free_blocks;

//memoria, Cada posição guarda uma instrução
typedef struct { 
    char ins; //'M', 'A', 'S', 'C', 'L', 'T'
    int n; //M 20 -> n=20
    char nome[15]; 
} instruction;

extern instruction memory[MEM_SIZE];   

//informaçoes do processo


void init_memoria();
int alocar_memoria(int tamanho, int *base);
void libertar_memoria(int base, int tamanho);
void defragmentar();

int carregar_programa(const char *nome_prg, int *base, int *num_inst);
int criar_processo(const char *nome, int ppid, int base, int num_inst,
                   int prioridade, int prazo, int variavel_inicial, int pc_inicial);

int executar_instrucao(PCB *p, int tempo);
void escalonar();

int escolher_fcfs();
int escolher_sjf();
int escolher_priority();
int escolher_rm();
int escolher_edf();
int escalonador_curto_prazo();

int clonar_processo(PCB *pai);
int substituir_programa(PCB *p, const char *nome_ficheiro);

int executar_processo(int idx, int quantum_exec, int tempo_inicio);

void desbloquear_processos();

void executa_por_tempo(int unidades);
void relatorio();
void estatisticas_globais();
void lancar_chegados();