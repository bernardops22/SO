#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>

typedef struct {
  int tipo; // Tipo de Consulta: 1-Normal, 2-COVID19, 3-Urgente
  char descricao[100]; // Descrição da Consulta
  int pid_consulta; // PID do processo que quer fazer a consulta
} Consulta;

/* ---*/
Consulta nova_consulta(){
  Consulta c;
  c.pid_consulta=getpid();
  printf("\nInsira o tipo de consulta\n1 - Normal\n2 - COVID19\n3 - Urgente\nInserir: ");
  scanf("%d",&c.tipo);
  printf("Insira a descricao da consulta: ");
  scanf("%s",&c.descricao);
  printf("Consulta: %d, %s, %d\n", c.tipo, c.descricao, c.pid_consulta);
  return c;
}

void pedido_consulta(Consulta c){
  FILE* file = fopen( "PedidoConsulta.txt", "a");
  fprintf(file, "%d,%s,%d\n", c.tipo, c.descricao, c.pid_consulta);
  fclose(file);
}

void iniciar_consulta(){
  char pid_file[10];
  FILE* file = fopen( "SrvConsultas.pid", "r");
  if (file != NULL) {
		fgets(pid_file, 10, file);
		kill (atoi(pid_file), SIGUSR1);
		fclose(file);
  }
}  

/* ---*/
int n = 0;
void trata_sinal(int sinal){
  if (sinal == SIGUSR2){
    printf("Consulta nao e possivel para o processo %d.\n",getpid());
    remove("PedidoConsulta.txt");
    n=1;
  }
  if (sinal == SIGHUP){
    printf("Consulta iniciada para o processo %d.\n",getpid());
    remove("PedidoConsulta.txt");
    signal(SIGHUP,SIG_IGN);
    n=2;
    iniciar_consulta();
  }
  if (sinal == SIGTERM){
    if(n==2){
     printf("Consulta concluida para o processo %d.\n",getpid());
     n=1;
     }
    else perror("Erro");
  }
}

/* --- */
int main() {
  Consulta c;
  c = nova_consulta();
  pedido_consulta(c);
  signal (SIGUSR2, trata_sinal);
  signal (SIGHUP, trata_sinal);
  signal (SIGTERM, trata_sinal);
  while ( n != 1 )
		pause();
}