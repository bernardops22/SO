#include "Header.h"

#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>


#define exit_on_error(s,m) if (s<0) { perror(m); exit(1);}
#define exit_on_null(s,m) if (s==NULL) { perror(m); exit(1); }
#define KEY 0x0a92439
#define NCONSULTAS 10
#define NCONTADORES 4
#define DURACAO 10

Consulta c;
int n = 0;
int shm_id;
int sem_id;
int mq_id;
int indice_da_lista;

void iniciar_servidor ();           //S1
void limpar_lista_consultas ();     //S1
void limpar_contadores ();          //S1
void receber_pedido ();             //S2 S3.1 S3.2 S5
void tratar_pedido ();              //S3.3 S3.3.1 S3.3.2
int verificar_vagas ();             //S3.3.1 S3.3.2
void lista_cheia ();                //S3.3.1
void inserir_consulta ();           //S3.3.2
void incrementar_contadores ();     //S3.3.2
void armar_sinal ();                //S3.3.2 S4
void iniciar_consulta ();           //S3.3.2
void terminar_consulta ();          //S3.3.2
void libertar_sala ();
void cancelar_consulta ();          //S3.3.3
void mudar_semaforo ( int valor );
void desligar_servidor ();          //S4
int is_lista_limpa();              //S4