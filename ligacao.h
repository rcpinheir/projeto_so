#define LIGACAO_H

//memoria, Cada posição guarda uma instrução
typedef struct { 
    char ins; //'M', 'A', 'S', 'C', 'L', 'T'
    int n; //M 20 -> n=20
    char nome[15]; 
} instruction;

instruction memory[1000];

//informaçoes do processo
typedef struct {
    int pc; //onde estamos na memoria
    int pid; //número de identificação do processo
    int ppid; //identificador do processo progenitor
    int base; //inicio da memoria
    int variavel;
    int prioridade; 
    int prazo; // tempo
    int num_instrucoes; // quantas tem
} Processo;

