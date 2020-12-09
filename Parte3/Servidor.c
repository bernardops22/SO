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
  sem_id = semget( KEY, 1, IPC_CREAT | 0666 );
  exit_on_error(sem_id, " - Erro ao criar o semaforo");                                  //CRIAR SEMAFORO
  mq_id = msgget ( KEY, IPC_CREAT | 0666 );
  exit_on_error(mq_id, " - Erro ao criar a fila de mensagens");                          //CRIAR MESSAGE QUEUE
  printf ( "\n + Servidor iniciado com sucesso.\n" );
}

void limpar_lista_consultas (){
  //IF STATMENT CASO A SHM EXISTA
  //COLOCAR TAMBEM OS CONTADORES NA SHM
  int status = semctl(sem_id, 0, SETVAL, 0);                                                  // SEM = 0
  exit_on_error(status, " - Erro ao alterar valor do semaforo");
  shm_id = shmget ( KEY, NCONSULTAS * sizeof( Consulta ), IPC_EXCL | IPC_CREAT | 0666 );        //CRIAR SHARED MEMORY
  exit_on_error ( shm_id, " - Erro ao aceder a zona de memoria partilhada" );
  Consulta* mem = ( Consulta * ) shmat( shm_id, NULL, 0 );                                      //FAZER dt
  exit_on_null ( mem, " - Erro ao ligar a memoria partilhada" );
  for ( int i = 0; i < NCONSULTAS; i++ )  mem[i].tipo = -1;
  status = semctl(sem_id, 0, SETVAL, 1);                                                      // SEM = 1
  exit_on_error(status, " - Erro ao alterar valor do semaforo");
  printf ( " + Memoria iniciada com sucesso\n + A aguardar pedidos de consulta.\n\n" );
  //shmdt ( shm_id, NULL, 0);
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
  //INICIAR SEMAFORO
  Consulta* mem = ( Consulta * ) shmat( shm_id, NULL, 0 );                      //FAZER dt
  exit_on_null ( mem, " - Erro ao ligar a memoria partilhada" );
  for (int i = 0; i < NCONSULTAS; i++ ){
    if ( mem[i].tipo == -1 ){
      inserir_consulta ( mem, i );
      exit ( 0 );
    }
  }
  printf ( "Lista de consultas cheia" );
  mensagem m;
  m.tipo = c.pid_consulta;
  snprintf ( m.texto, sizeof ( RECUSADA ), "%d", RECUSADA);
  int status = msgsnd ( mq_id, &m, sizeof( m.texto ), 0 );                        //ENVIO RECUSADA
  exit_on_error ( status, " - Erro ao recusar a consulta" );
  cperdidas++;
}

void inserir_consulta ( Consulta *mem, int indice_da_lista ){
  mem[indice_da_lista] = c;
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
  //TERMINAR SEMAFORO
  iniciar_consulta( indice_da_lista );
}

void iniciar_consulta ( int indice_da_lista ){
  printf ( " + Consulta agendada para a sala %d.\n", indice_da_lista );
  mensagem m;
  m.tipo = c.pid_consulta;
  snprintf ( m.texto, sizeof( INICIADA ), "%d", INICIADA );
  int status = msgsnd ( mq_id, &m, sizeof( m.texto ), 0 );                        //ENVIO INICIADA
  exit_on_error ( status, " - Erro ao iniciar a consulta" );
  temporizador ();
  terminar_consulta ( indice_da_lista );
}

void temporizador (){
  printf ( " + Tempo restante: " );
  for ( int i = DURACAO; i > 0; i-- ){
    fflush( stdout );
    printf ( "%d ", i );
    sleep ( 1 );
  }
  printf ( "\n" );
}

void cancelar_consulta (){
  mensagem m;
  int status = msgrcv( mq_id, &m, sizeof( m.texto ), c.pid_consulta, 0);          //RECEBER MENSAGEM
  exit_on_error( status, " - Erro ao receber pedido do cliente" );
  if ( atoi( m.texto ) == CANCELADA ) printf ( " - Consulta cancelada pelo utilizador %d.\n", c.pid_consulta );
}

void terminar_consulta ( int indice_da_lista ){
  printf ( " + Consulta terminada na sala %d.\n", indice_da_lista );
  mensagem m;
  m.tipo = c.pid_consulta;
  snprintf ( m.texto, sizeof( TERMINADA ), "%d", TERMINADA );
  int status = msgsnd ( mq_id, &m, sizeof( m.texto ), 0 );                        //ENVIO TERMINADA
  exit_on_error ( status, " - Erro ao terminar a consulta" );
}

void desligar_servidor (){
  //APRESENTAR RESULTADOS ACEDENDO AOS CONTADORES SEM RECURSO A SEMAFOROS
  int* mem = shmat(shm_id, NULL, 0);
  exit_on_null ( mem, " - Erro ao ligar a memoria partilhada" );
  printf ( " Perdidas | Normais | COVID-19 | Urgentes\n" );
  printf ( "   %d     |   %d    |    %d    |     %d\n\n", mem[0], mem[1], mem[2], mem[3] ); //ALTERAR INDICES
  n = 1;
  printf ( "\n - Servidor encerrado.\n" );
}