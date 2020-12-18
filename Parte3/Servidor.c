#include "Servidor.h"

int main (){
  iniciar_servidor();
  armar_sinal ();
  receber_pedido ();
}

void iniciar_servidor (){
  system ( "clear" );
  sem_id = semget ( KEY, 1, IPC_CREAT | 0600 );                                                                      //CRIAR SEMAFORO
  exit_on_error ( sem_id, " - Erro ao criar o semaforo" );
  printf ( "\n   + Semaforo iniciado com sucesso.\n" );
  mq_id = msgget ( KEY, IPC_CREAT | 0600 );                                                                          //CRIAR MESSAGE QUEUE
  exit_on_error ( mq_id, " - Erro ao criar a fila de mensagens" ); 
  printf ( "   + Lista de mensagens iniciada com sucesso.\n" );
  shm_id = shmget ( KEY,  NCONSULTAS * sizeof( Consulta ) + sizeof ( int )*NCONTADORES, 0 );
  if ( shm_id < 0 ){
    shm_id = shmget ( KEY, NCONSULTAS * sizeof( Consulta ) + sizeof ( int )*NCONTADORES, IPC_CREAT | 0600 );         //CRIAR SHARED MEMORY
    exit_on_error ( shm_id, " - Erro ao aceder a zona de memoria partilhada" );
    limpar_lista_consultas ();
    limpar_contadores ();
    printf ( "   + Memoria iniciada com sucesso.\n" );
  }
  else{
    shm_id = shmget ( KEY, NCONSULTAS * sizeof( Consulta ) + sizeof ( int )*NCONTADORES, 0 );
    exit_on_error ( shm_id, " - Erro ao aceder a zona de memoria partilhada" );
    printf ( "   + Memoria reutilizada com sucesso.\n" );
  }
  printf ( "\n   + A aguardar pedidos de consulta.\n\n" );
}

void limpar_lista_consultas (){
  Consulta *mem = ( Consulta * ) shmat ( shm_id, NULL, 0 );
  exit_on_null ( mem, " - Erro ao ligar a memoria partilhada" );
  mudar_semaforo ( 0 );
  for ( int i = 0; i < NCONSULTAS; i++ )  mem[i].tipo = -1;
  mudar_semaforo ( 1 );
  shmdt( mem );
}

void limpar_contadores (){
  int *mem_cont = ( int * ) shmat ( shm_id, NULL, 0 );
  exit_on_null ( mem_cont, " - Erro ao ligar a memoria partilhada" );
  mudar_semaforo ( 0 );
  for ( int i = NCONSULTAS; i < NCONSULTAS + NCONTADORES; i++ ) mem_cont[i] = 0;
  mudar_semaforo ( 1 );
  shmdt ( mem_cont );
}

void receber_pedido (){
  while ( n != 1 ) {
    mensagem m;
    int msg_status = msgrcv ( mq_id, &m, sizeof ( m.c ), PEDIDO, 0 );                                                //RECEBER PEDIDO
    if ( msg_status < 0 ) {
  	  if ( errno != EINTR ) printf(" - Houve um erro ao esperar pela mensagem (1): %s\n", strerror(errno));
    } 
    else{
      c.tipo = m.c.tipo;
      strcpy ( c.descricao, m.c.descricao );
      c.pid_consulta = m.c.pid_consulta;
      c.status = PEDIDO;
      printf ( "   + Chegou novo pedido de consulta do tipo %d, descricao '%s' e PID %d.\n", c.tipo, c.descricao, c.pid_consulta );
      tratar_pedido ();
    }
  }
}

void tratar_pedido () {
  pid_t parent;
  pid_t grandparent;
  if ( parent = fork() ) waitpid ( parent, NULL, 0 );
  else if ( !parent ){
    if ( grandparent = fork () ) exit ( 0 );
    else if ( !grandparent ){
      if ( verificar_vagas () ){
        signal ( SIGINT, SIG_IGN );
        inserir_consulta ();
        incrementar_contadores ();
        iniciar_consulta ();
      }
      else lista_cheia ();
      exit ( 0 );
    }
    else{
      perror ( " - Erro ao tratar pedido de consulta." );
      exit ( 0 );
    }
  }
  else {
    perror ( " - Erro ao tratar pedido de consulta." );
    exit ( 0 );
  }
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
  printf ( " - Lista de consultas cheia.\n" );
  mensagem m;
  m.tipo = c.pid_consulta;
  c.status = RECUSADA;
  m.c = c;
  int msg_status = msgsnd ( mq_id, &m, sizeof( m.c ), 0 );                                                           //ENVIO RECUSADA
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
  mem[indice_da_lista].status = c.status;
  mudar_semaforo ( 1 );
  shmdt( mem );
  printf ( "   + Consulta agendada para a sala %d.\n", indice_da_lista );
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

void atualizar_estado_consulta (){
  Consulta * mem = ( Consulta * ) shmat ( shm_id, NULL, 0 );
  exit_on_null ( mem, " - Erro ao ligar a memoria partilhada" );
  mudar_semaforo ( 0 ); 
  mem[indice_da_lista].status = c.status;
  mudar_semaforo ( 1 );
  shmdt( mem );
}

void iniciar_consulta (){
  mensagem m;
  m.tipo = c.pid_consulta;
  c.status = INICIADA;
  atualizar_estado_consulta ();
  m.c = c;
  int msg_status = msgsnd ( mq_id, &m, sizeof( m.c ), 0 );                                                           //ENVIO INICIADA
  exit_on_error ( msg_status, " - Erro ao iniciar a consulta" );
  alarm ( DURACAO );
  cancelar_consulta();
}

void terminar_consulta (){
  if ( c.status == INICIADA || c.status == CANCELADA ){
    printf ( "   + Consulta terminada na sala %d.\n", indice_da_lista );
    mensagem m;
    m.tipo = c.pid_consulta;
    c.status = TERMINADA;
    atualizar_estado_consulta ();
    m.c = c;
    int msg_status = msgsnd ( mq_id, &m, sizeof( m.c ), 0 );                                                         //ENVIO TERMINADA
    exit_on_error ( msg_status, " - Erro ao terminar a consulta" );
    libertar_sala ();
  }
}

void libertar_sala (){
  Consulta * mem = ( Consulta * ) shmat( shm_id, NULL, 0 );
  exit_on_null ( mem, " - Erro ao ligar a memoria partilhada" );
  mudar_semaforo ( 0 );
  mem[indice_da_lista].tipo = -1;
  mudar_semaforo ( 1 );
  shmdt( mem );
  signal ( SIGALRM, terminar_consulta );
}

void cancelar_consulta (){
  mensagem m;
  int msg_status = msgrcv( mq_id, &m, sizeof( m.c ), c.pid_consulta, 0);                                             //RECEBER CANCELADA
  if ( msg_status < 0 ) {
  	if ( errno != EINTR ) printf(" - Houve um erro ao esperar pela mensagem (2): %s\n", strerror(errno));
    } 
  else{
    if ( m.c.status == CANCELADA ){
      printf ( " - Consulta cancelada pelo utilizador %d.\n", c.pid_consulta );
      c.status = CANCELADA;
      atualizar_estado_consulta ();
      terminar_consulta ();
    }
  }
}

void mudar_semaforo ( int valor ){
  if ( valor == 0 || valor == 1 ){
    int status = semctl( sem_id, 0, SETVAL, valor );
    exit_on_error( status, " - Erro ao alterar valor do semaforo" );
  }
  else printf ( " - Valor do semaforo invalido" );
}

void desligar_servidor (){
  n = 1;
  int * mem_cont = ( int * ) shmat( shm_id, NULL, 0 );
  exit_on_null ( mem_cont, " - Erro ao ligar a memoria partilhada" );
  printf ( "\n     Perdidas | Normais | COVID-19 | Urgentes\n" );
  printf ( "         %d         %d         %d          %d\n\n", mem_cont[10], mem_cont[11], mem_cont[12], mem_cont[13] );
  shmdt( mem_cont );
  exit ( 0 );
}