#include "Cliente.h"

int main () {
  iniciar_cliente ();
  nova_consulta ();
  pedido_consulta ();
  armar_SIGINT ();
  receber_mensagens ();
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
  c.status = 1;
  scanf ( "%d", &c.tipo );
  if ( c.tipo < 1 || c.tipo > 3 ){ 
    printf ( " - Erro: Tipo de consulta invalido.\n\n" ); 
    exit ( 0 );
  }
  printf ( " + Insira a descricao da consulta: " );
  scanf ( "\n%99[^\n]", &c.descricao );
}

void pedido_consulta (){
  int id = msgget ( MSGKEY, 0 );
  exit_on_error ( id, " - Message queue nao encontrada" );
  int status = msgsnd ( id, &c, sizeof( c ) - sizeof( c.status ), 0 );
  exit_on_error ( status, "Erro ao enviar o pedido de consulta\n" );
  printf ( "O pedido de consulta foi enviado.\n" );
}

void receber_mensagens (){
  int status;
  int id = msgget ( PID, 0 );
  int mensagem = msgrcv(id, &status, sizeof( status ), PID, 0);
  exit_on_error(mensagem, "Erro ao receber mensagem do servidor");
  tratar_mensagem( status );
}

void tratar_mensagem ( int status ){
  switch ( status ){
    case 2:
      printf ( " + Consulta iniciada para o processo %d.\n", PID );
      n = 2;
      break;
    case 3:
      if ( n == 2 ){
        printf ( " + Consulta concluida para o processo %d.\n\n", PID );
        n = 1;
      }
      else printf ( " - Erro: A consulta ainda nao foi iniciada\n" );
      break;
    case 4:
      printf ( " - Consulta nao e possivel para o processo %d.\n\n", PID );
      n = 1;
      break;
  }
}

void armar_SIGINT (){
  signal ( SIGINT, trata_SIGINT );
}

void trata_SIGINT ( int sinal ){
  printf ( "\n - Paciente cancelou o pedido.\n\n" );
  //  Envia ao servidor dedicado uma mensagem com a indicação de consulta 5-Cancelada
  n = 1;
}