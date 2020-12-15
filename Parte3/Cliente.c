#include "Cliente.h"

int main () {
  iniciar_cliente ();
  nova_consulta ();
  pedido_consulta ();
  armar_SIGINT ();
  receber_mensagem ();
}

void iniciar_cliente (){
  system ( "clear" );
  printf ( "\n ----------Cliniq-IUL----------\n" );
  printf( " | Insira o tipo de consulta: |          A SUA SAUDE\n" );
  printf( " | 1 - Normal                 |      E O NOSSO OBJETIVO!\n" );
  printf( " | 2 - COVID19                |\n" );
  printf( " | 3 - Urgente                |          WE WANT IUL\n" );
  printf( " ------------------------------\n" );
  printf( "\n   + Escolha uma opcao: " );
}

void nova_consulta (){
  c.pid_consulta = PID;
  c.status = PEDIDO;
  scanf ( "%d", &c.tipo );
  if ( c.tipo < 1 || c.tipo > 3 ){ 
    printf ( " - Erro: Tipo de consulta invalido.\n\n" ); 
    exit ( 0 );
  }
  printf ( "   + Insira a descricao da consulta: " );
  scanf ( "\n%99[^\n]", &c.descricao );
}

void pedido_consulta (){
  mensagem m;
  mq_id = msgget ( KEY, 0 );
  exit_on_error ( mq_id, " - Message queue nao encontrada" );
  m.tipo = PEDIDO;
  snprintf ( m.texto, TAMANHOCONSULTA, "%d,%s,%d", c.tipo, c.descricao, c.pid_consulta );
  int status = msgsnd ( mq_id, &m, sizeof( m.texto ), 0 );                                                           //ENVIO PEDIDO
  exit_on_error ( status, " - Erro ao enviar o pedido de consulta" );
  printf ( "   + O pedido de consulta foi enviado.\n" );
}

void receber_mensagem (){
  while ( n != 1 ){
    mensagem m;
    int msg_status = msgrcv( mq_id, &m, sizeof ( m.texto ), PID , 0 );                                               //RECEBER INICIO
    if ( msg_status < 0 ) {
      if ( errno != EINTR ){
        printf(" - Erro ao esperar pela mensagem: %s\n", strerror(errno));
        n = 1;
      }
    } 
    else {
      tratar_mensagem( atoi ( m.texto ) );
    }
  }
}

void tratar_mensagem ( int m ){
  switch ( m ){
    case INICIADA:
     printf ( "   + Consulta iniciada para o processo %d.\n", PID );
      c.status = 2;
      n = 2;
      break;
    case TERMINADA:
      if ( n == 2 ){
        printf ( "   + Consulta concluida para o processo %d.\n\n", PID );
        c.status = 3;
        n = 1;
      }
      else printf ( " - Erro: A consulta ainda nao foi iniciada\n" );
      break;
    case RECUSADA:
      printf ( " - Consulta nao e possivel para o processo %d.\n\n", PID );
      c.status = 4;
      n = 1;
      break;
  }
}

void armar_SIGINT (){
  signal ( SIGINT, cancelar_pedido );
}

void cancelar_pedido (){
  printf ( "\n   + Paciente cancelou o pedido.\n" );
  mensagem m;
  m.tipo = PID;
  snprintf ( m.texto, sizeof ( CANCELADA ), "%d", CANCELADA );
  int status = msgsnd ( mq_id, &m, sizeof( m.texto ), 0 );                                                           //ENVIO CANCELAR
  exit_on_error ( status, " - Erro ao cancelar a consulta" );
  c.status = 5;
  n = 1;
}