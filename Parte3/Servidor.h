#include "Header.h"
#include <sys/shm.h>

#define exit_on_null(s,m) if (s==NULL) { perror(m); exit(1); }
#define DURACAO 10
#define NCONSULTAS 10
#define NCONTADORES 4

Consulta c;
int mq_id;
int n = 0;
int shm_id;
int sem_id;
int mq_id;
int cperdida = 0;
int cnormal = 0;
int ccovid = 0;
int curgente = 0;

void iniciar_servidor ();
void limpar_lista_consultas ();
void receber_pedido ();
void tratar_texto ();
void tratar_pedido ();
void verificar_vagas ();
void inserir_consulta ( int indice_da_lista );
void iniciar_consulta ( int indice_da_lista );
void terminar_consulta ( int indice_da_lista );
void libertar_sala ( int indice_da_lista );
void cancelar_consulta ();
void armar_SIGINT ();
void mudar_semaforo ( int valor );
void desligar_servidor ();
void incrementar_contadores ();