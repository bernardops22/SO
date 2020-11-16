#include "header.h"
Consulta lista_consultas[10];
int n = 0;

void limpar_lista_consultas( Consulta lista_consultas[] ){
  for ( int i = 0; i < 10; i++ ) 
    lista_consultas[i].tipo = -1;
}

/*...*/
void registar_pid(){
  FILE * file = fopen( "SrvConsultas.pid", "w" );
  fprintf( file, "%d", getpid() );
  fclose( file ); 
}

void nova_consulta(){
  FILE * file = fopen( "PedidoConsulta.txt", "r" );
  if( file != NULL ){
    char consulta[100];
    fgets(consulta, 100, file);
    
    //CONTINUAR AQUI
    
    Consulta c;
    c.tipo = 
    c.descricao = 
    c.pid_consulta = 
    printf("Chegou novo pedido de consulta do tipo %d, descricao %s e PID %d",  
    fclose ( file );
  }
}

void trata_sinal( int sinal ){
  switch( sinal ){
    case SIGUSR1:
      nova_consulta();
      break;
  }
  n = 1;
}

/*...*/
int main(){
  limpar_lista_consultas(lista_consultas);
  registar_pid();
  signal ( SIGUSR1, trata_sinal );
  while( n != 1 ) 
    pause();
}