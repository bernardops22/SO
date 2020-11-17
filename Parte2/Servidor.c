#include "Servidor.h"

int main(){
  signal ( SIGUSR1, trata_sinal );
  signal ( SIGINT, trata_sinal );
  limpar_lista_consultas( lista_consultas );
  registar_pid();
  while( n != 1 ){
    pause();
  }
}

/* ---*/
void limpar_lista_consultas( Consulta lista_consultas[] ){
  for ( int i = 0; i < 10; i++ ) 
    lista_consultas[i].tipo = -1;
}

//S2
void registar_pid(){
  FILE * file = fopen( "SrvConsultas.pid", "w" );
  fprintf( file, "%d", getpid() );
  fclose( file ); 
}

/* ---*/
void trata_sinal( int sinal ){
  switch( sinal ){
    case SIGUSR1:
      //NAO ACEITA VÁRIOS PEDIDOS EM SIMULTANEO
      // SO ACEITA NOVOS PEDIDOS QUANDO TERMINA A FUNCAO
      nova_consulta();
      break;
    //S4
    case SIGINT:
      remove ( "SrvConsultas.pid" );
      //ATUALIZAR StatsConsultas.dat
      n = 1;
      break;
  }
}

/* ---*/
void nova_consulta(){
  //S3.1
  FILE * file = fopen( "PedidoConsulta.txt", "r" );
  if ( file != NULL ){
    char line [100];
    Consulta c;
    c.tipo = atoi( fgets( line, sizeof( int ), file ) );
    fscanf( file, "%[^\n]", c.descricao );
    fgets( line, 100, file );
    c.pid_consulta = atoi( fgets( line, PID_MAX, file ) );
    //S3.2
    printf( "Chegou novo pedido de consulta do tipo %d, descricao '%s' e PID %d\n", c.tipo, c.descricao, c.pid_consulta );  
    fclose ( file );
    verificar_vagas( c );
  }
}

void verificar_vagas( Consulta c ){
  //S3.3
  for (int i = 0; i < 10; i++)
    if ( lista_consultas[i].tipo == -1 ){
      indice_da_lista = i;
      break;
    }
  if ( indice_da_lista == 10 ){
    printf ( "Lista de consultas cheia\n" );
    kill ( c.pid_consulta, SIGUSR2 );
    cperdidas++;
  }
  else{
    inserir_consulta ( c );
  }
}

//S3.4
void inserir_consulta ( Consulta c ){
  lista_consultas[indice_da_lista].tipo = c.tipo;
  strncpy ( lista_consultas[indice_da_lista].descricao, c.descricao, 99 );
  lista_consultas[indice_da_lista].pid_consulta = c.pid_consulta;    
  printf ( "Consulta agendada para a sala %d\n", indice_da_lista );
  switch( c.tipo ){
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
  iniciar_consulta( c );
}

void iniciar_consulta( Consulta c ){
  pid_t parent = fork();
  if ( !parent ){
    //S3.4.1
    kill ( c.pid_consulta, SIGHUP );
    //S3.4.2
    sleep( 10 );
    printf( "Consulta terminada na sala %d\n", indice_da_lista );
    kill ( c.pid_consulta, SIGTERM );
    exit (1);
  }
  else  {
    wait (NULL);
    lista_consultas[indice_da_lista].tipo = -1;
    memset( lista_consultas[indice_da_lista].descricao,0,99 );
    lista_consultas[indice_da_lista].pid_consulta = 0;
  }
}