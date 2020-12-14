#include "Servidor.h"

int main (){
  iniciar_servidor();
  limpar_lista_consultas ();
  armar_sinal ();
  while ( n != 1 ){
    receber_pedido ();
    signal ( SIGINT, desligar_servidor );
  }
}

void iniciar_servidor (){
  system ( "clear" );
  sem_id = semget ( KEY, 1, IPC_CREAT | 0600 );                                                                      //CRIAR SEMAFORO
  exit_on_error ( sem_id, " - Erro ao criar o semaforo." );
  printf ( "\n   + Semaforo iniciado com sucesso\n" );
  mq_id = msgget ( KEY, IPC_CREAT | 0600 );                                                                          //CRIAR MESSAGE QUEUE
  exit_on_error ( mq_id, " - Erro ao criar a fila de mensagens." ); 
  printf ( "   + Lista de mensagens iniciada com sucesso\n" );
  shm_id = shmget ( KEY, NCONSULTAS * sizeof( Consulta ) + sizeof ( int )*NCONTADORES, IPC_CREAT | 0600 );         //CRIAR SHARED MEMORY
  exit_on_error ( shm_id, " - Erro ao aceder a zona de memoria partilhada." );
}

void limpar_lista_consultas (){
  Consulta *mem = ( Consulta * ) shmat ( shm_id, NULL, 0 );
  exit_on_null ( mem, " - Erro ao ligar a memoria partilhada" );
  mudar_semaforo ( 0 );
  for ( int i = 0; i < NCONSULTAS; i++ )  mem[i].tipo = -1;
  mudar_semaforo ( 1 );
  shmdt( mem );
  printf ( "   + Memoria iniciada com sucesso\n" );
  printf ( "\n   + A aguardar pedidos de consulta.\n\n" );
}

void receber_pedido (){
  mensagem m;
  int msg_status = msgrcv ( mq_id, &m, sizeof ( m.texto ), PEDIDO, 0 );                                            //RECEBER PEDIDO
  if ( msg_status < 0 ) {
    if ( errno != EINTR ) printf(" - Erro ao esperar pela mensagem: %s\n", strerror(errno));
  } 
  else {
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
  signal ( SIGINT, SIG_IGN );
  pid_t parent = fork ();
  if ( parent < 0 ){
   perror ( " - Erro ao tratar pedido de consulta\n" );
   exit ( 0 );
  }
  if ( !parent ){
    if ( verificar_vagas () ){
      inserir_consulta ();
      incrementar_contadores ();
      iniciar_consulta ();
    }
    else lista_cheia ();
    exit ( 0 );
  }
  else if ( waitpid ( parent, NULL, 0 ) < 0) perror(" - Falha ao libertar processo filho\n");
}

int verificar_vagas (){
  Consulta* mem = ( Consulta * ) shmat ( shm_id, NULL, 0 );
  exit_on_null ( mem, " - Erro ao ligar a memoria partilhada" );
  mudar_semaforo ( 0 );
  for (int i = 0; i < NCONSULTAS; i++ ){
    if ( mem[i].tipo == -1 ){
      indice_da_lista = i;
      mudar_semaforo ( 1 );
      shmdt ( mem );
      return 1;
    }
  }
  return 0;
}

void lista_cheia (){
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
  mudar_semaforo ( 1 );
  shmdt( mem_cont );
}

void inserir_consulta (){
  Consulta * mem = ( Consulta * ) shmat ( shm_id, NULL, 0 );
  exit_on_null ( mem, " - Erro ao ligar a memoria partilhada" );
  mudar_semaforo ( 0 );
  mem[indice_da_lista].tipo = c.tipo;
  strcpy( mem[indice_da_lista].descricao,  c.descricao);
  mem[indice_da_lista].pid_consulta = c.pid_consulta;  
  mudar_semaforo ( 1 );
  shmdt( mem );
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
  mudar_semaforo ( 1 );
  shmdt( mem_cont );
}

void armar_sinal (){
  signal ( SIGINT, desligar_servidor );
  signal ( SIGALRM, terminar_consulta );
}

void iniciar_consulta (){
  mensagem m;
  m.tipo = c.pid_consulta;
  snprintf ( m.texto, sizeof( INICIADA ), "%d", INICIADA );
  int msg_status = msgsnd ( mq_id, &m, sizeof( m.texto ), 0 );                                                       //ENVIO INICIADA
  exit_on_error ( msg_status, " - Erro ao iniciar a consulta" );
  alarm ( DURACAO );
  cancelar_consulta();
}

void terminar_consulta (){
  printf ( "   + Consulta terminada na sala %d.\n", indice_da_lista );
  mensagem m;
  m.tipo = c.pid_consulta;
  snprintf ( m.texto, sizeof( TERMINADA ), "%d", TERMINADA );
  int msg_status = msgsnd ( mq_id, &m, sizeof( m.texto ), 0 );                                                       //ENVIO TERMINADA
  exit_on_error ( msg_status, " - Erro ao terminar a consulta" );
  libertar_sala ();
}

void libertar_sala (){
  Consulta * mem = ( Consulta * ) shmat( shm_id, NULL, 0 );
  exit_on_null ( mem, " - Erro ao ligar a memoria partilhada" );
  mudar_semaforo ( 0 );
  mem[indice_da_lista].tipo = -1;
  mudar_semaforo ( 1 );
  shmdt( mem );
}

void cancelar_consulta (){
  mensagem m;
  int msg_status = msgrcv( mq_id, &m, sizeof( m.texto ), c.pid_consulta, 0);                                         //RECEBER CANCELADA
  if ( msg_status < 0 ) {
    	if ( errno != EINTR ) printf(" - Erro ao esperar pela mensagem: %s\n", strerror(errno));
    } 
  else{
      if ( atoi( m.texto ) == CANCELADA ) printf ( " - Consulta cancelada pelo utilizador %d.\n", c.pid_consulta );
      terminar_consulta ();
    }
}

void mudar_semaforo ( int valor ){
  int status = semctl( sem_id, 0, SETVAL, valor );
  exit_on_error( status, " - Erro ao alterar valor do semaforo" );
}

void desligar_servidor (){
  int * mem_cont = ( int * ) shmat( shm_id, NULL, 0 );
  exit_on_null ( mem_cont, " - Erro ao ligar a memoria partilhada" );
  mudar_semaforo ( 0 );
  printf ( "\n     Perdidas | Normais | COVID-19 | Urgentes\n" );
  printf ( "         %d         %d         %d          %d\n\n", mem_cont[10], mem_cont[11], mem_cont[12], mem_cont[13] );
  mudar_semaforo ( 1 );
  shmdt( mem_cont );
  int mq_status = msgctl( mq_id , IPC_RMID, NULL );                                                                  //REMOVER MESSAGE QUEUE
  exit_on_error ( mq_status, " - Erro ao remover fila de mensagens" );
  int sem_status = semctl ( sem_id, 0, IPC_RMID );                                                                   //REMOVER SEMAFORO
  exit_on_error ( sem_status , " - Erro ao remover fila de mensagens" );
  n = 1;
}