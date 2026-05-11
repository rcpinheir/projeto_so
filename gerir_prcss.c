1.  #include <stdio.h>
2.  #include <string.h>
3.  #include <stdlib.h>
4.  #include "pcb.h"
5.
6.  PCB pcb_table[MAX_PROCESSES];
7.  int num_processes = 0;
8.  int current_time  = 0;
9.  int running       = -1;   // -1 = nenhum processo a correr
10.
11. int create_process(char *filename, int arrival, int priority, int ppid){
12.     if(num_processes >= MAX_PROCESSES) return -1;
13.     int slot = find_free_slot(100);
14.     if(slot == -1){
15.         defrag_memory();
16.         slot = find_free_slot(100);
17.         if(slot == -1) return -1; }
18.     int size = load_program(filename, slot);
19.     if(size == -1) return -1;
20.     int idx = num_processes;
21.     pcb_table[idx].pid          = idx + 1;
22.     pcb_table[idx].ppid         = ppid;
23.     pcb_table[idx].priority     = priority;
24.     pcb_table[idx].pc           = slot;
25.     pcb_table[idx].start        = slot;
26.     pcb_table[idx].size         = size;
27.     pcb_table[idx].variable     = 0;
28.     pcb_table[idx].arrival_time = arrival;
29.     pcb_table[idx].start_time   = -1;
30.     pcb_table[idx].end_time     = -1;
31.     pcb_table[idx].cpu_time     = 0;
32.     pcb_table[idx].state        = READY;
33.     strncpy(pcb_table[idx].program, filename, MAX_FILENAME);
34.     num_processes++;
35.     return idx; }
36.
37. void fork_process(int parent_idx, int jump){  // instrucao C n
38.     if(num_processes >= MAX_PROCESSES) return;
39.     PCB *pai = &pcb_table[parent_idx];
40.     int  idx = num_processes;
41.     pcb_table[idx]            = *pai;
42.     pcb_table[idx].pid        = idx + 1;
43.     pcb_table[idx].ppid       = pai->pid;
44.     pcb_table[idx].state      = READY;
45.     pcb_table[idx].cpu_time   = 0;
46.     pcb_table[idx].start_time = -1;
47.     pcb_table[idx].end_time   = -1;
48.     pcb_table[idx].pc         = pai->pc + 1;  // filho continua apos C
49.     pai->pc += jump;                           // pai salta n instrucoes
50.     num_processes++; }
51.
52. void load_new_program(int idx, char *filename){  // instrucao L filename
53.     PCB *p = &pcb_table[idx];
54.     free_program(p->start, p->size);
55.     int slot = find_free_slot(100);
56.     if(slot == -1){
57.         defrag_memory();
58.         slot = find_free_slot(100);
59.         if(slot == -1) return; }
60.     int size = load_program(filename, slot);
61.     if(size == -1) return;
62.     p->start = slot;
63.     p->pc    = slot;
64.     p->size  = size;
65.     strncpy(p->program, filename, MAX_FILENAME); }
66.
67. void execute(int quantum){
68.     if(running == -1) return;
69.     PCB *p = &pcb_table[running];
70.     if(p->start_time == -1)
71.         p->start_time = current_time;
72.     for(int t = 0; t < quantum; t++){
73.         if(running == -1) return;
74.         if(p->pc >= p->start + p->size){
75.             p->state    = TERMINATED;
76.             p->end_time = current_time;
77.             free_program(p->start, p->size);
78.             running = -1;
79.             return; }
80.         Instruction inst = memory[p->pc];
81.         switch(inst.ins){
82.             case 'M': p->variable  = inst.n;  break;
83.             case 'A': p->variable += inst.n;  break;
84.             case 'S': p->variable -= inst.n;  break;
85.             case 'B': p->state = BLOCKED;
86.                       running  = -1;
87.                       current_time++;
88.                       return;
89.             case 'T': p->state    = TERMINATED;
90.                       p->end_time = current_time;
91.                       free_program(p->start, p->size);
92.                       running = -1;
93.                       current_time++;
94.                       return;
95.             case 'C': fork_process(running, inst.n);         break;
96.             case 'L': load_new_program(running, inst.nome);
97.                       p->cpu_time++;
98.                       current_time++;
99.                       return; }
100.        p->pc++;
101.        p->cpu_time++;
102.        current_time++; } }
103.
104. void load_plan(char *filename){
105.     FILE *f = fopen(filename, "r");
106.     if(!f) return;
107.     char prog[MAX_FILENAME];
108.     int  arrival;
109.     while(fscanf(f, "%s %d", prog, &arrival) != EOF)
110.         create_process(prog, arrival, 1, 0);
111.     fclose(f); }
112.
113. void check_arrivals(void){
114.     for(int i = 0; i < num_processes; i++)
115.         if(pcb_table[i].arrival_time <= current_time &&
116.            pcb_table[i].state        == READY        &&
117.            pcb_table[i].start_time   == -1)
118.             printf("[t=%d] PID:%d pronto\n",
119.                    current_time, pcb_table[i].pid); }