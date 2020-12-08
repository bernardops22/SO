#include "Servidor.h"

int main (){
  iniciar_servidor();
  limpar_lista_consultas ();
  armar_SIGINT ();
  while ( n != 1 ) receber_pedido ();
}

void iniciar_servidor (){
  system ( "clear" );
  printf( "\n + A iniciar o servidor" );
  for ( int i = 1; i < 6; i++ ){
    fflush( stdout );
    sleep ( 1 );
    printf ( "." );
  }
  mq_id = msgget ( KEY, IPC_CREAT | 0666 );
  printf ( "\n + Servidor iniciado com sucesso.\n" );
}

void limpar_lista_consultas (){
  shm_id = shmget ( KEY, NCONSULTAS * sizeof( Consulta ), IPC_CREAT | 0666 );
  exit_on_error ( shm_id, " - Erro ao criar uma zona de memoria partilhada" );
  Consulta* c = ( Consulta * ) shmat( shm_id, NULL, 0 );
  exit_on_null ( c, " - Erro ao ligar a memoria partilhada" );
  for ( int i = 0; i < NCONSULTAS; i++ )  c[i].tipo = -1;
  printf ( " + Memoria iniciada com sucesso\n + A aguardar pedidos de consulta.\n\n" );
  //FAZER shmdt aqui com addr NULL
}

void armar_SIGINT (){
  signal ( SIGINT, trata_SIGINT);
}

void trata_SIGINT ( int sinal ){
  desligar_servidor ();
}

void receber_pedido (){
  mensagem m;
  int status = msgrcv( mq_id, &m, sizeof( m.texto ), PEDIDO, 0);                  //RECEBER MENSAGEM
  //printf("%s\n", m.texto );
  exit_on_error( status, " - Erro ao receber pedido do cliente" );
  scanf( m.texto , "%d,%99[^,]%*c%d", &c.tipo, &c.descricao, &c.pid_consulta ); //NAO ESTA A RECEBER BEM
  printf ( " + Chegou novo pedido de consulta do tipo %d, descricao '%s' e PID [%d]\n", c.tipo, c.descricao, c.pid_consulta );
  tratar_pedido ();
}

void tratar_pedido () {
  pid_t parent = fork ();
  if ( !parent ){
    verificar_vagas ();
    //...
    
    exit ( 0 );
  }
  else  {
    //...
  }
}

void verificar_vagas (){
  Consulta* c = ( Consulta * ) shmat( shm_id, NULL, 0 );
  for (int i = 0; i < NCONSULTAS; i++ ){
    if ( c[i].tipo == -1 ){
      //inserir_consulta ();
      break;
    }
    printf ( "Lista de consultas cheia" );
    mensagem m;
    //m.tipo = c.pid_consulta;                                          //ENVIO DE MENSAGEM
    //sprintf ( m.texto, "%d", RECUSADA);
    //ENVIAR MENSAGEM
  }
}
/*
void inserir_consulta (){
  lista_consultas[indice_da_lista-1].tipo = c.tipo;
  strncpy ( lista_consultas[indice_da_lista-1].descricao, c.descricao, 99 );
  lista_consultas[indice_da_lista-1].pid_consulta = c.pid_consulta;    
  printf ( " + Consulta agendada para a sala %d\n", indice_da_lista );
  switch ( c.tipo ){
    case 1:
      cnormal++;
      break;
    case 2:
      ccovid19++;
      break;
    case 3:
      curgente++;
      break;
  }
  iniciar_consulta();
}

void temporizador(){
  printf ( " + Tempo restante: " );
  for ( int i = 10; i > 0; i-- ){
    fflush( stdout );
    printf ( "%d ", i );
    sleep ( 1 );
  }
  printf ( "\n" );
}*/

void desligar_servidor (){
  n = 1;
  printf ( "\n - Servidor encerrado.\n" );
}