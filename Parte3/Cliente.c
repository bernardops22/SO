#include "Cliente.h"

int main () {
  iniciar_cliente ();
  nova_consulta ();
  pedido_consulta ();
  armar_SIGINT ();
  while ( n != 1 ) receber_mensagens();
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
  c.status = PEDIDO;
  scanf ( "%d", &c.tipo );
  if ( c.tipo < 1 || c.tipo > 3 ){ 
    printf ( " - Erro: Tipo de consulta invalido.\n\n" ); 
    exit ( 0 );
  }
  printf ( " + Insira a descricao da consulta: " );
  scanf ( "\n%99[^\n]", &c.descricao );
}

void pedido_consulta (){
  int mq_id = msgget ( KEY, 0 );
  exit_on_error ( mq_id, " - Message queue nao encontrada" );
  mensagem m;
  m.tipo = PEDIDO;
  snprintf ( m.texto, TAMANHOCONSULTA, "%d,%s,%d", c.tipo, c.descricao, c.pid_consulta );
  int status = msgsnd ( mq_id, &m, sizeof( m.texto ), 0 );                        //ENVIO DE MENSAGEM
  exit_on_error ( status, " - Erro ao enviar o pedido de consulta" );
  printf ( " + O pedido de consulta foi enviado.\n" );
}

void receber_mensagens (){
  mensagem m;
  int status = msgrcv(mq_id, &m, sizeof( m.texto ), PID, 0);                        //RECEBER MENSAGEM
  exit_on_error( status, " - Erro ao receber mensagem do servidor");
  tratar_mensagem( m.texto );
}

void tratar_mensagem ( int m ){
  switch ( m ){
    case INICIADA:
      printf ( " + Consulta iniciada para o processo %d.\n", PID );
      n = 2;
      break;
    case TERMINADA:
      if ( n == 2 ){
        printf ( " + Consulta concluida para o processo %d.\n\n", PID );
        n = 1;
      }
      else printf ( " - Erro: A consulta ainda nao foi iniciada\n" );
      break;
    case RECUSADA:
      printf ( " - Consulta nao e possivel para o processo %d.\n\n", PID );
      n = 1;
      break;
  }
}

void armar_SIGINT (){
  signal ( SIGINT, trata_SIGINT );
}

void trata_SIGINT (){
  mensagem m;
  m.tipo = PID;
  sprintf ( m.texto, "%d", CANCELADA);
  int status = msgsnd ( mq_id, &m, sizeof( m.texto ), 0 );                        //ENVIO DE MENSAGEM
  exit_on_error ( status, " - Erro ao cancelar a consulta" );
  printf ( "\n - Paciente cancelou o pedido.\n" );
  n = 1;
}