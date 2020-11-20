#include "Servidor.h"

int main (){
  iniciar_servidor();
  limpar_lista_consultas ();
  registar_pid ();
  armar_sinal ();
  while ( n != 1 ) pause();
}

void iniciar_servidor (){
  system ( "clear" );
  printf( "\n + A iniciar o servidor" );
  for ( int i = 1; i < 6; i++ ){
    fflush( stdout );
    sleep ( 1 );
    printf ( "." );
  }
  printf ( "\n + Servidor iniciado com sucesso.\n + A aguardar pedidos de consulta.\n\n" );
}

void limpar_lista_consultas (){
  for ( int i = 0; i < 10; i++ )
    lista_consultas[i].tipo = -1;
}

void registar_pid (){
  FILE * file = fopen ( SERVIDOR_PID, "w" );
  fprintf ( file, "%d", getpid() );
  fclose ( file ); 
}

void armar_sinal (){
  signal ( SIGUSR1, trata_sinal );
  signal ( SIGINT, trata_sinal );
}

void trata_sinal ( int sinal ){
  switch ( sinal ){
    case SIGUSR1:
        signal ( SIGINT, SIG_IGN );
        nova_consulta ();
        verificar_vagas ();
        signal ( SIGINT, trata_sinal );
    break;
    case SIGINT:
      desligar_servidor ();
      break;
  }
}

void nova_consulta (){
  FILE * file = fopen ( PEDIDO_CONSULTA, "r" );
  if ( file != NULL ){
    char line[100];
    c.tipo = atoi ( fgets( line, sizeof( int ), file ) );
    fscanf ( file, "%[^\n]", c.descricao );
    fgets ( line, 100, file );
    c.pid_consulta = atoi ( fgets( line, PID_MAX, file ) );
    printf ( " + Chegou novo pedido de consulta do tipo %d, descricao '%s' e PID %d\n", c.tipo, c.descricao, c.pid_consulta );  
    fclose ( file );
  }
  else perror ( " - Erro na leitura de PedidoConsulta.txt" );
}

void verificar_vagas (){
  for ( int i = 0; i < 10; i++ ){
    if ( lista_consultas[i].tipo == -1 ){
      indice_da_lista = i + 1;
      break;
    }
  }
  if ( indice_da_lista == 10 ){
    printf ( " - Lista de consultas cheia\n" );
    kill ( c.pid_consulta, SIGUSR2 );
    cperdidas++;
  }
  else inserir_consulta ();
}

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
}

void iniciar_consulta (){
  pid_t parent = fork ();
  if ( !parent ){
    kill ( c.pid_consulta, SIGHUP );
    temporizador();
    printf ( " + Consulta terminada na sala %d\n\n", indice_da_lista );
    kill ( c.pid_consulta, SIGTERM );
    exit ( 0 );
  }
  else  {
    wait ( NULL );
    lista_consultas[indice_da_lista-1].tipo = -1;
    memset ( lista_consultas[indice_da_lista-1].descricao, 0, 99 );
    lista_consultas[indice_da_lista-1].pid_consulta = 0;
  }
}

void desligar_servidor (){
  remove ( SERVIDOR_PID );
  FILE * file = fopen ( STATS_CONSULTAS, "w" );
  fwrite ( &cperdidas, sizeof( cperdidas ), 1, file );
  fwrite ( &cnormal, sizeof( cnormal ), 1, file );
  fwrite ( &ccovid19, sizeof( ccovid19 ), 1, file );
  fwrite ( &curgente, sizeof( curgente ), 1, file );
  n = 1;
  printf ( "\n - Servidor encerrado.\n" );
}