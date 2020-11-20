#include "Cliente.h"

int main () {
  iniciar_cliente ();
  nova_consulta ();
  pedido_consulta ();
  armar_sinal ();
  while ( n != 1 ) pause ();
}

void iniciar_cliente (){
  system ( "clear" );
  printf ( "\n ----------Cliniq-IUL----------\n" );
  printf( " | Insira o tipo de consulta: |          A SUA SAUDE\n" );
  printf( " | 1 - Normal                 |      E O NOSSO OBJETIVO!\n" );
  printf( " | 2 - COVID19                |\n" );
  printf( " | 3 - Urgente                |          WE WANT IUL\n" );
  printf( " ------------------------------\n" );
  printf( " + Escolha uma opcao: " );
}

void nova_consulta (){
  c.pid_consulta = PID;
  scanf ( "%d", &c.tipo );
  if ( c.tipo < 1 || c.tipo > 3 ){ 
    printf ( " - Erro: Tipo de consulta invalido.\n\n" ); 
    exit ( 0 );
  }
  printf ( " + Insira a descricao da consulta: " );
  scanf ( "\n%99[^\n]", &c.descricao );
}

void pedido_consulta (){
  FILE *file = fopen ( PEDIDO_CONSULTA, "r" );
  if ( file != NULL ){
    fclose ( file );
    printf ( " - Erro: Tentado novamente. Por favor aguarde 10 segundos.\n" );
    alarm ( 10 );
  }
  else{
    FILE *file = fopen ( PEDIDO_CONSULTA, "a" );
    fprintf ( file, "%d\n%s\n%d", c.tipo, c.descricao, c.pid_consulta );
    fclose ( file );
    get_srv_pid ();
  }
}

void get_srv_pid (){
  char temp[11];
  FILE* file = fopen ( SERVIDOR_PID, "r" );
  if ( file != NULL ) {
    fgets ( temp, 10, file );
    contactar_servidor ( atoi( temp ) );
    fclose ( file );
  }
  else{
    perror ( " - Erro: Servidor inativo" );
    printf( "\n" );
    remove ( PEDIDO_CONSULTA );
    exit (0);
  }
}

void contactar_servidor ( int srv_pid ){
  kill ( srv_pid, SIGUSR1 );
}

void armar_sinal (){
  signal ( SIGHUP, trata_sinal );
  signal ( SIGTERM, trata_sinal );
  signal ( SIGUSR2, trata_sinal );
  signal ( SIGINT, trata_sinal );
  signal ( SIGALRM, trata_sinal );
}

void trata_sinal ( int sinal ){
  switch ( sinal ){
   case SIGHUP:
     printf ( " + Consulta iniciada para o processo %d.\n", PID );
     remove ( PEDIDO_CONSULTA );
     n = 2;
     break; 
   case SIGTERM: 
     if ( n==2 ){
      printf ( " + Consulta concluida para o processo %d.\n\n", PID );
      n = 1;
     }
     else perror (" - Erro: A consulta ainda nao foi iniciada");
     break;
   case SIGUSR2:
     printf ( " - Consulta nao e possivel para o processo %d.\n\n", PID );
     remove ( PEDIDO_CONSULTA );
     n = 1;
     break;
   case SIGINT: 
     remove ( PEDIDO_CONSULTA );
     printf ( "\n - Paciente cancelou o pedido.\n\n" );
     n = 1;
     break; 
   case SIGALRM:
     pedido_consulta ();
     break;
  }
}