#include "Cliente.h"

#define PID getpid()

int main() {
  c = nova_consulta();
  pedido_consulta( c );
  signal ( SIGUSR2, trata_sinal );
  signal ( SIGHUP, trata_sinal );
  signal ( SIGTERM, trata_sinal );
  signal ( SIGALRM, trata_sinal );
  signal ( SIGINT, trata_sinal );
  while ( n != 1 )
		pause();
}

/* ---*/
Consulta nova_consulta(){
  c.pid_consulta = PID;
  printf( "\nInsira o tipo de consulta\n1 - Normal\n2 - COVID19\n3 - Urgente\nInserir: " );
  scanf( "%d", &c.tipo );
  printf( "Insira a descricao da consulta: " );
  scanf( "\n%99[^\n]", &c.descricao );
  //printf( "Consulta: %d, %s, %d\n", c.tipo, c.descricao, c.pid_consulta );
  return c;
}

void pedido_consulta(){
  FILE *file = fopen( "PedidoConsulta.txt", "r" );
  if ( file != NULL ){
    fclose( file );
    printf( "Erro: Tentado novamente. Por favor aguarde 10 segundos...\n" );
    alarm( 10 );
  }
  else{
    FILE *file = fopen( "PedidoConsulta.txt", "a" );
    fprintf( file, "%d\n%s\n%d", c.tipo, c.descricao, c.pid_consulta );
    fclose( file );
    get_srv_pid();
  }
}

void contactar_servidor( int srv_pid ){
  kill ( srv_pid, SIGUSR1 );
}

void get_srv_pid(){
  char srv_pid[10];
  FILE* file = fopen( "SrvConsultas.pid", "r" );
  if ( file != NULL ) {
    fgets( srv_pid, 10, file );
    contactar_servidor( atoi( srv_pid ) );
    fclose( file );
  }
  else{
    perror("Servidor inativo");
    exit(0);
  }
}

void trata_sinal( int sinal ){
  switch( sinal ){
   case SIGUSR2:
     printf( "Consulta nao e possivel para o processo %d.\n", PID );
     remove( "PedidoConsulta.txt" );
     n=1;
     break;
     
   case SIGHUP:
     printf( "Consulta iniciada para o processo %d.\n", PID );
     remove( "PedidoConsulta.txt" );
     signal ( SIGINT, SIG_IGN );
     n=2;
     break;
     
   case SIGTERM: 
     if( n==2 ){
      printf( "Consulta concluida para o processo %d.\n", PID );
      n=1;
      }
     else perror("");
     break;
     
   case SIGALRM:
     pedido_consulta( c );
     break;
     
   case SIGINT: 
     remove( "PedidoConsulta.txt" );
     printf( "\nPaciente cancelou o pedido\n" );
     n = 1;
     break;   
  }
}