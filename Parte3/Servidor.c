#include "Servidor.h"

int main (){
  iniciar_servidor();
  limpar_lista_consultas ();
  armar_SIGINT ();
  receber_pedido ();
}

void iniciar_servidor (){
  system ( "clear" );
  sem_id = semget ( KEY, 1, IPC_CREAT | 0666 );                                                                      //CRIAR SEMAFORO
  exit_on_error ( sem_id, " - Erro ao criar o semaforo" );
  mq_id = msgget ( KEY, IPC_CREAT | 0666 );                                                                          //CRIAR SHARED MEMORY
  exit_on_error ( mq_id, " - Erro ao criar a fila de mensagens" ); 
  shm_id = shmget ( KEY, NCONSULTAS * sizeof( Consulta ) + sizeof ( int * )*NCONTADORES, IPC_CREAT | 0666 );         //CRIAR MESSAGE QUEUE
  exit_on_error ( shm_id, " - Erro ao aceder a zona de memoria partilhada" );
  printf ( "\n   + Servidor iniciado com sucesso.\n" );
}

void limpar_lista_consultas (){
  Consulta *mem = ( Consulta * ) shmat ( shm_id, NULL, 0 );
  exit_on_null ( mem, " - Erro ao ligar a memoria partilhada" );
  mudar_semaforo ( 0 );
  for ( int i = 0; i < NCONSULTAS; i++ )  mem[i].tipo = -1;
  shmdt( mem );
  mudar_semaforo ( 1 );
  printf ( "   + Memoria iniciada com sucesso\n   + A aguardar pedidos de consulta.\n\n" );
}

void receber_pedido (){
  while ( n != 1 ) {
    mensagem m;
    int msg_status = msgrcv ( mq_id, &m, sizeof ( m.texto ), PEDIDO, 0 );                                            //RECEBER MENSAGEM
    exit_on_error ( msg_status, " - Erro ao receber pedido do cliente" );
    tratar_texto ( m.texto );
    printf ( "   + Chegou novo pedido de consulta do tipo %d, descricao '%s' e PID %d\n", c.tipo, c.descricao, c.pid_consulta );
    tratar_pedido ();
  }
}

void tratar_texto ( char texto[] ){
  char *pt;
  pt = strtok ( texto, "," );
  c.tipo = atoi ( pt );
  pt = strtok ( NULL, "," );
  strcpy ( c.descricao, pt );
  pt = strtok ( NULL, "," );
  c.pid_consulta = atoi ( pt );
}

void tratar_pedido () {
  pid_t parent = fork ();
  if ( !parent ){
    verificar_vagas ();
    exit ( 0 );
  }
  else  {
    //ELIMINAR ZOMBIES
  }
}

void verificar_vagas (){
  Consulta* mem = ( Consulta * ) shmat ( shm_id, NULL, 0 );
  exit_on_null ( mem, " - Erro ao ligar a memoria partilhada" );
  mudar_semaforo ( 0 );
  for ( int i = 0; i < NCONSULTAS; i++ ){
    if ( mem[i].tipo == -1 ){
      mudar_semaforo ( 1 );
      inserir_consulta ( i );
      incrementar_contadores ();
      iniciar_consulta ( i );
      exit ( 0 );
    }
  }
  shmdt( mem );
  mudar_semaforo ( 1 );
  printf ( " - Lista de consultas cheia\n" );
  mensagem m;
  m.tipo = c.pid_consulta;
  snprintf ( m.texto, sizeof ( RECUSADA ), "%d", RECUSADA );
  int msg_status = msgsnd ( mq_id, &m, sizeof( m.texto ), 0 );                                                       //ENVIO RECUSADA
  exit_on_error ( msg_status, " - Erro ao recusar a consulta" );
  int * mem_cont = ( int * ) shmat ( shm_id, NULL, 0 );
  exit_on_null ( mem_cont, " - Erro ao ligar a memoria partilhada" );
  mudar_semaforo ( 0 );
  mem_cont[10]++;
  shmdt( mem_cont );  
  mudar_semaforo ( 1 );
}

void inserir_consulta ( int indice_da_lista ){
  Consulta * mem = ( Consulta * ) shmat ( shm_id, NULL, 0 );
  exit_on_null ( mem, " - Erro ao ligar a memoria partilhada" );
  mudar_semaforo ( 0 );
  mem[indice_da_lista].tipo = c.tipo;
  strcpy( mem[indice_da_lista].descricao,  c.descricao);
  mem[indice_da_lista].pid_consulta = c.pid_consulta;  
  shmdt( mem );
  mudar_semaforo ( 1 );
  printf ( "   + Consulta agendada para a sala %d\n", indice_da_lista );
}

void incrementar_contadores (){
  int * mem_cont = ( int * ) shmat ( shm_id, NULL, 0 );  
  exit_on_null ( mem_cont, " - Erro ao ligar a memoria partilhada" );
  mudar_semaforo ( 0 );
  switch ( c.tipo ){
    case 1:
      mem_cont[11]++;
      break;
    case 2:
      mem_cont[12]++;
      break;
    case 3:
      mem_cont[13]++;
      break;
  }
  shmdt( mem_cont );
  mudar_semaforo ( 1 );
}

void iniciar_consulta ( int indice_da_lista ){
  mensagem m;
  m.tipo = c.pid_consulta;
  snprintf ( m.texto, sizeof( INICIADA ), "%d", INICIADA );
  int msg_status = msgsnd ( mq_id, &m, sizeof( m.texto ), 0 );                                                       //ENVIO INICIADA
  exit_on_error ( msg_status, " - Erro ao iniciar a consulta" );
  //cancelar_consulta ();
  sleep ( DURACAO );
  terminar_consulta ( indice_da_lista );
  libertar_sala ( indice_da_lista );
}

void terminar_consulta ( int indice_da_lista ){
  printf ( "   + Consulta terminada na sala %d.\n", indice_da_lista );
  mensagem m;
  m.tipo = c.pid_consulta;
  snprintf ( m.texto, sizeof( TERMINADA ), "%d", TERMINADA );
  int msg_status = msgsnd ( mq_id, &m, sizeof( m.texto ), 0 );                                                       //ENVIO TERMINADA
  exit_on_error ( msg_status, " - Erro ao terminar a consulta" );
}

void libertar_sala ( int indice_da_lista ){
  Consulta * mem = ( Consulta * ) shmat( shm_id, NULL, 0 );
  exit_on_null ( mem, " - Erro ao ligar a memoria partilhada" );
  mudar_semaforo ( 0 );
  mem[indice_da_lista].tipo = -1;
  shmdt( mem );
  mudar_semaforo ( 1 );
}

/*void cancelar_consulta (){
  mensagem m;
  int msg_status = msgrcv( mq_id, &m, sizeof( m.texto ), c.pid_consulta, 0);                                         //RECEBER MENSAGEM
  exit_on_error( msg_status, " - Erro ao receber pedido do cliente" );
  if ( atoi( m.texto ) == CANCELADA ) printf ( " - Consulta cancelada pelo utilizador %d.\n", c.pid_consulta );
}*/

void armar_SIGINT (){
  signal ( SIGINT, desligar_servidor );
}

void mudar_semaforo ( int valor ){
  int status = semctl( sem_id, 0, SETVAL, valor );
  exit_on_error( status, " - Erro ao alterar valor do semaforo" );
}

void desligar_servidor (){
  int * mem_cont = ( int * ) shmat( shm_id, NULL, 0 );
  exit_on_null ( mem_cont, " - Erro ao ligar a memoria partilhada" );
  mudar_semaforo ( 0 );
  printf ( "\n   Perdidas | Normais | COVID-19 | Urgentes\n" );
  printf ( "      %d          %d         %d          %d\n", mem_cont[10], mem_cont[11], mem_cont[12], mem_cont[13] );
  shmdt( mem_cont );
  mudar_semaforo ( 1 );
  int mq_status = msgctl( mq_id , IPC_RMID, NULL );                                                                  //REMOVER MESSAGE QUEUE
  exit_on_error ( mq_status, " - Erro ao remover fila de mensagens" );
  printf ( "\n   + Servidor encerrado.\n" );
  n = 1;
}