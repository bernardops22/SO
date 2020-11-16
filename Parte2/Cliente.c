#include "header.h"
#define PID getpid()
int n = 0;

/* ---*/
Consulta nova_consulta(){
  Consulta c;
  c.pid_consulta=getpid();
  printf( "\nInsira o tipo de consulta\n1 - Normal\n2 - COVID19\n3 - Urgente\nInserir: " );
  scanf( "%d", &c.tipo);
  printf( "Insira a descricao da consulta: " );
  scanf( "%s", &c.descricao );
  printf( "Consulta: %d, %s, %d\n", c.tipo, c.descricao, c.pid_consulta );
  return c;
}

void pedido_consulta( Consulta c ){
  FILE* file = fopen( "PedidoConsulta.txt", "a" );
  fprintf( file, "%d,%s,%d\n", c.tipo, c.descricao, c.pid_consulta );
  fclose( file );
}

/* ---*/
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
     signal( SIGHUP,SIG_IGN );
     n=2;
     break;
     
   case SIGTERM: 
     if( n==2 ){
      printf( "Consulta concluida para o processo %d.\n", PID );
      n=1;
      }
     else perror( "Erro" );
     break;
     
   case SIGALRM:
     printf( "Nao foi possivel contactar o servidor de consultas, tentando novamente.\n" );
     break;
     
   case SIGINT: 
     remove( "PedidoConsulta.txt" );
     printf( "\nVou terminar...\n" );
     n=1;
     break;   
  }
}

void get_srv_pid(){
  char pid_file[10];
  FILE* file = fopen( "SrvConsultas.pid", "r" );
  if ( file != NULL ) {
    fgets(pid_file, 10, file );
    kill ( atoi( pid_file ), SIGUSR1 );
    fclose( file );
  }
}

void contactar_servidor(){
  get_srv_pid();
  int i = 1;
  while ( i < 4 ){
    alarm( 5 );
    printf ( "\nA contactar o servidor.\nAguarde 5 segundos...\n" );
    sleep( 5 );
    i++;
  }
  printf( "\nNao foi possivel marcar consulta para o processo %d. Tente novamente mais tarde.\n\n", PID );
  remove( "PedidoConsulta.txt" ); 
  n = 1;
} 

/* --- */
int main() {
  Consulta c;
  c = nova_consulta();
  pedido_consulta( c );
  signal ( SIGUSR2, trata_sinal );
  signal ( SIGHUP, trata_sinal );
  signal ( SIGTERM, trata_sinal );
  signal ( SIGALRM, trata_sinal );
  signal ( SIGINT, trata_sinal );
  contactar_servidor();
  while ( n != 1 )
		pause();
}