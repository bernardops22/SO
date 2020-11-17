#include "Cliente.h"

#define PID getpid()

int main() {
  //C1 e C2
  c = nova_consulta();
  //C2
  pedido_consulta( c );
  //C4
  signal ( SIGHUP, trata_sinal );
  //C5
  signal ( SIGTERM, trata_sinal );
  //C6
  signal ( SIGUSR2, trata_sinal );
  //C7
  signal ( SIGINT, trata_sinal );
  //C8
  signal ( SIGALRM, trata_sinal );
  while ( n != 1 )
		pause();
}

//C1 e C2
Consulta nova_consulta(){
  c.pid_consulta = PID;
  printf( "\nInsira o tipo de consulta\n1 - Normal\n2 - COVID19\n3 - Urgente\nInserir: " );
  scanf( "%d", &c.tipo );
  printf( "Insira a descricao da consulta: " );
  scanf( "\n%99[^\n]", &c.descricao );
  return c;
}

//C2
void pedido_consulta(){
  FILE *file = fopen( "PedidoConsulta.txt", "r" );
  //C8
  if ( file != NULL ){
    fclose( file );
    printf( "Erro: Tentado novamente. Por favor aguarde 10 segundos.\n" );
    alarm( 10 );
  }
  else{
    FILE *file = fopen( "PedidoConsulta.txt", "a" );
    fprintf( file, "%d\n%s\n%d", c.tipo, c.descricao, c.pid_consulta );
    fclose( file );
    get_srv_pid();
  }
}

//C3
void get_srv_pid(){
  char srv_pid[10];
  FILE* file = fopen( "SrvConsultas.pid", "r" );
  if ( file != NULL ) {
    fgets( srv_pid, 10, file );
    contactar_servidor( atoi( srv_pid ) );
    fclose( file );
  }
  else{
    perror( "Servidor inativo" );
    remove( "PedidoConsulta.txt" );
    exit(0);
  }
}

void contactar_servidor( int srv_pid ){
  kill ( srv_pid, SIGUSR1 );
}

void trata_sinal( int sinal ){
  switch( sinal ){
  
   //C4
   case SIGHUP:
     printf( "Consulta iniciada para o processo %d.\n", PID );
     remove( "PedidoConsulta.txt" );
     signal ( SIGINT, SIG_IGN );
     n=2;
     break;

   //C5  
   case SIGTERM: 
     if( n==2 ){
      printf( "Consulta concluida para o processo %d.\n", PID );
      n=1;
      }
     else perror("A consulta ainda nao foi iniciada");
     break;
   
   //C6
   case SIGUSR2:
     printf( "Consulta nao e possivel para o processo %d.\n", PID );
     remove( "PedidoConsulta.txt" );
     n=1;
     break;
   
   //C7
   case SIGINT: 
     remove( "PedidoConsulta.txt" );
     printf( "\nPaciente cancelou o pedido\n" );
     n = 1;
     break;
        
   //C8
   case SIGALRM:
     pedido_consulta( c );
     break;
  }
}